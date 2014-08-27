#include "pipe/none_with_constrain.h"

namespace ZGen {

namespace ShiftReduce {

NoneWithConstrainPipe::NoneWithConstrainPipe(int beam_size) 
  : Pipe(beam_size) {
}

NoneWithConstrainPipe::~NoneWithConstrainPipe() {
}

void NoneWithConstrainPipe::get_possible_action(const StateItem& item,
    action_collection_t& actions) {
}

int NoneWithConstrainPipe::config_sentence(const dependency_t* input) {
  input_ref = input;
  return 0;
}

}

}
