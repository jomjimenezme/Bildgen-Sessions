#!/bin/bash
for i in ../modelle/*.in
do
    echo $i
    b=`basename $i .in`
    proj3 < $i
    cp -f proj3.png proj3_$b.png

    shopt -s nullglob
    for j in ../modelle/$b*.info
    do
        d=`basename $j .info`
        echo $j
        cat $i $j | proj3
        cp -f proj3.png proj3_$d.png
    done
done
