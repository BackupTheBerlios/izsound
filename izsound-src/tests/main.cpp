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

/*
 * This program is a collection of various tests I have written. Using a unit
 * testing framework in the spirit of the JUnit would have not been very
 * useful as testing that sound operations work is not that 'binary' :-) It
 * would have been good to check the chaining system for instance, but making
 * small DSP units constructions works quite well. It is also quite a good
 * idea to run the tests in conjunction with Valgrind to check that we
 * are not making memory faults.
 *
 * I repeat: these are my tests, made in my development context and they test
 * what I want to test in the way I want. They are _not_ strong tests, but it's
 * like that, and that's the way it is :-)
 */

#include <iostream>

#include <string.h>

#include <pitch.h>
#include <volume.h>
#include <dspunit.h>
#include <flanger.h>
#include <silencer.h>
#include <ossoutput.h>
#include <blackhole.h>
#include <crossfader.h>
#include <libaooutput.h>
#include <demultiplexer.h>
#include <oggfiledecoder.h>
#include <delayextrastereo.h>

using namespace std;
using namespace izsound;

/* Functions declarations. */
void chainwork();
void silence_oss();
void oggplay();
void pitchplay();
void crossfade();
void volume();
void aoplayoss();
void oggseek();
void aowritefile();
void delayextrastereo();
void demultiplexer();
void flanger();

/**
 * The program entry-point.
 *
 * @param argc The number of arguments.
 * @param argv The arguments values.
 */
int main(int argc, char** argv)
{
  // Loose arguments handling
  if (argc < 2)
  {
    cout << "Usage: test [test1 name] [test2 name] ..." << endl << endl;
    cout << "Available tests:" << endl
         << "- chainwork   : Silencer + Blackhole" << endl
         << "- silence_oss : Silencer + OssOutput" << endl
         << "- oggplay     : OggFileDecoder + OssOutput"
            "\n  (needs 2 files track.ogg and track2.ogg)" << endl
         << "- pitchplay   : OggFileDecoder + Pitch + OssOutput"
            "\n  (needs a file named track.ogg)" << endl
         << "- crossfade   : OggFileDecoder(2) + CrossFader + OssOutput"
            "\n  (needs 2 files track.ogg and track2.ogg)" << endl
         << "- volume      : OggFileDecoder + Volume + OssOutput"
            "\n  (needs a file named track.ogg)" << endl
         << "- aoplayoss   : OggFileDecoder + LibaoOutput"
            "\n  (needs a file named track.ogg)" << endl
         << "- oggseek     : OggFileDecoder + OssOutput"
         << "\n  (needs a file named track.ogg)" << endl
         << "- aowritefile : OggFileDecoder + LibaoOutput"
            "\n  (needs a file named track.ogg)" << endl
         << "- delayextrastereo : OggFileDecoder + DelayExtraStereo "
            "+ LibaoOutput"
            "\n  (needs a file named track.ogg)" << endl
         << "- demultiplexer : OggFileDecoder + DeMultiplexer + LibaoOutput * 2"
            "\n  (needs a file named track.ogg)" << endl
         << "- flanger: OggFileDecoder + Flanger + LibaoOutput"
            "\n  (needs a file named track.ogg)" << endl
         << endl;
      exit(0);
  }
  for (int i = 1; i < argc; ++i)
  {
    // Yes, it looks like bad old C, a map to functions would be cleaner,
    // but ... it's like that and that's the wat it is.
    if (strcmp(argv[i], "chainwork") == 0)
      chainwork();
    else if (strcmp(argv[i], "silence_oss") == 0)
      silence_oss();
    else if (strcmp(argv[i], "oggplay") == 0)
      oggplay();
    else if (strcmp(argv[i], "pitchplay") == 0)
      pitchplay();
    else if (strcmp(argv[i], "crossfade") == 0)
      crossfade();
    else if (strcmp(argv[i], "volume") == 0)
      volume();
    else if (strcmp(argv[i], "aoplayoss") == 0)
      aoplayoss();
    else if (strcmp(argv[i], "oggseek") == 0)
      oggseek();
    else if (strcmp(argv[i], "aowritefile") == 0)
      aowritefile();
    else if (strcmp(argv[i], "delayextrastereo") == 0)
      delayextrastereo();
    else if (strcmp(argv[i], "demultiplexer") == 0)
      demultiplexer();
    else if (strcmp(argv[i], "flanger") == 0)
      flanger();
  }

  return 0;
}

/**
 * Plugs Silencer with Blackhole. This allows to test that the chaining
 * of the elements (basically !) works.
 */
