#!/bin/bash
for i in material/*.in
do
    echo $i
    b=`basename $i .in`
    ./rotk < $i
    cp -f rotk.png rotk_$b.png
done
