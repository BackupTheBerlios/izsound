#!/usr/bin/env python

# IzSound - Copyright (c) 2003, 2004 Julien PONGE - All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of the author nor the names of its contributors
#    may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND ITS CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os
import os.path

from optparse import OptionParser

import sys

# Used to map the interfaces to the output files
IFACES = [('izsound.i', 'izsound_wrap.cpp')]

# The Swig command line template
swig_cmd = 'swig -Wall -python -c++ -Isrc/ -o %s '

# Invokes Swig on a list of interfaces mappings
def invoke_swig(mappings, directory):
  for mapping in mappings:
    interface = os.path.join(directory, mapping[0])
    generated = os.path.join(directory, mapping[1])
    cmd = swig_cmd % (generated, interface)
    print cmd
    success = os.system(cmd);
    if (success != 0):
      print '[failed]'
      sys.exit(-1)

# Main
parser = OptionParser()
parser.add_option('--no-ao_dsp', action='store_true', dest='no_ao_dsp',
                  help='do not build the LibaoOutput wrapper')
parser.add_option('--no-oss_dsp', action='store_true', dest='no_oss_dsp',
                  help='do not build the OssOutput wrapper')
parser.add_option('--no-mad_dsp', action='store_true', dest='no_mad_dsp',
                  help='do not build the MadDecoder wrapper')

(options, args) = parser.parse_args()
if options.no_ao_dsp is True:
  swig_cmd = swig_cmd + '-DNO_AO_DSP '
if options.no_oss_dsp is True:
  swig_cmd = swig_cmd + '-DNO_OSS_DSP '
if options.no_mad_dsp is True:
  swig_cmd = swig_cmd + '-DNO_MAD_DSP '
swig_cmd = swig_cmd + '%s'

invoke_swig(IFACES, 'src/')
