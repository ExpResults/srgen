#include "pipe/none.h"

namespace ZGen {

namespace ShiftReduce {

// The NonePipe
NonePipe::NonePipe(const char * postag_dict_path,
    int beam_size)
  : constraint(postag_dict_path),
  Pipe(beam_size) {
}


int NonePipe::get_possible_actions(const StateItem & item,
    std::vector<action::action_t> & actions) {
  actions.clear();

  // Then loop over the words in the buffer.
  for (int j = 0; j < item.N; ++ j) {
    if (item.buffer.test(j)) {

      // Generate all possible SHIFT actions, first loop over possible PoSTags.
      const char * name = WordEngine::get_const_instance().decode(item.instance_ref->forms.at(j));
      std::vector< postag_t > possible_tags;
      if (input_ref->is_phrases[j]) {
        possible_tags.push_back(PoSTagEncoderAndDecoder::NP);
      } else {
        constraint.get_possible_tags(name, possible_tags);
      }

      for (int i = 0; i < possible_tags.size(); ++ i) {
        postag_t tag = possible_tags[i];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag,
              item.instance_ref->forms[j], j));
      }
    }
  }

  if (item.stack.size() > 2) {
    // There is one pseudo node in the stack
    // Generate LEFT-ARC and RIGHT-ARC actions
    for (int i = kStartIndexOfValidDependencyRelation;
        i < kNumberOfDependencyRelations; ++ i) {
      actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, i, 0));
      actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, i, 0));
    }
  }

  return (int)actions.size();
}


int NonePipe::config_sentence(const dependency_t* input) {
  input_ref = input;
}


int NonePipe::config_initial_lattice() {
}

}   //  end for namespace ShiftReduce

}   //  end for namespace ZGen