void chainwork()
{
  // Init
  cout << endl << "[ chainwork ]" << endl << endl;
  Silencer silencer;
  Blackhole blackhole(true);
  
  // Connection
  silencer.connect(&blackhole, 0, 0);

  // Let's run
  for (int i = 0; i < 5; ++i) silencer.run();
  silencer.setDataLength(1024);
  for (int i = 0; i < 5; ++i) silencer.run();


  // Deconnection
  silencer.disconnect(0);
}

/**
 * Produces some silence to the OSS output unit.
 */
void silence_oss()
{
  // Init
  cout << endl << "[ oss_output ]" << endl << endl;
  bool success;
  Silencer silencer;
  OssOutput oss(success);
  if (!success)
  {
    cout << "OSS output initialisation failed." << endl;
    return;
  }

  // Connection
  silencer.connect(&oss, 0, 0);

  // Let's run
  for (int i = 0; i < 30; ++i) silencer.run();

  // Cleanups
  oss.flush();
  silencer.disconnect(0);
}

/**
 * Plays an ogg file to the OSS output unit.
 */
void oggplay()
{
  // Init
  cout << endl << "[ oggplay ]" << endl << endl;
  bool success;
  OssOutput oss(success);
  if (!success)
  {
    cout << "OSS output initialisation failed." << endl;
    return;
  }
  OggFileDecoder decoder;

  // Connection
  decoder.connect(&oss, 0, 0);

  // Let's run
  decoder.play();
  decoder.open("track.ogg", success);
  decoder.play();
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  for (int i = 0; i < 2000; ++i) decoder.run();
  decoder.open("track2.ogg", success);
  cout << "Total time on the track: " << decoder.getTotalTime()
       << " (s)" << endl;
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  while (!decoder.isEndReached())
  {
    cout << "\rTime: " << decoder.getCurrentTime() << " (s)"
         << "          ";
    decoder.run();
  }
  cout << endl;

  // Cleanups
  oss.flush();
  decoder.disconnect(0);
}

/**
 * Plays a file to the OSS output unit and plays with its pitch.
 */
void pitchplay()
{
  // Init
  cout << endl << "[ pitchplay ]" << endl << endl;
  bool success;
  OssOutput oss(success);
  if (!success)
  {
    cout << "OSS output initialisation failed." << endl;
    return;
  }
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  double ratio = 0.8;
  double tab[] = { 0.8, 0.85, 0.90, 0.95, 1.0, 1.05, 1.1, 1.15, 1.20,
                   1.15, 1.1, 1.05, 1.0, 0.95, 0.90, 0.85, 0.8 };
  Pitch pitch(ratio);

  // Connection
  decoder.connect(&pitch, 0, 0);
  pitch.connect(&oss, 0, 0);

  // Let's roll baby !
  int cpt1 = 1, cpt2 = 0;
  while (!decoder.isEndReached())
  {
    decoder.run();
    if (cpt1++ % 100 == 0)
    {
      ratio = tab[++cpt2 % 17];
      pitch.setRatio(ratio);
    }
  }

  // Cleanups
  oss.flush();
  decoder.disconnect(0);
}

/**
 * Performs some crossfading between 2 streams.
 */
