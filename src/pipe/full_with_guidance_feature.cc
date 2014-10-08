#include "pipe/full_with_guidance_feature.h"

namespace ZGen {

namespace ShiftReduce {

FullWithGuidanceFeaturePipe::FullWithGuidanceFeaturePipe(bool learn, 
    int beam_size) 
  : FullPipe(NULL, learn, true, beam_size) {
}


FullWithGuidanceFeaturePipe::~FullWithGuidanceFeaturePipe() {
}


int FullWithGuidanceFeaturePipe::config_sentence(const dependency_t* input) {
  input_ref = input;
  tree.set_ref(input);
  tree_with_guidance.set_ref(input);
  return 0;
}

}

}
