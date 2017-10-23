#!/bin/sh

make approx

#./reduce <n> <nbthreads>
for n in 10 10000;
do
    for nbthreads in 4;
    do
                TEST1=$(./reduce $n $nbthreads 2> /dev/null)
                if ./approx 0 "$TEST1";
                then
                    echo oktest "./reduce $n $nbthreads"
                else
                    echo notok "./reduce $n $nbthreads" 
                    exit 1
                fi
    done
done
	       
