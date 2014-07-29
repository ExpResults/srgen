#ifndef __SR_CONTEXT_H__
#define __SR_CONTEXT_H__

#include "state.h"

namespace SR {

struct Context {
  Context(const StateItem & item) {
    int N = item.sentence_ref->size();
    int S0 = item.stack_top();
    int S0ld;

    S0w = item.sentence_ref->at(S0);
    S0p = item.postags[S0];

#define _LEGEAL_RANGE_(x) (((x) >= 0) && ((x) < N))

    if ( _LEGEAL_RANGE_(item.left_most_child[S0]) ) {
      int S0ld = item.left_most_child[S0];

      S0ldw = item.sentence_ref->at(S0ld);
      S0ldp = item.postags[S0ld];

      if ( _LEGEAL_RANGE_(item.left_2nd_most_child[S0]) ) {
        int S0l2d = item.left_2nd_most_child[S0];

        S0l2dw = item.sentence_ref->at(S0l2d);
        S0l2dp = item.postags[S0l2d];
      } else {
        S0l2dw = WordEncoderAndDecoder::NONE;
        S0l2dp = PoSTagEncoderAndDecoder::NONE;
      }

      if ( _LEGEAL_RANGE_(item.left_most_child[S0ld]) ) {
        int S0ldd = item.left_most_child[S0ld];

        S0lddw = item.sentence_ref->at(S0ldd);
        S0lddp = item.postags[S0ldd];
      } else {
        S0lddw = WordEncoderAndDecoder::NONE;
        S0lddp = PoSTagEncoderAndDecoder::NONE;
      }
    } else {
      S0ldw  = WordEncoderAndDecoder::NONE;
      S0ldp  = PoSTagEncoderAndDecoder::NONE;
      S0l2dw = WordEncoderAndDecoder::NONE;
      S0l2dp = PoSTagEncoderAndDecoder::NONE;
      S0lddw = WordEncoderAndDecoder::NONE;
      S0lddp = PoSTagEncoderAndDecoder::NONE;
    }

    if ( _LEGEAL_RANGE_(item.right_most_child[S0]) ) {
      int S0rd = item.right_most_child[S0];

      S0rdw = item.sentence_ref->at(S0rd);
      S0rdp = item.postags[S0rd];

      if ( _LEGEAL_RANGE_(item.right_2nd_most_child[S0]) ) {
        int S0r2d = item.right_2nd_most_child[S0];

        S0r2dw = item.sentence_ref->at(S0r2d);
        S0r2dp = item.postags[S0r2d];
      } else {
        S0l2dw = WordEncoderAndDecoder::NONE;
        S0l2dp = PoSTagEncoderAndDecoder::NONE;
      }

      if ( _LEGEAL_RANGE_(item.right_most_child[S0rd]) ) {
        S0rddw = item.sentence_ref->at(S0rd);
        S0rddp = item.postags[S0rd];
      } else {
        S0rddw = WordEncoderAndDecoder::NONE;
        S0rddp = PoSTagEncoderAndDecoder::NONE;
      }
    } else {
      S0rdw  = WordEncoderAndDecoder::NONE;
      S0rdp  = PoSTagEncoderAndDecoder::NONE;
      S0r2dw = WordEncoderAndDecoder::NONE;
      S0r2dp = PoSTagEncoderAndDecoder::NONE;
      S0rddw = WordEncoderAndDecoder::NONE;
      S0rddp = PoSTagEncoderAndDecoder::NONE;
    }
  }

  word_t    S0w, S0ldw, S0rdw, S0lddw, S0rddw, S0l2dw, S0r2dw;
  postag_t  S0p, S0ldp, S0rdp, S0lddp, S0rddp, S0l2dp, S0r2dp;
};

}

#endif  //  end for __SR_CONTEXT_H__
