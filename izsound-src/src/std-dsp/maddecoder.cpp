/*
 * IzSound - Copyright (c) 2003, 2004 Julien PONGE - All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Please see at the bottom of this file for more informations on the exact
 * licensing terms of this file.
 */

#include "maddecoder.h"

using namespace std;
using namespace izsound;

MadDecoder::MadDecoder(const unsigned int &sampleRate)
  : DspUnit(sampleRate, 0, 1)
{
  // Init
  commonInits();
  m_playerStatus = STOP;
}

MadDecoder::MadDecoder(const char* filename, bool &success,
                       const unsigned int &sampleRate)
  : DspUnit(sampleRate, 0, 1)
{
  // Init
  commonInits();
  m_playerStatus = PLAY;
  open(filename, success);
}

void MadDecoder::commonInits()
{
  m_frameCount      = 0;
  m_inputFile       = 0;
  m_endReached      = false;
}

void MadDecoder::initMadStructs()
{
  mad_stream_init(&m_stream);
  mad_frame_init(&m_frame);
  mad_synth_init(&m_synth);
  mad_timer_reset(&m_timer);
}

void MadDecoder::releaseMadStructs()
{
  mad_synth_finish(&m_synth);
  mad_frame_finish(&m_frame);
  mad_stream_finish(&m_stream);
  mad_timer_reset(&m_timer);
}

MadDecoder::~MadDecoder()
{
  closeFile();
}

void MadDecoder::closeFile()
{
  if (m_inputFile == 0) return;
  fclose(m_inputFile);
  releaseMadStructs();
}

void MadDecoder::performDsp()
{
  switch (m_playerStatus)
  {
    case PLAY  : playNextChunk(); break;
    case PAUSE : playNothing();   break;
    case STOP  : playNothing();   break;
  }
}

void MadDecoder::playNothing()
{
  SlotData* output = m_outSlots[0];
  (*output)[0].clear();
  (*output)[1].clear();
}

void MadDecoder::playNextChunk()
{
  // Fills the input buffer
fill_me:
  if(m_stream.buffer == NULL || m_stream.error == MAD_ERROR_BUFLEN)
  {
    // Vars
    size_t readSize;
    size_t remaining;
    unsigned char* readStart;

   // There can be residual data in the input buffer
    if(m_stream.next_frame != NULL)
    {
      remaining = m_stream.bufend - m_stream.next_frame;
      memmove(m_inputBuffer, m_stream.next_frame, remaining);
      readStart = m_inputBuffer + remaining;
      readSize  = INPUT_BUFFER_SIZE - remaining;
    }
    else
    {
      readSize  = INPUT_BUFFER_SIZE;
      readStart = m_inputBuffer;
      remaining = 0;
    }

    // We read from the file
    readSize = fread(readStart, 1, readSize, m_inputFile);
    if(readSize <= 0)
    {
      if(feof(m_inputFile))
      {
        m_endReached = true;
      }
      stop();
      return;
    }

    // We fill our mad buffer
    mad_stream_buffer(&m_stream, m_inputBuffer, readSize + remaining);
    m_stream.error = MAD_ERROR_NONE;
  }

  // Decoding
  if(mad_frame_decode(&m_frame, &m_stream))
  {
    if(MAD_RECOVERABLE(m_stream.error))
    {
      goto fill_me;
    }
    else
    {
      if(m_stream.error == MAD_ERROR_BUFLEN)
      {
        goto fill_me;
      }
      else
      {
        stop();
        return;
      }
    }
  }

  // Frames & timer update
  ++m_frameCount;
  mad_timer_add(&m_timer, m_frame.header.duration);

  // PCM synth
  mad_synth_frame(&m_synth, &m_frame);

  // Now, we have some fresh samples
  SlotData* output = m_outSlots[0];
  short sample;
  double left;
  double right;
  (*output)[0].clear();
  (*output)[1].clear();
  for(int i = 0; i < m_synth.pcm.length; ++i)
  {
    // Left channel
    sample = scale(m_synth.pcm.samples[0][i]);
    left = ((double)sample) / 32768.0;

    // Right channel
    if(MAD_NCHANNELS(&m_frame.header) == 2)
    {
      sample = scale(m_synth.pcm.samples[1][i]);
    }
    right = ((double)sample) / 32768.0;

    // We send it
    (*output)[0].push_back(left);
    (*output)[1].push_back(right);
  }
}

