#include "pipe/partial.h"
#include <boost/log/trivial.hpp>

namespace ZGen {

namespace ShiftReduce {

const int kStackLegalStart = 1;

PartialPipe::PartialPipe(const char* postag_dict_path,
    bool output_label,
    int beam_size)
  : constraint(postag_dict_path),
  labeled(output_label),
  Pipe(beam_size) {
}


PartialPipe::~PartialPipe() {
}


void PartialPipe::get_possible_shift_actions(const StateItem& item, 
    int j, word_t word, postag_t tag, action_collection_t& actions) {
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
      tag = possible_tags[i];
      actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag,
            item.instance_ref->forms.at(j), j));
    }
  } else {
    actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
  }

}

//
int PartialPipe::get_possible_actions(const StateItem& item,
    action_collection_t& actions) {
  actions.clear();

  if (item.stack.size() == 1) {
    // The initial state, every word is OK to be shifted onto the stack.
    for (int j = 0; j < item.N; ++ j) {
      if (item.buffer.test(j)) {
        word_t   word = input_ref->forms[j];
        postag_t  tag = input_ref->postags[j];
        get_possible_shift_actions(item, j, word, tag, actions);
      }
    }
  } else {
    if (legal_left_arc(item)) {
      if (!labeled) {
        deprel_t deprel = DeprelsEncoderAndDecoder::NONE;
        actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, deprel, 0));
      } else {
        for (deprel_t deprel = kStartIndexOfValidDependencyRelation;
            deprel < kNumberOfDependencyRelations; ++ deprel) {
          actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, deprel, 0));
        }
      }
    }

    if (legal_right_arc(item)) {
      if (!labeled) {
        deprel_t deprel = DeprelsEncoderAndDecoder::NONE;
        actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, deprel, 0));
      } else {
        for (deprel_t deprel = kStartIndexOfValidDependencyRelation;
            deprel < kNumberOfDependencyRelations; ++ deprel) {
          actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, deprel, 0));
        }
      }
    }

    for (int j = 0; j < item.N; ++ j) {
      word_t word = input_ref->forms[j];
      postag_t tag = input_ref->postags[j];
      if (legal_shift(item, j)) {
        get_possible_shift_actions(item, j, word, tag, actions);
      }
    }
  }
  return (int)actions.size();
}


int PartialPipe::config_sentence(const dependency_t* input) {
  input_ref= input;
  forest.set_ref(input);
  BOOST_LOG_TRIVIAL(trace) << forest;
  return 0;
}


int PartialPipe::config_initial_lattice() {
  return 0;
}


//
bool PartialPipe::legal_left_arc(const StateItem& item) {
  if (item.stack.size() < 3 ||
      !(forest.is_root(item.top1) || forest.arc(item.top0, item.top1))) {
    return false;
  }

  const DependencyTree::edgeset_t& children = forest.children(item.top1);
  for (int i = 0; i < children.size(); ++ i) {
    int d = children[i];
    if (item.buffer[d]) {
      return false;
    }
  }

  for (int i = kStackLegalStart; i < item.stack.size(); ++ i) {
    int k = item.stack[i];
    if (forest.head(k) == item.top1) {
      return false;
    }
  }

  return true;
}

//
bool PartialPipe::legal_right_arc(const StateItem& item) {
  if (item.stack.size() < 3 ||
      !(forest.is_root(item.top0) || forest.arc(item.top1, item.top0))) {
    return false;
  }

  const DependencyTree::edgeset_t& children = forest.children(item.top0);
  for (int i = 0; i < children.size(); ++ i) {
    int d = children[i];
    if (item.buffer[d]) {
      return false;
    }
  }

  for (int i = kStackLegalStart; i < item.stack.size(); ++ i) {
    int k = item.stack[i];
    if (forest.head(k) == item.top0) {
      return false;
    }
  }

  return true;
}


//
bool PartialPipe::legal_shift(const StateItem& item, int target) {
  // If this word has already be shifted onto the stack, ignore it
  if (!item.buffer[target]) {
    return false;
  }

  int L = item.stack.size();

  std::bitset<kMaxNumberOfWords> sigma;
  for (int i = kStackLegalStart; i < L; ++ i) {
    int k = item.stack[i];
    sigma[k] = true;
  }

  for (int i = kStackLegalStart; i < L; ++ i) {
    int k = item.stack[i];
    if (forest.same_ancestor(k, target)) {
      int lca = forest.lca(k, target);

      if (forest.head(k) == target) {
        for (int lmi = i; lmi < L; ++ lmi) {
          int lm = item.stack[lmi];

          if (!forest.is_root(lm) && forest.head(lm)!= target) {
            return false;
          }

          const DependencyTree::edgeset_t& cc = forest.children(lm);
          for (int l = 0; l < cc.size(); ++ l) {
            int c = cc[l];
            if (sigma[c] || item.buffer[c]) { return false; }
          }
        }
      } else if (lca == target) {
        return false;
        //
      } else if (forest.head(target) == k) {
        for (int lmi = i+ 1; lmi < L; ++ lmi) {
          int lm = item.stack[lmi];
          if (!forest.is_root(lm) && forest.head(lm)!= target) {
            return false;
          }
          const DependencyTree::edgeset_t& cc = forest.children(lm);
          for (int l = 0; l < cc.size(); ++ l) {
            int c = cc[l];
            if (sigma[c] || item.buffer[c]) { return false; }
          }
        }
      } else if (lca == k) {
        const DependencyTree::edgeset_t& desc = forest.descendants(lca);
        for (int lmi = i+ 1; lmi < L; ++ lmi) {
          int lm = item.stack[lmi];
          if (!forest.is_root(lm) &&
              std::find(desc.begin(), desc.end(), lm)== desc.end()) {
              //!(forest.lca(lm, target) ==lm && forest.lca(lca,lm) == lca)) {
            return false;
          }

          if (forest.is_root(lm)) {
            const DependencyTree::edgeset_t& cc = forest.children(lm);
            for (int l = 0; l < cc.size(); ++ l) {
              int c = cc[l];
              if (sigma[c] || item.buffer[c]) { return false; }
            }
          }
        }
      } else {
        if (forest.head(k) != lca) {
          return false;
        }

        for (int l = kStackLegalStart; l < i; ++ l) {
          int lm = item.stack[l];
          if (lm == lca) {
            return false;
          }
        }

        const DependencyTree::edgeset_t& cc = forest.children(k);
        for (int l = 0; l < cc.size(); ++ l) {
          int c = cc[l];
          if (sigma[c] || item.buffer[c]) { return false; }
        }

        if (item.buffer[lca]) {
          const DependencyTree::edgeset_t& desc = forest.descendants(lca);
          for (int l = i+ 1; l < L; ++ l) {
            int lm = item.stack[l];
            if (!forest.is_root(lm) &&
                std::find(desc.begin(), desc.end(), lm)== desc.end()) {
              //!(forest.lca(lm, target) ==lm && forest.lca(lca,lm) == lca)) {
              return false;
            }

            if (forest.is_root(lm)) {
              const DependencyTree::edgeset_t& cc = forest.children(lm);
              for (int l = 0; l < cc.size(); ++ l) {
                int c = cc[l];
                if (sigma[c] || item.buffer[c]) { return false; }
              }
            }
          }
        }
      }
    }
  }
  return true;
}

}

}


