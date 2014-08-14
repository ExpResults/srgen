#ifndef __SR_CONTEXT_H__
#define __SR_CONTEXT_H__

#include "types/state.h"
#include "util/utils.h"   //  for bin

#define _LEGEAL_RANGE_(x) (((x) >= 0) && ((x) < N))

#define __SET_CTX(prefix) do { \
  prefix##w = (item.instance_ref->forms).at(prefix); \
  prefix##p = item.postags[prefix]; \
  prefix##l = item.deprels[prefix]; \
  /*prefix##k = item.rank[prefix]; */\
} while (0);

#define __CLEAR_CTX(prefix) do { \
  prefix##w = WordEncoderAndDecoder::NONE; \
  prefix##p = PoSTagEncoderAndDecoder::NONE; \
  prefix##l = DeprelsEncoderAndDecoder::NONE; \
  /*prefix##k = 0;*/ \
} while (0);

#define __SET_CNT(prefix) do { \
  prefix##la = item.nr_left_children[prefix]; \
  prefix##ra = item.nr_right_children[prefix]; \
  prefix##ls = bin(item.nr_left_descendant[prefix]); \
  prefix##rs = bin(item.nr_right_descendant[prefix]); \
} while (0);

#define __CLEAR_CNT(prefix) do { \
  prefix##la = 0; \
  prefix##ra = 0; \
  prefix##ls = 0; \
  prefix##rs = 0; \
} while (0);


namespace ZGen {

namespace ShiftReduce {

const int kNoneWord = WordEncoderAndDecoder::NONE;
const int kNonePoSTag = PoSTagEncoderAndDecoder::NONE;

struct Context {
  Context(const StateItem & item) {
    int N = item.instance_ref->size();
    int S0 = item.stack_top();

    const sentence_t & forms = item.instance_ref->forms;

    if (S0 >= 0) {
      _is_begin_state = false;

      S0w = forms.at(S0);
      S0p = item.postags[S0];
      __SET_CNT(S0);

      if ( _LEGEAL_RANGE_(item.left_most_child[S0]) ) {
        int S0ld = item.left_most_child[S0];
        __SET_CTX(S0ld);

        if ( _LEGEAL_RANGE_(item.left_2nd_most_child[S0]) ) {
          int S0l2d = item.left_2nd_most_child[S0];
          __SET_CTX(S0l2d);
        } else {
          __CLEAR_CTX(S0l2d);
        }

        if ( _LEGEAL_RANGE_(item.left_most_child[S0ld]) ) {
          int S0ldd = item.left_most_child[S0ld];
          __SET_CTX(S0ldd);
        } else {
          __CLEAR_CTX(S0ldd);
        }
      } else {
        __CLEAR_CTX(S0ld);
        __CLEAR_CTX(S0l2d);
        __CLEAR_CTX(S0ldd);
      }

      if ( _LEGEAL_RANGE_(item.right_most_child[S0]) ) {
        int S0rd = item.right_most_child[S0];
        __SET_CTX(S0rd);

        if ( _LEGEAL_RANGE_(item.right_2nd_most_child[S0]) ) {
          int S0r2d = item.right_2nd_most_child[S0];
          __SET_CTX(S0r2d);
        } else {
          __CLEAR_CTX(S0l2d);
        }

        if ( _LEGEAL_RANGE_(item.right_most_child[S0rd]) ) {
          int S0rdd = item.right_most_child[S0rd];
          __SET_CTX(S0rdd);
        } else {
          __CLEAR_CTX(S0rdd);
        }
      } else {
        __CLEAR_CTX(S0rd);
        __CLEAR_CTX(S0r2d);
        __CLEAR_CTX(S0rdd);
      }
    } else {
      // The given state is begin state.
      _is_begin_state = true;
      //
      S0w = WordEncoderAndDecoder::BEGIN;
      S0p = PoSTagEncoderAndDecoder::BEGIN;
      __CLEAR_CNT(S0);
      __CLEAR_CTX(S0ld);
      __CLEAR_CTX(S0l2d);
      __CLEAR_CTX(S0ldd);
      __CLEAR_CTX(S0rd);
      __CLEAR_CTX(S0r2d);
      __CLEAR_CTX(S0rdd);
    }

    if (item.stack.size() > 2) {
      _has_S1 = true;
      int S1 = item.stack[item.stack.size() - 2];

      S1w = forms.at(S1);
      S1p = item.postags[S1];
      __SET_CNT(S1);
      S0S1Dist = bin(item.rank[S1]- item.rank[S0]);

      if ( _LEGEAL_RANGE_(item.left_most_child[S1]) ) {
        int S1ld = item.left_most_child[S1];
        __SET_CTX(S1ld);
        if ( _LEGEAL_RANGE_(item.left_2nd_most_child[S1]) ) {
          int S1l2d = item.left_2nd_most_child[S1];
          __SET_CTX(S1l2d);
        } else {
          __CLEAR_CTX(S1l2d);
        }

        if ( _LEGEAL_RANGE_(item.left_most_child[S1ld]) ) {
          int S1ldd = item.left_most_child[S1ld];
          __SET_CTX(S1ldd);
        } else {
          __CLEAR_CTX(S1ldd);
        }
      } else {
        __CLEAR_CTX(S1ld);
        __CLEAR_CTX(S1l2d);
        __CLEAR_CTX(S1ldd);
      }

      if ( _LEGEAL_RANGE_(item.right_most_child[S1]) ) {
        int S1rd = item.right_most_child[S1];
        __SET_CTX(S1rd);
        if ( _LEGEAL_RANGE_(item.right_2nd_most_child[S1]) ) {
          int S1r2d = item.right_2nd_most_child[S1];
          __SET_CTX(S1r2d);
        } else {
          __CLEAR_CTX(S1r2d);
        }

        if ( _LEGEAL_RANGE_(item.right_most_child[S1rd]) ) {
          int S1rdd = item.right_most_child[S1rd];
          __SET_CTX(S1rdd);
        } else {
          __CLEAR_CTX(S1rdd);
        }
      } else {
        __CLEAR_CTX(S1rd);
        __CLEAR_CTX(S1r2d);
        __CLEAR_CTX(S1rdd);
      }
    } else {
      _has_S1 = false;
      S1w = WordEncoderAndDecoder::NONE;
      S1p = PoSTagEncoderAndDecoder::NONE;
      __CLEAR_CNT(S1);
      __CLEAR_CTX(S1ld);
      __CLEAR_CTX(S1l2d);
      __CLEAR_CTX(S1ldd);
      __CLEAR_CTX(S1rd);
      __CLEAR_CTX(S1r2d);
      __CLEAR_CTX(S1rdd);
      S0S1Dist = 0;
    }

    // SHIFTED words
    // item.word_sequence.size() should be equal to item.postag_sequence.size()
    int C = item.word_sequence.size();

    W0 = item.word_sequence[C - 1];
    P0 = item.postag_sequence[C - 1];

    _has_W1 = false;
    if (C > 1) {
      _has_W1 = true;
      W1 = item.word_sequence[C - 2];
      P1 = item.postag_sequence[C - 2];

      if (C > 2) {
        W2 = item.word_sequence[C - 3];
        P2 = item.postag_sequence[C - 3];
      } else {
        W2 = WordEncoderAndDecoder::NONE;
        P2 = PoSTagEncoderAndDecoder::NONE;
      }
    } else {
      W1 = (W2 = WordEncoderAndDecoder::NONE);
      P1 = (P2 = PoSTagEncoderAndDecoder::NONE);
    }
  }

