#!/bin/bash

ROOT=`pwd`

TRAIN_TOK=${ROOT}/data/train/train.tok
TRAIN_DEP=${ROOT}/data/train/train.dep.unlabel
TEST_TOK=${ROOT}/data/input/testi.tok
TEST_DEP=${ROOT}/data/test/testr.dep.unlabel
DEV_TOK=${ROOT}/data/dev/devi.tok
DEV_DEP=${ROOT}/data/dev/devr.dep.unlabel

SIG=`date '+%Y-%m-%d-%H%M%S'`-unlabeled

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
    ${EXE} learn -m ${MODEL_PREFIX} \
        -t full \
        -i ${TRAIN_DEP}

    tar zcvf ${MODEL_PREFIX}.weight.${i}.tgz ${MODEL_PREFIX}.weight
    tar zcvf ${MODEL_PREFIX}.word.${i}.tgz ${MODEL_PREFIX}.word

    if [ $i -ge 1 ]; then
        ${EXE} test -m ${MODEL_PREFIX} \
            -t full \
            -i ${DEV_DEP} \
            -o ${OUTPUT_DIR}/devo.dep.${i}

        ${EXE} test -m ${MODEL_PREFIX} \
            -t full \
            -i ${TEST_DEP} \
            -o ${OUTPUT_DIR}/testo.dep.${i}

        ./eval/getSentFromDeptree.py ${OUTPUT_DIR}/testo.dep.${i} > ${OUTPUT_DIR}/testo.tok.${i}
        ./eval/getSentFromDeptree.py ${OUTPUT_DIR}/devo.dep.${i} > ${OUTPUT_DIR}/devo.tok.${i}

        cd eval
        echo "test `./bleu-eval.sh ${OUTPUT_DIR}/testo.tok.${i} ${ROOT}/data/test/testr.tok`"
        echo "dev  `./bleu-eval.sh ${OUTPUT_DIR}/devo.tok.${i} ${ROOT}/data/dev/devr.tok`"
        cd ..
    fi
done
