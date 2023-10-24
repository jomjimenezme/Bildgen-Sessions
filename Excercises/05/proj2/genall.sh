#!/bin/bash
for i in ../modelle/*.in
do
    b=`basename $i .in`
    shopt -s nullglob
    for j in ../modelle/$b*.info
    do
        d=`basename $j .info`
        echo $j
        cat $i $j | ./proj2
        cp -f proj2.png proj2_$d.png
    done
done
