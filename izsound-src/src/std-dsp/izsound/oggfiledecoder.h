/*
 * IzSound - Copyright (c) 2003 Julien PONGE - All rights reserved.
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

#ifndef IZSOUND_OGGFILEDECODER_H
#define IZSOUND_OGGFILEDECODER_H

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <player.h>
#include <dspunit.h>

namespace izsound
{

/**
 * Decodes the audio stream contained in an OGG Vorbis file. This unit has
 * no input slot and one output slot.
 *
 * @author Julien PONGE <julien@izforge.com>
 */
class OggFileDecoder : public DspUnit, virtual public Player
{

private:

  /** The OGG file descriptor. */
  OggVorbis_File* m_oggFile;

  /** The reading buffer size. */
  unsigned int m_readBufferSize;

  /** Thereading buffer. */
  char* m_readBuffer;

  /** Tells wether we've reached the end of the stream or not. */
  bool m_endReached;

  /**
   * Clears the current ogg file descriptor, if any.
   */
  void clearOggDescriptor();

  /**
   * Playing status enumeration.
   */
  enum PlayingState {PLAY, PAUSE, STOP};

  /**
   * The player current status.
   */
  PlayingState m_playerStatus;

protected:

  /**
   * Performs the decoding job, sending to the next unit one chunk of data.
   */
  virtual void performDsp();

  /**
   * Plays the next current OGG file decoded chunk.
   */
  void playNextChunk();

  /**
   * Plays nothing (clears the next DSP unit buffers).
   */
  void playNothing();

public:

  /**
   * The constructor. This one starts with a file and in the playing state.
   *
   * @param filename The valid path to the OGG file to read.
   * @param success A boolean that states wether the file opening succeeded or
   *        not. If <code>false</code>, don't play with the unit !
   * @param bufferSize The buffer size to use for the decoding. Note that this
            is a maximum size, but nothing guaranties that the buffer will be
            full at each <code>performDsp()</code> invocation.
  * @param sampleRate The audio chain sample rate.   */
  OggFileDecoder(const char* filename, bool &success,
                 const unsigned int &bufferSize = 4096,
                 const unsigned int &sampleRate = 44100);

  /**
   * The constructor. This ones starts with no file loaded.
   */                 
  OggFileDecoder(const unsigned int &bufferSize = 4096,
                 const unsigned int &sampleRate = 44100);

  /**
   * The destructor.
   */
  virtual ~OggFileDecoder();

  /**
   * Tells wether the end of the stream has been reached or not.
   */
  inline virtual bool isEndReached() { return m_endReached; }

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
   * Returns the total stream playing time, in seconds.
   *
   * @return The total playing time.
   */
  inline virtual double getTotalTime();

  /**
   * Seeks to a position.
   *
   * @param pos The position in seconds.
   */
  inline virtual void seek(const double &pos);

  /**
   * Gets the current time, in seconds.
   *
   * @return The current time, in seconds.
   */
  inline virtual double getCurrentTime();

};

}

#endif
