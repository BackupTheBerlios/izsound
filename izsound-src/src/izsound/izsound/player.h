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

#ifndef IZSOUND_PLAYER_H
#define IZSOUND_PLAYER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

namespace izsound
{

/**
 * Defines an interface for a player. A player supports various kinds of
 * operations like:
 * <ul>
 *   <li>play / stop / pause</li>
 *   <li>seeking</li>
 *   <li>...</li>
 * </ul>
 *
 * @author Julien PONGE <julien@izforge.com>
 */
class Player
{

public:

  /**
   * Play operation. The expected behaviour is to start playing from the
   * current position.
   */
  virtual void play() = 0;

  /**
   * Pause operation.
   */
  virtual void pause() = 0;

  /**
   * Stop operation. The expected behaviour is to go back to the beginning of
   * the source.
   */
  virtual void stop() = 0;

  /**
   * Opens a file from a filename.
   *
   * @param filename The filename.
   * @param success Indicates wether the operation succeeded or not.
   */
  virtual void open(const char* filename, bool &success) = 0;

  /**
   * Indicates if we have reached the end of the stream or not.
   *
   * @return <code>true</code> if we have reached the end, <code>false</code>
   *         otherwise.
   */
  virtual bool isEndReached() = 0;

  /**
   * Returns the total stream playing time, in seconds.
   *
   * @return The total playing time.
   */
  virtual double getTotalTime() = 0;

  /**
   * Seeks to a position.
   *
   * @param pos The position in seconds.
   */
  virtual void seek(const double &pos) = 0;

  /**
   * Gets the current time, in seconds.
   *
   * @return The current time, in seconds.
   */
  virtual double getCurrentTime() = 0;

};

}

#endif
