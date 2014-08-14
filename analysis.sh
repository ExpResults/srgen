#!/bin/bash

nr1=$(egrep 'BLEU' $1 | egrep 'dev' | wc -l)
nr2=$(egrep 'BLEU' $1 | egrep 'test' | wc -l)

if [ $nr1 != $nr2 ]; then
    echo "ERROR: number of dev and test not equal."
    exit 1
fi

echo "total iteration: $(egrep 'BLEU' $1 | egrep 'dev' | wc -l)"

rank=$(egrep 'BLEU' $1 | egrep 'dev' | cat -n | awk 'BEGIN{a=0;b=0}{if($6>a){a=$6;b=$0}}END{print b}' | awk '{print $1}')

#echo ${rank}
echo "dev result: $(egrep 'BLEU' $1| egrep 'dev' | cat -n | head -${rank} | tail -1 | awk '{print $1" BLEU="$6}')"

echo "best record: $(egrep 'BLEU' $1 | egrep 'test' | cat -n | head -${rank} | tail -1 | awk '{print $1" BLEU="$6}')"
