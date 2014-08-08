#include "utils.h"

namespace ZGen {

namespace ShiftReduce {

void shuffle_instance(const dependency_t & instance,
    dependency_t & shuffled_instance,
    std::vector<int> & order) {
  int N = instance.forms.size();

  order.clear();
  for (int i = 0; i < N; ++ i) {
    order.push_back(i);
  }

  std::random_shuffle(order.begin(), order.end());

  shuffled_instance.forms.resize(N);

  if (N == instance.postags.size()) {
    shuffled_instance.postags.resize(N);
  }

  if (N == instance.heads.size()) {
    shuffled_instance.heads.resize(N);
    shuffled_instance.deprels.resize(N);
  }

  for (int i = 0; i < N; ++ i) {
    shuffled_instance.forms[order[i]] = instance.forms[i];

    if (N == instance.postags.size()) {
      shuffled_instance.postags[order[i]] = instance.postags[i];
    }

    if (N == instance.heads.size()) {
      if (instance.heads[i] == -1) {
        shuffled_instance.heads[order[i]] = -1;
        shuffled_instance.deprels[order[i]] = DeprelsEncoderAndDecoder::NONE;
      } else {
        shuffled_instance.heads[order[i]] = order[instance.heads[i]];
        shuffled_instance.deprels[order[i]] = instance.deprels[i];
      }
    }
  }
}

}

}
