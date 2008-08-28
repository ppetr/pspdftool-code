#!/bin/sh
autoheader
aclocal
automake --add-missing --copy
autoconf
rm -f src/config.h
