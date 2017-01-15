#!/bin/bash
echo "enter the numbers"
read n1
read n2
echo "n1+n2= `expr $n1 + $n2`"
echo "n1*n2= `expr $n1 \* $n2`"
echo "n1/n2= `expr $n1 \/ $n2`"
echo $$
echo $#
