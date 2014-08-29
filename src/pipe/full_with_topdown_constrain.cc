#include "pipe/full_with_topdown_constrain.h"

namespace ZGen {

namespace ShiftReduce {

FullWithTopDownConstrainPipe::FullWithTopDownConstrainPipe(int beam_size)
  : FullPipe(NULL, true, beam_size) {
}

/**/
FullWithTopDownConstrainPipe::~FullWithTopDownConstrainPipe() {
}


int FullWithTopDownConstrainPipe::config_sentence(const dependency_t* input) {
  FullPipe::config_sentence(input);
  int N = input->size();
  direction.resize(N);
  if (input->extras.size() == N) {
    for (int i = 0; i < N; ++ i) {
      direction[i] = atoi(input->extras[i].c_str());
    }
  } else {
    for (int i = 0; i < N; ++ i) {
      direction[i] = 0;
    }
  }
  return 0;
}

/**/
int FullWithTopDownConstrainPipe::get_possible_actions(const StateItem& item,
    action_collection_t& actions) {
  actions.clear();

  if (item.stack.size() == 1) {
    for (int j = 0; j < item.N; ++ j) {
      if (item.buffer.test(j) && conform_constrain(item, j)) {
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
        if (item.buffer.test(d) && conform_constrain(item, d)) {
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
          if (item.buffer.test(s) && conform_constrain(item, s)) {
            word_t word = input_ref->forms[s];
            postag_t tag = input_ref->postags[s];
            get_possible_shift_actions(item, s, word, tag, actions);
          }
        }

        int h = tree.head(top0);
        if (item.buffer.test(h) && conform_constrain(item, h)) {
          word_t word = input_ref->forms[h];
          postag_t tag = input_ref->postags[h];
          get_possible_shift_actions(item, h, word, tag, actions);
        }
      }
    }
  }

  return (int)actions.size();
}

bool FullWithTopDownConstrainPipe::conform_constrain(const StateItem& item, int j) {
  const DependencyTree::edgeset_t& children = tree.children(j);
  for (int i = 0; i < children.size(); ++ i) {
    int c = children[i];
    if (item.buffer.test(c) && illegal_right_arc(j, c)) {
      return false;
    }
  }
  for (int i = j; tree.head(i) != -1; i = tree.head(i)) {
    int h = tree.head(i);
    if (item.buffer.test(h) && illegal_left_arc(h, i)) {
      return false;
    }
  }
  return true;
}

bool FullWithTopDownConstrainPipe::illegal_right_arc(int from, int to) {
  int d = direction[to];
  if (-1 == d) {
    return true;
  }
  return false;
}

bool FullWithTopDownConstrainPipe::illegal_left_arc(int from, int to) {
  int d = direction[to];
  if (1 == d) {
    return true;
  }
  return false;
}

} //  end for namespace Shiftreduce

} //  end for namespace ZGen