void crossfade()
{
  // Init
  cout << endl << "[ crossfade ]" << endl << endl;
  bool success;
  OssOutput oss(success);
  if (!success)
  {
    cout << "OSS output initialisation failed." << endl;
    return;
  }
  OggFileDecoder decoder1("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  OggFileDecoder decoder2("track2.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  CrossFader crossfader;

  // Connections
  decoder1.connect(&crossfader, 0, 0);
  decoder2.connect(&crossfader, 0, 1);
  crossfader.connect(&oss, 0, 0);

  // Let's play
  double pos_tab[] = { -1.0, -0.75, -0.5, -0.25, 0.0, 0.25, 0.5, 0.75, 1.0,
                       0.75, 0.5, 0.25, 0.0, -0.25, -0.5, -0.75 };
  int cpt1 = 0;
  int cpt2 = 0;
  while ( !(decoder1.isEndReached() && decoder2.isEndReached()) )
  {
    decoder1.run();
    decoder2.run();
    if (cpt1++ % 250 == 0)
    {
      crossfader.setPosition(pos_tab[cpt2++ % 16]);
    }
  }

  // Cleanups
  oss.flush();
}

/**
 * Tests that we can change the volume on a stream.
 */
void volume()
{
  // Init
  cout << endl << "[ volume ]" << endl << endl;
  bool success;
  OssOutput oss(success);
  if (!success)
  {
    cout << "OSS output initialisation failed." << endl;
    return;
  }
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  Volume volume(0.25);

  // Connection
  decoder.connect(&volume, 0, 0);
  volume.connect(&oss, 0, 0);

  // Let's roll baby !
  while (!decoder.isEndReached())
  {
    decoder.run();
  }

  // Cleanups
  oss.flush();
}

/**
 * LibaoOutput DSP test with the OSS driver.
 */
void aoplayoss()
{
  // Init
  cout << endl << "[ aoplayoss ]" << endl << endl;
  bool success;
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  LibaoOutput ao("oss", 0, success);
  if (!success)
  {
    cout << "Could not initialise OSS/LibAO." << endl;
    return;
  }

  // Connection
  decoder.connect(&ao, 0, 0);

  // Let's roll baby !
  while (!decoder.isEndReached())
  {
    decoder.run();
  }

  // Cleanups
  ao.flush();
}

/**
 * Makes some looping. Might be boring for your ears too.
 */
void oggseek()
{
  // Init
  cout << endl << "[ oggseek ]" << endl << endl;
  bool success;
  OssOutput oss(success);
  if (!success)
  {
    cout << "OSS output initialisation failed." << endl;
    return;
  }
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }

  // Connection
  decoder.connect(&oss, 0, 0);

  // Let's roll baby !
  for (int i = 0; i < 5; ++i)
  {
    decoder.seek(5.0);
    for (int j = 0; j < 100; ++j) decoder.run();
  }

  // Cleanups
  oss.flush();
}

/*
 * Decodes an OGG file and writes it to a PCM WAVE file.
 */
void aowritefile()
{
  // Init
  cout << endl << "[ aofilewrite ]" << endl << endl;
  bool success;
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  LibaoOutput ao("wav", 0, "aowritefile.wav", success);
  if (!success)
  {
    cout << "Could not initialise Wav/LibAO." << endl;
    return;
  }

  // Connection
  decoder.connect(&ao, 0, 0);

  // Let's roll baby !
  while (!decoder.isEndReached())
  {
    decoder.run();
  }

  // Cleanups
  ao.flush();
}

/**
 * DelayExtraStereo test.
 */
void delayextrastereo()
{
  // Init
  cout << endl << "[ delayextrastereo ]" << endl << endl;
  bool success;
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  DelayExtraStereo extraStereo;
  LibaoOutput ao("oss", 0, success);
  if (!success)
  {
    cout << "Could not initialise OSS/LibAO." << endl;
    return;
  }

  // Connection
  decoder.connect(&extraStereo, 0, 0);
  extraStereo.connect(&ao, 0, 0);

  // Let's roll baby !
  while (!decoder.isEndReached())
  {
    decoder.run();
  }

  // Cleanups
  ao.flush();
}

/**
 * DeMultiplexer test.
 */
void demultiplexer()
{
  // Init
  cout << endl << "[ demultiplexer ]" << endl << endl;
  bool success;
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  LibaoOutput aoPlay("oss", 0, success);
  if (!success)
  {
    cout << "Could not initialise OSS/LibAO." << endl;
    return;
  }
  LibaoOutput aoWrite("wav", 0, "aowritefile.wav", success);
  if (!success)
  {
    cout << "Could not initialise Wav/LibAO." << endl;
    return;
  }
  DeMultiplexer demux(3); // One is useless, we just make sure there aren't
                          // bugs in this case.

  // Connection
  decoder.connect(&demux, 0, 0);
  demux.connect(&aoPlay, 0, 0);
  demux.connect(&aoWrite, 1, 0);

  // Let's roll baby !
  while (!decoder.isEndReached())
  {
    decoder.run();
  }

  // Cleanups
  aoPlay.flush();
  aoWrite.flush();
}

/**
 * Flanger test.
 */
void flanger()
{
  // Init
  cout << endl << "[ flanger ]" << endl << endl;
  bool success;
  OggFileDecoder decoder("track.ogg", success);
  if (!success)
  {
    cout << "OGG initialisation failed." << endl;
    return;
  }
  LibaoOutput ao("oss", 0, success);
  if (!success)
  {
    cout << "Could not initialise OSS/LibAO." << endl;
    return;
  }
  Flanger flanger;

  // Connection
  decoder.connect(&flanger, 0, 0);
  flanger.connect(&ao, 0, 0);

  // Let's roll baby !
  while (!decoder.isEndReached())
  {
    decoder.run();
  }

  // Cleanups
  ao.flush();
}
