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

#include <flanger.h>

/*
 * The internal states:
 * - INIT_STATE: the flanger is initialising and pitches up on half a period
 * - FAST_STATE: the flanger is pitching up on a full period
 * - SLOT_STATE: the flanger is pitching down on a full period
 * - HOME_STATE: the flanger is bringing the phase to 0.
 */
#define INIT_STATE 0
#define FAST_STATE 1
#define SLOW_STATE 2
#define HOME_STATE 3

using namespace std;
using namespace izsound;

Flanger::Flanger(const double &frequency,
                 const double &amplitude,
                 const double &wet,
                 const unsigned int &sampleRate)
   : DspUnit(sampleRate, 1, 1)
{
  // Init
  m_originalBuffer.resize(2);
  m_pitchedBuffer.resize(2);
  setFrequency(frequency);
  setAmplitude(amplitude);
  setWet(wet);
  m_internalState = INIT_STATE;
}

Flanger::~Flanger()
{

}

void Flanger::performDsp()
{ 
  // Init
  unsigned int i;
  unsigned int j;
  unsigned int k;
  static unsigned int samplesCounter = 0;
  SlotData* input  = m_inSlots[0];
  SlotData* output = m_outSlots[0];
  (*output)[0].clear();
  (*output)[1].clear();
  back_insert_iterator<deque<double> > biLeft1(m_originalBuffer[0]);
  back_insert_iterator<deque<double> > biRight1(m_originalBuffer[1]);
  back_insert_iterator<deque<double> > biLeft2(m_pitchedBuffer[0]);
  back_insert_iterator<deque<double> > biRight2(m_pitchedBuffer[1]);
  
  // We copy to the internal buffers
  copy((*input)[0].begin(), (*input)[0].end(), biLeft1);
  copy((*input)[1].begin(), (*input)[1].end(), biRight1);
  copy((*input)[0].begin(), (*input)[0].end(), biLeft2);
  copy((*input)[1].begin(), (*input)[1].end(), biRight2);
  
  // We process the data sample per sample
  double ltemp;
  double rtemp;
  unsigned int osize = m_originalBuffer[0].size();
  unsigned int psize = m_pitchedBuffer[0].size();
  for (i = 0, j = 0; (i < osize) && (j < psize); ++i, ++j)
  {
    switch (m_internalState)
    {
    case INIT_STATE:
      // We must pitch up during half a period
      if ((++samplesCounter % m_pitchTrigger) == 0)
      {
        if (j != psize - 1)
        {
          ++j;
        }
      }
      if ((samplesCounter % m_halfPeriodSamplesCount) == 0)
      {
        samplesCounter  = 0;
        m_internalState = SLOW_STATE;
      }
      break;

    case FAST_STATE:
      // We must pitch up during a full period
      if ((++samplesCounter % m_pitchTrigger) == 0)
      {
        if (j !=psize - 1)
        {
          ++j;
        }
      }
      if ((samplesCounter % m_periodSamplesCount) == 0)
      {
        samplesCounter  = 0;
        m_internalState = SLOW_STATE;
      }
      break;

    case SLOW_STATE:
      // We must slow down during a full period
      if ((++samplesCounter % m_pitchTrigger) == 0)
      {
        if (j != 0)
        {
          --j;
        }
      }
      if ((samplesCounter % m_periodSamplesCount) == 0)
      {
        samplesCounter  = 0;
        m_internalState = FAST_STATE;
      }
      break;

    case HOME_STATE:
      break;

    default: // Just to please the compilers :-)
      continue;
    }
    ltemp = (m_originalBuffer[0][i] * m_dry) + (m_pitchedBuffer[0][j] * m_wet);
    rtemp = (m_originalBuffer[1][i] * m_dry) + (m_pitchedBuffer[1][j] * m_wet);
    (*output)[0].push_back(ltemp);
    (*output)[1].push_back(rtemp);
  }

  // Cleanups
  for (k = 0; k <= i; ++k)
  {
    m_originalBuffer[0].pop_front();
    m_originalBuffer[1].pop_front();
  }
  for (k = 0; k <= j; ++k)
  {
    m_pitchedBuffer[0].pop_front();
    m_pitchedBuffer[1].pop_front();
  }
}

void Flanger::setFrequency(const double &frequency)
{
  m_frequency = frequency;
  m_periodSamplesCount = (unsigned int)((double)m_sampleRate / m_frequency);
  m_halfPeriodSamplesCount = m_periodSamplesCount / 2;
}

void Flanger::setAmplitude(const double &amplitude)
{
  m_amplitude = amplitude;
  double rate = (double)m_sampleRate;
  double bs = rate / ((rate * m_amplitude) - m_amplitude);
  m_pitchTrigger = (unsigned int)bs;
}

void Flanger::setWet(const double &wet)
{
  m_wet = wet;
  m_dry = 1.0 - m_wet;
}
