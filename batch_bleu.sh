#!/bin/bash
ROOT=`pwd`
OUTPUT_DIR=${ROOT}/$1
N=`ls ${OUTPUT_DIR}/devo.tok.* | wc -l`

for i in `seq 1 ${N}`; do
    cd eval
    echo "test `./bleu-eval.sh ${OUTPUT_DIR}/testo.tok.${i} ${ROOT}/data/test/testr.tok`"
    echo "dev  `./bleu-eval.sh ${OUTPUT_DIR}/devo.tok.${i} ${ROOT}/data/dev/devr.tok`"
    cd ..
done
