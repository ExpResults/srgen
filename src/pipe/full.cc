#include "pipe/full.h"
#include <boost/log/trivial.hpp>

namespace ZGen {

namespace ShiftReduce {

FullPipe::FullPipe(int beam_size)
  : Pipe(beam_size) {
}

FullPipe::~FullPipe() {
}

//
int FullPipe::get_possible_actions(const StateItem & item,
    std::vector<action::action_t> & actions) {
  actions.clear();

  if (item.stack.size() == 1) {
    for (int j = 0; j < item.N; ++ j) {
      if (item.buffer.test(j)) {
        word_t   word = input_ref->forms[j];
        postag_t  tag = input_ref->postags[j];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
      }
    }
  } else {
    bool all_descendants_shifted = true;
    int top0 = item.stack.back();
    const DependencyTree::edgeset_t& descendants = tree.descendants(top0);

    for (int j = 0; j < descendants.size(); ++ j) {
      int d = descendants[j];
      if (item.buffer.test(d)) {
        all_descendants_shifted = false;
        break;
      }
    }

    if (!all_descendants_shifted) {
      for (int j = 0; j < descendants.size(); ++ j) {
        int d = descendants[j];
        if (item.buffer.test(d)) {
          word_t word = input_ref->forms[d];
          postag_t tag = input_ref->postags[d];
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, d));
        }
      }
    } else {
      int top1 = (item.stack.size() > 2 ? item.stack[item.stack.size() - 2]: -1);

      if (top1 >= 0 && tree.arc(top0, top1)) {
        deprel_t deprel = input_ref->deprels[top1];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, deprel, 0));
      } else if (top1 >= 0 && tree.arc(top1, top0)) {
        deprel_t deprel = input_ref->deprels[top0];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, deprel, 0));
      } else {
        const DependencyTree::edgeset_t& siblings = tree.siblings(top0);

        for (int i = 0; i < siblings.size(); ++ i) {
          int s = siblings[i];
          if (item.buffer.test(s)) {
            word_t word = input_ref->forms[s];
            postag_t tag = input_ref->postags[s];
            actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, s));
          }
        }

        int h = tree.head(top0);
        if (item.buffer.test(h)) {
          word_t word = input_ref->forms[h];
          postag_t tag = input_ref->postags[h];
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, h));
        }
      }
    }
  }

  return (int)actions.size();
}


int FullPipe::config_sentence(const dependency_t* input) {
  input_ref= input;
  tree.set_ref(input);
  return 0;
}


int FullPipe::config_initial_lattice() {
  for (int i = 0; i < input_ref->deprels.size(); ++ i) {
    lattice->deprels[i] = input_ref->deprels[i];
  }
}


}

}


