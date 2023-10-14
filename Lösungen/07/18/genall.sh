#!/bin/bash
for i in ../modelle/*.in
do
    echo $i
    b=`basename $i .in`
    ./proj4 < $i
    cp -f proj4.png proj4_$b.png

    shopt -s nullglob
    for j in ../modelle/$b*.info
    do
        d=`basename $j .info`
        echo $j
        cat $i $j | proj4
        cp -f proj4.png proj4_$d.png
    done
done
