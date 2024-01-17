#!/bin/bash
for i in material/*.in
do
    echo $i
    b=`basename $i .in`
    ./polygon < $i
    cp -f polygon.png polygon_$b.png
done
