/*
 * IzSound - Copyright (c) 2003, 2004 Julien PONGE - All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

%module izsound

%{
#include <izsound/player.h>
#include <izsound/dspunit.h>
%}

/*
 * Typemap issues resolving.
 */
%include "typemaps.i"
//%apply(bool &INPUT) { bool &success };

/*
 * We put all the IzSound classes in the same module.
 */

namespace izsound
{

/*
 * IzSoundException
 */
class IzSoundException
{

public:

  IzSoundException(const std::string &what) throw();

  virtual const char* what() const throw();

};

/*
 * DspUnit
 */
class DspUnit
{

public:

  virtual ~DspUnit();

  unsigned int getSampleRate() const;

  unsigned int getMaxInSlots() const;

  unsigned int getMaxOutSlots() const;

  void connect(DspUnit* dspUnit, const unsigned int &mySlot,
               const unsigned int &itsSlot);

  void disconnect(const unsigned int &mySlot);

  void run();

  virtual void reset();

protected:

  DspUnit(const unsigned int &sampleRate = 44100,
          const unsigned int &maxIn = 1, const unsigned int &maxOut = 1);

};

/*
 * Player
 */
class Player
{

public:

  virtual void play() = 0;

  virtual void pause() = 0;

  virtual void stop() = 0;

  virtual void open(const char* filename) throw(IzSoundException) = 0;

  virtual bool isEndReached() = 0;

  virtual double getTotalTime() = 0;

  virtual void seek(const double &pos) = 0;

  virtual double getCurrentTime() = 0;

};

}

/*
 * Other units inclusion.
 */
%include "bandfilter.i"
%include "blackhole.i"
%include "crossfader.i"
%include "datapicker.i"
%include "delayextrastereo.i"
%include "demultiplexer.i"
%include "flanger.i"
%include "fragmenter.i"
%include "oggfiledecoder.i"
%include "pitch.i"
%include "silencer.i"
%include "volume.i"
%include "whitenoise.i"

/*
 * Conditional DSP units.
 */

#ifndef NO_AO_DSP
%include "libaooutput.i"
#endif

#ifndef NO_MAD_DSP
%include "maddecoder.i"
#endif

#ifndef NO_OSS_DSP
%include "ossoutput.i"
#endif
