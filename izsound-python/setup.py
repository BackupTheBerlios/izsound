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


from distutils.core import setup, Extension

# Generalities
metadata = {
  'name'             : 'izsound-python',
  'version'          : '0.3.0',
  'author'           : 'Julien PONGE',
  'author_email'     : 'julien@izforge.com',
  'url'              : 'http://www.izforge.com/izsound/',
  'description'      : 'Simple wrapping of the IzSound APIs.',
  'long_description' : 'Simple wrapping of the IzSound APIs.',
  'license'          : 'BSD-style'
}

# We describe the module
mods = ['izsound']

# We describe the extension module
mod  = '_izsound'
src  = ['src/izsound_wrap.cpp']
libs = ['izsound', 'stdizdsp']
exts = [Extension(mod, src, libraries = libs)]

# Let's go
setup(name         = metadata['name'],
      version      = metadata['version'],
      author       = metadata['author'],
      author_email = metadata['author_email'],
      url          = metadata['url'],
      description  = metadata['description'],
      license      = metadata['license'],
      package_dir  = {'' : 'src'},
      py_modules   = mods,
      ext_modules  = exts)
