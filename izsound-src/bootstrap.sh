#! /bin/sh
#
# Bootstrap script for the autotools.
# Julien Ponge <julien@izforge.com>

# We set some vars because of the autotools hell
export WANT_AUTOMAKE='1.7'
export WANT_AUTOCONF_2_5='1'

# Let's go
echo -e "Running aclocal ...\t"
aclocal 
echo -e "Running libtoolize ...\t"
libtoolize -f --automake --copy 
echo -e "Running autoheader ...\t"
autoheader 
echo -e "Running automake ...\t"
automake -a -c --foreign 
echo -e "Running autoconf ...\t"
autoconf 
echo -e "\nThe package is now ready to be built :-)"
