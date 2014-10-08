#include "pipe/none.h"

namespace ZGen {

namespace ShiftReduce {

// The NonePipe
NonePipe::NonePipe(const char * postag_dict_path,
    bool learn,
    bool output_label,
    int beam_size)
  : Pipe(postag_dict_path, learn, output_label, beam_size) {
}


int NonePipe::get_possible_actions(const StateItem & item,
    std::vector<action::action_t> & actions) {
  actions.clear();

  // Then loop over the words in the buffer.
  for (int j = 0; j < item.N; ++ j) {
    if (item.buffer.test(j)) {
      word_t   word = input_ref->forms[j];
      postag_t  tag = input_ref->postags[j];
      get_possible_shift_actions(item, j, word, tag, actions);
    }
  }

  if (item.stack.size() > 2) {
    if (labeled) {
      // There is one pseudo node in the stack
      // Generate LEFT-ARC and RIGHT-ARC actions
      for (int i = kStartIndexOfValidDependencyRelation;
          i < kNumberOfDependencyRelations; ++ i) {
        actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, i, 0));
        actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, i, 0));
      }
    } else {
      actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, 0, 0));
      actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, 0, 0));
    }
  }

  return (int)actions.size();
}


int NonePipe::config_sentence(const dependency_t* input) {
  input_ref = input;
  return 0;
}


int NonePipe::config_initial_lattice() {
  return 0;
}

}   //  end for namespace ShiftReduce

}   //  end for namespace ZGen
