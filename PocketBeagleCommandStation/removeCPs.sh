#!/bin/bash
out=`echo $1|sed 's/\.svg/-nocps.svg/g'`
grep -v 'M0,17.5 L0,-17.5 M17.5,0 L-17.5,0' $1 >$out