int MadDecoder::scale(mad_fixed_t sample)
{
  // Rounding
  sample += (1L << (MAD_F_FRACBITS - 16));

  // Clipping
  if (sample >= MAD_F_ONE)
  {
    sample = MAD_F_ONE - 1;
  }
  else if (sample < -MAD_F_ONE)
  {
    sample = -MAD_F_ONE;
  }

  // Quantization
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

void MadDecoder::play()
{
  m_playerStatus = PLAY;
}

void MadDecoder::pause()
{
  m_playerStatus = PAUSE;
}

void MadDecoder::stop()
{
  m_playerStatus = STOP;
  m_frameCount = 0;
  releaseMadStructs();
  initMadStructs();
  if (m_inputFile != 0)
  {
    rewind(m_inputFile);
  }
}

void MadDecoder::open(const char* filename, bool &success)
{
  closeFile();
  initMadStructs();
  m_inputFile = fopen(filename, "rb");
  success = (m_inputFile != NULL);
  m_endReached = false;
  m_frameCount = 0;
}

bool MadDecoder::isEndReached()
{
  return m_endReached;
}

double MadDecoder::getTotalTime()
{
  return 0.0; // TODO
}

void MadDecoder::seek(const double &pos)
{
  // TODO
}

double MadDecoder::getCurrentTime()
{
  double msecs = (double)mad_timer_count(m_timer, MAD_UNITS_MILLISECONDS);
  return (msecs / 1000.0);
}

/*
 * This implementation is based on the madldd program from Bertrand Petit.
 * I found it at http://www.bsd-dk.dk/~elrond/audio/madlld/ .
 * Indeed, the Mad library is _not_ documented (or if their authors think so,
 * they are _really_ ... mad). Its madldd program is actually an example of
 * how to use the libmad low-level functions. It is heavily documented and
 * helps understanding how it is possible to use this library.
 *
 * I have no doubt that the Mad library produces a high-quality decoding.
 * However, I have no doubt that its API is awful, and not having a single
 * clean documentation file (appart of the minimad.c file) is absolutely not
 * developer-friendly.
 *
 * At first, I tried to use the high-level function, as described in the
 * minimad.c example file. Indeed, the high-level API is quite easy to
 * understand. However, it has the following flaw: the decoding function holds
 * the looping logic. As a consequence, your program can't have the control of
 * the decoding process unless you do some silly hacks. You have the option to
 * call this function and tell it to run asynchronously. This way, the function
 * returns and you can do some other things. What happens is that the decoding
 * loops runs in a forked process. So with the needs I have (grab the data by
 * small chunks), the only issue was to do some silly hacks like using Unix
 * sockets just to get the data. Indeed, it would be produced in another
 * process ... That's why I had no choice but to use the low-level functions.
 *
 * My implementation is a great refactor of madldd.c. Several things have been
 * changed and adapted to meet my needs. It now has a vague ressemblance with
 * the original work, but its author _really_ deserves some credit.
 */

/*
 * The following copyright notice gives back credit to Bertrand Petit.
 */

/****************************************************************************
 * madlld.c -- A simple program decoding an mpeg audio stream to 16-bit     *
 * PCM from stdin to stdout. This program is just a simple sample           *
 * demonstrating how the low-level libmad API can be used.                  *
 *--------------------------------------------------------------------------*
 * (c) 2001, 2002 Bertrand Petit                                            *
 *                                                                          *
 * Redistribution and use in source and binary forms, with or without       *
 * modification, are permitted provided that the following conditions       *
 * are met:                                                                 *
 *                                                                          *
 * 1. Redistributions of source code must retain the above copyright        *
 *    notice, this list of conditions and the following disclaimer.         *
 *                                                                          *
 * 2. Redistributions in binary form must reproduce the above               *
 *    copyright notice, this list of conditions and the following           *
 *    disclaimer in the documentation and/or other materials provided       *
 *    with the distribution.                                                *
 *                                                                          *
 * 3. Neither the name of the author nor the names of its contributors      *
 *    may be used to endorse or promote products derived from this          *
 *    software without specific prior written permission.                   *
 *                                                                          *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''       *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED        *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A          *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR       *
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,             *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT         *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF         *
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND      *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,       *
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT       *
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF       *
 * SUCH DAMAGE.                                                             *
 *                                                                          *
 ****************************************************************************/

/*
 * The implementation makes use of a few lines of code coming from the minimad.c
 * file shipped with the Mad library distribution. The related code can be found
 * in MadDecoder::scale. As a consequence, the following file use is actually
 * governed by the GNU GPL, unless you provide an alternative implementation
 * for MadDecoder::scale.
 */

/*
 * libmad - MPEG audio decoder library
 * Copyright (C) 2000-2003 Underbit Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
