#include "pipe/full.h"
#include <boost/log/trivial.hpp>

namespace ZGen {

namespace ShiftReduce {

FullPipe::FullPipe(const char* postag_dict_path,
    bool output_label,
    int beam_size)
  : Pipe(postag_dict_path, output_label, beam_size) {
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
        get_possible_shift_actions(item, j, word, tag, actions);
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
          get_possible_shift_actions(item, d, word, tag, actions);
        }
      }
    } else {
      int top1 = item.top1;

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
            get_possible_shift_actions(item, s, word, tag, actions);
          }
        }

        int h = tree.head(top0);
        if (item.buffer.test(h)) {
          word_t word = input_ref->forms[h];
          postag_t tag = input_ref->postags[h];
          get_possible_shift_actions(item, h, word, tag, actions);
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
  return 0;
}


}

}


