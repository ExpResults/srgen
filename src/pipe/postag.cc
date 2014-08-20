#include "pipe/postag.h"

namespace ZGen {

namespace ShiftReduce {

PoSTagPipe::PoSTagPipe(int beam_size)
  : Pipe(beam_size) {
}


int PoSTagPipe::get_possible_actions(const StateItem & item,
    action_collection_t & actions) {
  actions.clear();

  // Then loop over the words in the buffer.
  for (int j = 0; j < item.N; ++ j) {
    if (item.buffer.test(j)) {
      word_t   word = input_ref->forms[j];
      postag_t  tag = input_ref->postags[j];
      actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
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


int PoSTagPipe::config_sentence(const dependency_t* input) {
  input_ref = input;
  return  0;
}


int PoSTagPipe::config_initial_lattice() {
  return 0;
}


} 

}
