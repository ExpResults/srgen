#!/bin/bash

ROOT=`pwd`

TRAIN_TOK=${ROOT}/data/train/train.tok
TRAIN_DEP=${ROOT}/data/train/train.dep
TEST_TOK=${ROOT}/data/input/testi.tok
DEV_TOK=${ROOT}/data/dev/devi.tok

SIG=`date '+%Y-%m-%d-%H%M%S'`

DIC=${ROOT}/data/dic/dic.7

WORKSPACE=${ROOT}/workspace/${SIG}

MODEL_DIR=${WORKSPACE}/model
OUTPUT_DIR=${WORKSPACE}/output
EXE_DIR=${WORKSPACE}/bin

mkdir -p ${MODEL_DIR}
mkdir -p ${OUTPUT_DIR}
mkdir -p ${EXE_DIR}

MODEL_PREFIX=${MODEL_DIR}/model

EXE=${EXE_DIR}/srg-run.${SIG}

cp ${ROOT}/bin/srg ${EXE}

rm ${MODEL_PREFIX}.*

for i in `seq 1 50`; do
    ${EXE} learn -t none \
        -m ${MODEL_PREFIX} \
        -p ${DIC}   \
        -i ${TRAIN_TOK} 

    cp ${MODEL_PREFIX}.weight ${MODEL_PREFIX}.weight.${i}
    cp ${MODEL_PREFIX}.word   ${MODEL_PREFIX}.word.${i}

    if [ $i -ge 10 ]; then
        ${EXE} test -t none \
            -m ${MODEL_PREFIX} \
            -p ${DIC} \
            -i ${DEV_TOK} \
            -o ${OUTPUT_DIR}/devo.dep.${i}

        ${EXE} test -t none \
            -m ${MODEL_PREFIX} \
            -p ${DIC}   \
            -i ${TEST_TOK} \
            -o ${OUTPUT_DIR}/testo.dep.${i}

        ./eval/getSentFromDeptree.py ${OUTPUT_DIR}/testo.dep.${i} > ${OUTPUT_DIR}/testo.tok.${i}
        ./eval/getSentFromDeptree.py ${OUTPUT_DIR}/devo.dep.${i} > ${OUTPUT_DIR}/devo.tok.${i}

        cd eval
        echo "test `./bleu-eval.sh ${OUTPUT_DIR}/testo.tok.${i} ${ROOT}/data/test/testr.tok`"
        echo "dev `./bleu-eval.sh ${OUTPUT_DIR}/devo.tok.${i} ${ROOT}/data/dev/devr.tok`"
        cd ..
    fi
done
