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

#include <volume.h>

using namespace std;
using namespace izsound;

Volume::Volume(const double &factor, const unsigned int &sampleRate)
  : DspUnit(sampleRate, 1, 1)
{
  m_factor = factor;
}

Volume::~Volume()
{

}

void Volume::performDsp()
{
  // Init
  SlotData* input  = m_inSlots[0];
  SlotData* output = m_outSlots[0];
  (*output)[0].clear();
  (*output)[1].clear();
  back_insert_iterator<deque<double> > biLeft((*output)[0]);
  back_insert_iterator<deque<double> > biRight((*output)[1]);

  // Let's go
  transform((*input)[0].begin(), (*input)[0].end(),
            biLeft, Multiply_OF(m_factor));
  transform((*input)[1].begin(), (*input)[1].end(),
            biRight, Multiply_OF(m_factor));
}