  bool is_begin_state() const {
    return _is_begin_state;
  }

  bool has_S1() const {
    return _has_S1;
  }

  bool has_W1() const {
    return _has_W1;
  }

  bool has_W2() const {
    return _has_W2;
  }

  bool      _is_begin_state;
  bool      _has_S1, _has_W1, _has_W2;

  word_t    S0w, S0ldw, S0rdw, S0lddw, S0rddw, S0l2dw, S0r2dw;
  postag_t  S0p, S0ldp, S0rdp, S0lddp, S0rddp, S0l2dp, S0r2dp;
  deprel_t       S0ldl, S0rdl, S0lddl, S0rddl, S0l2dl, S0r2dl;
  int       S0k, S0ldk, S0rdk, S0lddk, S0rddk, S0l2dk, S0r2dk;
  int       S0la, S0ra;
  int       S0ls, S0rs;

  word_t    S1w, S1ldw, S1rdw, S1lddw, S1rddw, S1l2dw, S1r2dw;
  postag_t  S1p, S1ldp, S1rdp, S1lddp, S1rddp, S1l2dp, S1r2dp;
  deprel_t       S1ldl, S1rdl, S1lddl, S1rddl, S1l2dl, S1r2dl;
  int       S1k, S1ldk, S1rdk, S1lddk, S1rddk, S1l2dk, S1r2dk;
  int       S1la, S1ra;
  int       S1ls, S1rs;

  word_t    W0, W1, W2;
  postag_t  P0, P1, P2;

  int       S0S1Dist;
};

}

}

#undef __SET_CTX
#undef __CLEAR_CTX
#undef __SET_CNT
#undef __CLEAR_CNT

#endif  //  end for __SR_CONTEXT_H__
