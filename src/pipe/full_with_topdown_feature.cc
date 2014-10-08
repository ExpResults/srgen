#include "pipe/full_with_topdown_feature.h"

namespace ZGen {

namespace ShiftReduce {

FullWithTopDownFeaturePipe::FullWithTopDownFeaturePipe(bool learn, 
    int beam_size)
  : FullPipe(NULL, learn, true, beam_size) {
}

FullWithTopDownFeaturePipe::~FullWithTopDownFeaturePipe() {
}

int FullWithTopDownFeaturePipe::config_sentence(const dependency_t* input) {
  FullPipe::config_sentence(input);
  int N = input->size();
  directions.resize(N);
  if (N == input->extras.size()) {
    for (int i = 0; i < N; ++ i) {
      const std::string& extra = input->extras[i];
      int delimiter = extra.find_first_of('|');
      if (std::string::npos == delimiter) {
        directions[i].first = std::stod(extra.substr(0, delimiter));
        directions[i].second = std::stod(extra.substr(delimiter+ 1));
      } else {
        directions[i].first = directions[i].second = 0.5;
      }
    }
  } else {
    for (int i = 0; i < N; ++ i) {
      directions[i].first = directions[i].second = 0.5;
    }
  }
  return 0;
}

int FullWithTopDownFeaturePipe::hist(double probability) {
  if (probability < 0.01) {
    return 1;
  } else if (probability < 0.05) {
    return 2;
  } else if (probability < 0.95) {
    return 3;
  } else if (probability < 0.99) {
    return 4;
  }
  return 5;
}

} //  end for namespace Shiftreduce

} //  end for namespace ZGen
