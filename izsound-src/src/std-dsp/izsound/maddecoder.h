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

#ifndef IZSOUND_MADDECODER_H
#define IZSOUND_MADDECODER_H

#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <deque>

#include <mad.h>

#include <player.h>
#include <dspunit.h>

#define INPUT_BUFFER_SIZE	(5*8192)

namespace izsound
{

/**
 * A MPEG files decoder DSP unit. It uses the Mad library to perform the job.
 * Due to the way that the Mad library is implemented, the seeking and total
 * time account methods require a full file scan when it is opened. This
 * requires a few seconds ...
 *
 * This unit has no input slot and one output slot.
 *
 * @author Julien PONGE <julien@izforge.com>
 */
class MadDecoder : public DspUnit, virtual public Player
{

private:

  /** Playing status enumeration. */
  enum PlayingState {PLAY, PAUSE, STOP};

  /** The player current status. */
  PlayingState m_playerStatus;

  /** The internal input buffer. */
  unsigned char m_inputBuffer[INPUT_BUFFER_SIZE];

  /** The frames counter. */
  unsigned long m_frameCount;

  /** libmad stream. */
  struct mad_stream	m_stream;

  /** libmad frame. */
  struct mad_frame m_frame;

  /** libmad synth. */
  struct mad_synth m_synth;

  /** libmad timer. */
  mad_timer_t m_timer;

  /** The input file. */
  FILE* m_inputFile;

  /** The file size (counted in number of 'unsigned int'). */
  unsigned long m_fileSize;

  /** True when the end of the file is reached. */
  bool m_endReached;

  /** The current file total time, in seconds. */
  double m_totalTime;

  /**
   * Performs the constructors common initialisation job.
   */
  void commonInits();

  /**
   * Initializes the libmad structures.
   */
  void initMadStructs();

  /**
   * Releases the libmad structures.
   */
  void releaseMadStructs();

  /**
   * Closes the current file.
   */
  void closeFile();

  /**
   * Plays the next chunck of data.
   */
  void playNextChunk();

  /**
   * Plays nothing.
   */
  void playNothing();

  /**
   * Converts a sample from mad's fixed point number format to an unsigned
   * short (16 bits).
   *
   * @param Fixed The fixed point number to convert.
   * @return The converted number.
   */
  inline int scale(mad_fixed_t sample);

  /**
   * Scans the input file to prepare the unit to be able to make some
   * operations that are not directly allowed by the mad library like getting
   * the duration or seeking.
   */
  void scanInputFile();

protected:

  /**
   * Performs the decoding job.
   */
  virtual void performDsp();

public:

  /**
   * Constructs a decoder with no file loaded.
   *
   * @param sampleRate The audio chain sample rate.
   */
  MadDecoder(const unsigned int &sampleRate = 44100);

  /**
   * Constructs a decoder with a file to open.
   *
   * @param filename The file to open.
   * @param success <code>true</code> if the file opening succeeded,
   *                <code>false</code> otherwise. You must not use the decoder
   *                in the last case !
   * @param sampleRate The audio chain sample rate.
   */
  MadDecoder(const char* filename, bool &success,
             const unsigned int &sampleRate = 44100);

  /**
   * The desctructor.
   */
  virtual ~MadDecoder();

  /**
   * Play operation. The expected behaviour is to start playing from the
   * current position.
   */
  virtual void play();

  /**
   * Pause operation.
   */
  virtual void pause();

  /**
   * Stop operation. The expected behaviour is to go back to the beginning of
   * the source.
   */
  virtual void stop();

  /**
   * Opens a file from a filename.
   *
   * @param filename The filename.
   * @param success Indicates wether the operation succeeded or not.
   */
  virtual void open(const char* filename, bool &success);

  /**
   * Indicates if we have reached the end of the stream or not.
   *
   * @return <code>true</code> if we have reached the end, <code>false</code>
   *         otherwise.
   */
  virtual bool isEndReached();

  /**
   * Returns the total stream playing time, in seconds.
   *
   * @return The total playing time.
   */
  virtual double getTotalTime();

  /**
   * Seeks to a position.
   *
   * @param pos The position in seconds.
   */
  virtual void seek(const double &pos);

  /**
   * Gets the current time, in seconds.
   *
   * @return The current time, in seconds.
   */
  virtual double getCurrentTime();

};

}

/*
 * Used for internal purposes.
 */

typedef struct
{
  FILE* file;
  mad_timer_t timer;
  unsigned long dataCounter;
} timing_data;

enum mad_flow input_cb(void *data, struct mad_stream *stream);
enum mad_flow output_cb(void *data, struct mad_header const *header,
                        struct mad_pcm *pcm);
enum mad_flow error_cb(void *data, struct mad_stream *stream,
                       struct mad_frame *frame);
#endif

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
