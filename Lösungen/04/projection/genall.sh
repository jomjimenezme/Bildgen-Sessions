#!/bin/bash
for i in ../modelle/*.in
do
    echo $i
    b=`basename $i .in`
    ./proj1 < $i
    cp -f proj1.png proj1_$b.png
done
