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

#ifndef IZSOUND_VOLUME_H
#define IZSOUND_VOLUME_H

#include <iterator>
#include <algorithm>

#include <dspunit.h>

namespace izsound
{

/**
 * A simple unit that changes the sound volume against a factor.
 * It has one input slot and one output slot.
 *
 * @author Julien PONGE <julien@izforge.com>
 */
class Volume : public DspUnit
{

private:

  /** The volume change factor. */
  double m_factor;

protected:

  /**
   * Changes the volume.
   */
  virtual void performDsp();

public:

  /**
   * The constructor.
   *
   * @param factor The factor used to modify the volume level.
   * @param sampleRate The audio chain sample rate.
   */
  Volume(const double &factor = 1.0, const unsigned int &sampleRate = 44100);

  /**
   * The destructor.
   */
  virtual ~Volume();

  /**
   * Sets the new volume level factor.
   *
   * @param factor The new factor.
   */
  inline void setFactor(const double &factor) { m_factor = factor; }

  /**
   * Gets the current volume factor.
   *
   * @return The volume factor.
   */
  inline double getFactor() const { return m_factor; }

};

/**
 * Unary function that multiplies by a factor.
 */
struct Multiply_OF
{
  double factor;

  Multiply_OF(const double &f) { factor = f; }

  double operator() (const double &val)
  {
    return val * factor;
  }
};

}

#endif
