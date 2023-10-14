#!/bin/bash
for i in material/*.in
do
    echo $i
    b=`basename $i .in`
    ./surfaces < $i
    cp -f surfaces.png surfaces_$b.png
done
