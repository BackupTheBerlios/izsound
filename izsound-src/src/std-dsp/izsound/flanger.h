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

#ifndef IZSOUND_FLANGER_H
#define IZSOUND_FLANGER_H

#include <iterator>
#include <algorithm>

#include <pitch.h>
#include <dspunit.h>
#include <crossfader.h>
#include <fragmenter.h>
#include <demultiplexer.h>

namespace izsound
{

/**
 * A DSP unit that applies a flanger effect on a sound stream.
 * This unit has one input slot and one output slot.
 *
 * The parameters valid ranges are:
 * <ul>
 *  <li>frequency: [0 ; +infinity] in Hz</li>
 *  <li>amplitude: [0 ; 1]</li>
 *  <li>wet: [0 ; 1]</li>
 * </ul>
 *
 * @author Julien PONGE <julien@izforge.com>
 */
class Flanger : public DspUnit
{

private:

  /** The flanger frequency. */
  double m_frequency;

  /** The flanger effect amplitude. */
  double m_amplitude;

  /** The flanger wet ratio. */
  double m_wet;

  /** The demultiplexer. */
  DeMultiplexer* m_demux;

  /** The pitch shifter. */
  Pitch* m_pitch;

  /** The crossfader. */
  CrossFader* m_xfader;

  /** The fragmenter. */
  Fragmenter* m_fragmenter;

  /** The internal chain output. */
  SlotData m_internalOutput;

protected:

  /**
   * Processes the sound and applies the flanger effect.
   */
  virtual void performDsp();

public:

  /**
   * The constructor.
   *
   * @param frequency The flanger frequency.
   * @param amplitude The effect amplitude.
   * @param wet The wet/dry ratio.
   * @param sampleRate The audio chain sample rate.
   */
  Flanger(const double &frequency = 0.5,
          const double &amplitude = 0.0001,
          const double &wet = 0.2,
          const unsigned int &sampleRate = 44100);

  /**
   * The destructor.
   */
  virtual ~Flanger();

  /**
   * Gets the flanger frequency.
   *
   * @return The flanger frequency.
   */
  inline double getFrequency() const { return m_frequency; }

  /**
   * Gets the flanger amplitude.
   *
   * @return The flanger amplitude.
   */
  inline double getAmplitude() const { return m_amplitude; }

  /**
   * Gets the flanger wet/dry ratio.
   *
   * @return The flanger wet/dry ratio.
   */
  inline double getWet() const { return m_wet; }

  /**
   * Sets the flanger frequency.
   *
   * @param frequency The new frequency.
   */
  void setFrequency(const double &frequency);

  /**
   * Sets the flanger amplitude.
   *
   * @param amplitude The new amplitude.
   */
  void setAmplitude(const double &amplitude);

  /**
   * Sets the flanger wet/dry ratio.
   *
   * @param wet The new wet/dry ratio.
   */
  void setWet(const double &wet);

};

/**
 * A discretised sinus table. 100 values cover the [-1 ; 1] interval.
 */
static const double DiscreteSinus[] = {
   -0.841470984808 ,
   -0.830497370492 ,
   -0.819191568301 ,
   -0.807558100405 ,
   -0.795601620036 ,
   -0.783326909627 ,
   -0.770738878899 ,
   -0.757842562895 ,
   -0.744643119971 ,
   -0.731145829727 ,
   -0.7173560909 ,
   -0.7032794192 ,
   -0.688921445111 ,
   -0.674287911628 ,
   -0.659384671971 ,
   -0.644217687238 ,
   -0.628793024018 ,
   -0.613116851973 ,
   -0.597195441362 ,
   -0.581035160537 ,
   -0.564642473395 ,
   -0.548023936792 ,
   -0.531186197921 ,
   -0.514135991653 ,
   -0.496880137844 ,
   -0.479425538604 ,
   -0.461779175541 ,
   -0.443948106966 ,
   -0.425939465066 ,
   -0.40776045306 ,
   -0.389418342309 ,
   -0.370920469413 ,
   -0.352274233275 ,
   -0.333487092141 ,
   -0.314566560616 ,
   -0.295520206661 ,
   -0.276355648564 ,
   -0.257080551892 ,
   -0.237702626427 ,
   -0.218229623081 ,
   -0.198669330795 ,
   -0.179029573426 ,
   -0.159318206614 ,
   -0.139543114644 ,
   -0.119712207289 ,
   -0.0998334166468 ,
   -0.0799146939692 ,
   -0.0599640064794 ,
   -0.0399893341866 ,
   -0.0199986666933 ,
   0.0 ,
   0.0199986666933 ,
   0.0399893341866 ,
   0.0599640064794 ,
   0.0799146939692 ,
   0.0998334166468 ,
   0.119712207289 ,
   0.139543114644 ,
   0.159318206614 ,
   0.179029573426 ,
   0.198669330795 ,
   0.218229623081 ,
   0.237702626427 ,
   0.257080551892 ,
   0.276355648564 ,
   0.295520206661 ,
   0.314566560616 ,
   0.333487092141 ,
   0.352274233275 ,
   0.370920469413 ,
   0.389418342309 ,
   0.40776045306 ,
   0.425939465066 ,
   0.443948106966 ,
   0.461779175541 ,
   0.479425538604 ,
   0.496880137844 ,
   0.514135991653 ,
   0.531186197921 ,
   0.548023936792 ,
   0.564642473395 ,
   0.581035160537 ,
   0.597195441362 ,
   0.613116851973 ,
   0.628793024018 ,
   0.644217687238 ,
   0.659384671971 ,
   0.674287911628 ,
   0.688921445111 ,
   0.7032794192 ,
   0.7173560909 ,
   0.731145829727 ,
   0.744643119971 ,
   0.757842562895 ,
   0.770738878899 ,
   0.783326909627 ,
   0.795601620036 ,
   0.807558100405 ,
   0.819191568301 ,
   0.830497370492 ,
   0.841470984808
};
#define N_DISCRETE_VALUES 100

}

#endif
