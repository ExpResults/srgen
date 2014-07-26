#ifndef __SR_SETTINGS_H__
#define __SR_SETTINGS_H__

#include <iostream>
#include <vector>
#include <string>

#include "engine.h"

namespace SR {

// Type define
typedef int                                       word_t;
typedef int                                       postag_t;
typedef int                                       deprel_t;
typedef double                                    floatval_t;
typedef std::vector<word_t>                       sentence_t;
typedef std::vector<std::pair<word_t, postag_t> > sentence_with_postag_t;

// The struct for dependency parse
struct DependencyParse {
  std::vector<word_t> forms;
  std::vector<postag_t> postags;
  std::vector<int> heads;
  std::vector<deprel_t> deprels;

  void push_back(const word_t & form,
      const postag_t & postag,
      int head,
      const deprel_t & deprel) {
    forms.push_back(form);
    postags.push_back(postag);
    heads.push_back(head);
    deprels.push_back(deprel);
  }
};


typedef DependencyParse   dependency_t;
typedef DependencyParse   unordered_dependency_t;

struct Parameter {
  Parameter()  : w(0), w_sum(0), w_time(0) {}

  floatval_t  w;
  floatval_t  w_sum;
  int         w_time;
};

typedef Parameter param_t;

const int kMaxNumberOfWords = 128;

const int kMaxLengthOfBuffer = 1024;

const int kStartIndexOfValidPoSTag = 3;

const int kNumberOfPoSTags = PoSTagEncoderAndDecoder::kMaxIndexOfPoSTag;

const int kStartIndexOfValidDependencyRelation = 1;

const int kNumberOfDependencyRelations = DeprelsEncoderAndDecoder::kMaxIndexOfDependencyRelation;
// with -none-

const int kMaxBeamSize = 128;

const int kMaxSteps = kMaxNumberOfWords * 2;
}

#endif  // end for __SR_SETTINGS_H__
