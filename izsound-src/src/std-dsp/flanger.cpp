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

using namespace std;
using namespace izsound;

Flanger::Flanger(const double &frequency,
                 const double &amplitude,
                 const double &wet,
                 const unsigned int &sampleRate)
   : DspUnit(sampleRate, 1, 1)
{
  // DSP
  m_fragmenter = new Fragmenter(100, m_sampleRate);
  m_demux  = new DeMultiplexer(2, m_sampleRate);
  m_pitch  = new Pitch(1.0001, m_sampleRate);
  m_xfader = new CrossFader(0.0, m_sampleRate);

  // We plug everything together
  m_fragmenter->connect(m_demux, 0, 0);
  m_demux->connect(m_xfader, 0, 0);
  m_demux->connect(m_pitch, 1, 0);
  m_pitch->connect(m_xfader, 0, 1);
  m_fragmenter->bindInput(this, 0, 0);
  m_internalOutput.resize(2);
  m_xfader->bindOutput(&m_internalOutput, 0);

  // Initial setup
  this->setFrequency(frequency);
  this->setAmplitude(amplitude);
  this->setWet(wet);
}

Flanger::~Flanger()
{
  delete m_fragmenter;
  delete m_demux;
  delete m_pitch;
  delete m_xfader;
}

void Flanger::performDsp()
{
  // Init
  SlotData* output = m_outSlots[0];

  // We run the internal chain
  m_fragmenter->run();
  static int cpt = 0;
  cpt++;
  if (cpt % 2 == 0)
    m_pitch->setRatio(0.998);
  else
    m_pitch->setRatio(1.001);

  // We send the result to the output stream
  (*output)[0].clear();
  (*output)[1].clear();
  back_insert_iterator<deque<double> > biLeft((*output)[0]);
  back_insert_iterator<deque<double> > biRight((*output)[1]);
  copy(m_internalOutput[0].begin(), m_internalOutput[0].end(), biLeft);
  copy(m_internalOutput[1].begin(), m_internalOutput[1].end(), biRight);
}

void Flanger::setFrequency(const double &frequency)
{
  m_frequency = frequency;
}

void Flanger::setAmplitude(const double &amplitude)
{
  m_amplitude = amplitude;
}

void Flanger::setWet(const double &wet)
{
  m_wet = wet;
}
