#include "pipe/full_with_guidance.h"

namespace ZGen {

namespace ShiftReduce {

FullWithGuidancePipe::FullWithGuidancePipe(int beam_size) 
  : FullPipe(beam_size) {
}


FullWithGuidancePipe::~FullWithGuidancePipe() {
}


int FullWithGuidancePipe::config_sentence(const dependency_t* input) {
  input_ref = input;
  tree.set_ref(input);
  tree_with_guidance.set_ref(input);
  return 0;
}

}

}
