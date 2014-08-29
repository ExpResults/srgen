#include "util/utils.h"
#include "types/engine.h"
#include <ctime>

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

  // std::srand( std::time(0) );
  std::random_shuffle(order.begin(), order.end());

  shuffled_instance.forms.resize(N);
  shuffled_instance.is_phrases.resize(N);

  if (N == instance.postags.size()) {
    shuffled_instance.postags.resize(N);
  }

  if (N == instance.heads.size()) {
    shuffled_instance.heads.resize(N);
    shuffled_instance.deprels.resize(N);
  }

  if (N == instance.extras.size()) {
    shuffled_instance.extras.resize(N);
  }

  for (int i = 0; i < N; ++ i) {
    shuffled_instance.forms[order[i]] = instance.forms[i];
    shuffled_instance.is_phrases[order[i]] = instance.is_phrases[i];
    
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

    if (N == instance.extras.size()) {
      shuffled_instance.extras[order[i]] = instance.extras[i];
    }
  }

  shuffled_instance.phrases.clear();
  shuffled_instance.phrases.resize(N);

  shuffled_instance.words.clear();
  shuffled_instance.words.reserve(instance.words.size());

  for (int rank = 0, k = 0; rank < order.size(); ++ rank) {
    for (int j = 0; j < order.size(); ++ j) {
      if (order[j] == rank) {
        shuffled_instance.phrases[rank].first = k;
        for (int l = instance.phrases[j].first;
            l < instance.phrases[j].second; ++ l, ++ k) {
          shuffled_instance.words.push_back(instance.words[l]);
        }
        shuffled_instance.phrases[rank].second = k;
        break;
      }
    }
  }

}


int bin(int x) {
  int sign = (x < 0 ? -1 : 1);

  x = x * sign;
  if (x > 40) {
    return sign * 40;
  } else if (x > 20) {
    return sign * 20;
  } else if (x > 10) {
    return sign * 10;
  } else if (x > 5) {
    return sign * 5;
  }
  return x * sign;
}


}

}
