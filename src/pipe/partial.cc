#include "pipe/partial.h"
#include <boost/log/trivial.hpp>

namespace ZGen {

namespace ShiftReduce {

PartialPipe::PartialPipe(const char* postag_dict_path,
    int beam_size)
  : constraint(postag_dict_path),
  Pipe(beam_size) {
}

PartialPipe::~PartialPipe() {
}

//
int PartialPipe::get_possible_actions(const StateItem& item,
    std::vector<action::action_t>& actions) {
  actions.clear();

  if (item.stack.size() == 1) {
    // The initial state, every word is OK to be shifted onto the stack.
    for (int j = 0; j < item.N; ++ j) {
      if (item.buffer.test(j)) {
        word_t   word = input_ref->forms[j];
        postag_t  tag = input_ref->postags[j];

        if (tag == PoSTagEncoderAndDecoder::NONE) {
          // If no postag is provided. Get a possible postag.
          const char* name= WordEngine::get_const_instance().decode(
              item.instance_ref->forms.at(j));
          std::vector< postag_t > possible_tags;

          if (input_ref->is_phrases[j]) {
            possible_tags.push_back(PoSTagEncoderAndDecoder::NP);
          } else {
            constraint.get_possible_tags(name, possible_tags);
          }

          for (int i = 0; i < possible_tags.size(); ++ i) {
            postag_t tag = possible_tags[i];
            actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag,
                  item.instance_ref->forms.at(j), j));
          }
        } else {
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
        }
      }
    }
  } else {
    int top1 = item.top1;

    if (top1 >= 0 && forest.arc(item.top0, item.top1)) {
      // One thing to note is that in Zhang, 2013, partial tree linearization's
      // input is unlabeled tree.
      deprel_t deprel = input_ref->deprels[item.top1];
      actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, deprel, 0));
    } else if (top1 >= 0 && forest.arc(item.top1, item.top0)) {
      deprel_t deprel = input_ref->deprels[item.top0];
      actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, deprel, 0));
      for (int j = 0; j < item.N; ++ j) {
        word_t word   = input_ref->forms[j];
        postag_t tag  = input_ref->postags[j];
        if (legal_shift(item, j)) {
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
        }
      }
    } else {
      if (legal_left_arc(item)) {
        deprel_t deprel = DeprelsEncoderAndDecoder::NONE;
        actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, deprel, 0));
      }

      if (legal_right_arc(item)) {
        deprel_t deprel = DeprelsEncoderAndDecoder::NONE;
        actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, deprel, 0));
      }

      for (int j = 0; j < item.N; ++ j) {
        word_t word = input_ref->forms[j];
        postag_t tag = input_ref->postags[j];

        if (legal_shift(item, j)) {
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
        }
      }
    }
  }
  return (int)actions.size();
}


int PartialPipe::config_sentence(const dependency_t* input) {
  input_ref= input;
  forest.set_ref(input);
  return 0;
}


int PartialPipe::config_initial_lattice() {
  return 0;
}

//
bool PartialPipe::legal_left_arc(const StateItem& item) {
  if (item.stack.size() < 3 || !forest.is_root(item.top1)) {
    return false;
  }

  const DependencyTree::edgeset_t& descendants = forest.descendants(item.top1);
  for (int i = 0; i < descendants.size(); ++ i) {
    int d = descendants[i];
    if (item.buffer[d]) {
      return false;
    }
  }

  for (int i = 1; i < item.stack.size() - 1; ++ i) {
    int k = item.stack[i];
    if (forest.head(k) == item.top1) {
      return false;
    }
  }

  return true;
}

//
bool PartialPipe::legal_right_arc(const StateItem& item) {
  if (item.stack.size() < 3 || !forest.is_root(item.top0)) {
    return false;
  }

  const DependencyTree::edgeset_t& descendants = forest.descendants(item.top0);
  for (int i = 0; i < descendants.size(); ++ i) {
    int d = descendants[i];
    if (item.buffer[d]) {
      return false;
    }
  }

  for (int i = 1; i < item.stack.size()- 1; ++ i) {
    int h = item.stack[i];
    if (forest.head(h) == item.top0) {
      return false;
    }
  }
  return true;
}


//
bool PartialPipe::legal_shift(const StateItem& item, int target) {
  //
  if (!item.buffer[target]) {
    return false;
  }

  int left_most_i = -1;
  for (int i = 0; i < item.stack.size(); ++ i) {
    int k = item.stack[i];
    if (forest.head(k) == target) {
      left_most_i = i;
      break;
    }
  }

  if (left_most_i > -1) {
    for (int i = left_most_i; i < item.stack.size(); ++ i) {
      int k = item.stack[i];

      int h = forest.head(k);
      if (h >= 0 && h != target && item.buffer[h]) {
        // there are node that has right parent in the buffer.
        return false;
      }

      const DependencyTree::edgeset_t& children = forest.children(k);
      for (int l = 0; l< children.size(); ++ l) {
        int c = children[l];
        if (item.buffer[c]) {
          return false;
        }
      }
    }
  }

  int head_i = -1;
  for (int i = 0; i < item.stack.size(); ++ i) {
    int k = item.stack[i];
    if (forest.head(target) == k) {
      head_i = i;
      break;
    }
  }

  if (head_i > -1) {
    for (int i = head_i+ 1; i < item.stack.size(); ++ i) {
      int k = item.stack[i];
      int head_k = forest.head(k);

      if (head_k >= 0 && head_k != target && item.buffer[head_k]) {
        return false;
      }

      const DependencyTree::edgeset_t& children = forest.children(k);
      for (int l = 0; l< children.size(); ++ l) {
        int c = children[l];
        if (item.buffer[c]) {
          return false;
        }
      }
    }
  }

  return true;
}

}

}


