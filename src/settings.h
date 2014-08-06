#ifndef __SR_SETTINGS_H__
#define __SR_SETTINGS_H__

#include <iostream>
#include <vector>
#include <string>

#include "engine.h"

namespace ZGen {

namespace ShiftReduce {

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

  void clear() {
    forms.clear();
    postags.clear();
    heads.clear();
    deprels.clear();
  }

  void push_back(const word_t & form,
      const postag_t & postag,
      int head,
      const deprel_t & deprel) {
    forms.push_back(form);
    postags.push_back(postag);
    heads.push_back(head);
    deprels.push_back(deprel);
  }

  friend std::ostream & operator << (std::ostream & os, const DependencyParse & d) {
    int N = d.forms.size();

    for (int i = 0; i < N; ++ i) {
      os << WordEngine::get_const_instance().decode(d.forms[i]) << "\t";
      if (d.postags.size() == N) {
        const char * tag = PoSTagEngine::get_const_instance().decode(d.postags[i]);
        os << (tag ? tag : "_");
      } else {
        os << "_";
      }
      os << "\t";
      if (d.heads.size() == N) {
        os << d.heads[i];
      } else {
        os << "_";
      }
      os << "\t";
      if (d.deprels.size() == N) {
        const char * deprel = DeprelEngine::get_const_instance().decode(d.deprels[i]);
        os << (deprel ? deprel : "NONE");
      } else {
        os << "NONE";
      }
      os << std::endl;
    }

    return os;
  }
};


typedef DependencyParse   dependency_t;
typedef DependencyParse   unordered_dependency_t;

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
