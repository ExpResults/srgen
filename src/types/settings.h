#ifndef __SR_SETTINGS_H__
#define __SR_SETTINGS_H__

#include <iostream>
#include <vector>
#include <string>

#include "engine.h"

namespace ZGen {

namespace ShiftReduce {

typedef double floatval_t;

const int kMaxNumberOfWords = 180;

const int kMaxLengthOfBuffer = 2048;

const int kStartIndexOfValidPoSTag = PoSTagEncoderAndDecoder::DOLLAR;

const int kNumberOfPoSTags = PoSTagEncoderAndDecoder::kMaxIndexOfPoSTag;

const int kStartIndexOfValidDependencyRelation = 1;

const int kNumberOfDependencyRelations = DeprelsEncoderAndDecoder::kMaxIndexOfDependencyRelation;
// with -none-

const int kMaxBeamSize = 128;

const int kMaxSteps = kMaxNumberOfWords * 2;

const floatval_t inf = 1e20;
}

}
#endif  // end for __SR_SETTINGS_H__
