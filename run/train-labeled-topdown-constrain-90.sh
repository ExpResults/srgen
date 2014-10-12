#!/bin/bash

ROOT=`pwd`

TRAIN_DEP=${ROOT}/data/train/train-labeled.dep
TRAIN_IN=${ROOT}/data/input/train-allpos-alldep-dir90.depl

TEST_TOK=${ROOT}/data/test/testr.tok
TEST_IN=${ROOT}/data/input/test-allpos-alldep-dir90.depl

DEV_TOK=${ROOT}/data/dev/devr.tok
DEV_IN=${ROOT}/data/input/dev-allpos-alldep-dir90.depl

SIG=`date '+%Y-%m-%d-%H%M%S'`-labeled-topdown-constrain-dir90

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
    ${EXE} learn -t full-topdown-constrain \
        -m ${MODEL_PREFIX} \
        -p ${DIC}          \
        -r ${TRAIN_DEP}    \
        -i ${TRAIN_IN}

    tar zcvf ${MODEL_PREFIX}.weight.${i}.tgz ${MODEL_PREFIX}.weight
    tar zcvf ${MODEL_PREFIX}.word.${i}.tgz ${MODEL_PREFIX}.word

    ${EXE} test -t full-topdown-constrain \
        -m ${MODEL_PREFIX}  \
        -p ${DIC}           \
        -i ${DEV_IN}        \
        -o ${OUTPUT_DIR}/devo.dep.${i}

    ${EXE} test -t full-topdown-constrain \
        -m ${MODEL_PREFIX}  \
        -p ${DIC}           \
        -i ${TEST_IN}       \
        -o ${OUTPUT_DIR}/testo.dep.${i}

    ./eval/getSentFromDeptree.py ${OUTPUT_DIR}/testo.dep.${i} > ${OUTPUT_DIR}/testo.tok.${i}
    ./eval/getSentFromDeptree.py ${OUTPUT_DIR}/devo.dep.${i} > ${OUTPUT_DIR}/devo.tok.${i}

    cd eval
    echo "test `./bleu-eval.sh ${OUTPUT_DIR}/testo.tok.${i} ${ROOT}/data/test/testr.tok`"
    echo "dev  `./bleu-eval.sh ${OUTPUT_DIR}/devo.tok.${i} ${ROOT}/data/dev/devr.tok`"
    cd ..
done
