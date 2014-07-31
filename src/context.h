#ifndef __SR_CONTEXT_H__
#define __SR_CONTEXT_H__

#include "state.h"
#define _LEGEAL_RANGE_(x) (((x) >= 0) && ((x) < N))

namespace ZGen {

namespace ShiftReduce {

const int kNoneWord = WordEncoderAndDecoder::NONE;
const int kNonePoSTag = PoSTagEncoderAndDecoder::NONE;

struct Context {
  Context(const StateItem & item) {
    int N = item.sentence_ref->size();
    int S0 = item.stack_top();

    if (S0 >= 0) {
      _is_begin_state = false;

      S0w = item.sentence_ref->at(S0);
      S0p = item.postags[S0];
      S0la = item.nr_left_children[S0];
      S0ra = item.nr_right_children[S0];
      S0ls = item.nr_left_descendant[S0];
      S0rs = item.nr_right_descendant[S0];

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
    } else {
      // The given state is begin state.
      _is_begin_state = true;
      //
      S0w    = WordEncoderAndDecoder::BEGIN;
      S0p    = PoSTagEncoderAndDecoder::BEGIN;
      S0ldw  = WordEncoderAndDecoder::NONE;
      S0ldp  = PoSTagEncoderAndDecoder::NONE;
      S0l2dw = WordEncoderAndDecoder::NONE;
      S0l2dp = PoSTagEncoderAndDecoder::NONE;
      S0lddw = WordEncoderAndDecoder::NONE;
      S0lddp = PoSTagEncoderAndDecoder::NONE;
      S0rdw  = WordEncoderAndDecoder::NONE;
      S0rdp  = PoSTagEncoderAndDecoder::NONE;
      S0r2dw = WordEncoderAndDecoder::NONE;
      S0r2dp = PoSTagEncoderAndDecoder::NONE;
      S0rddw = WordEncoderAndDecoder::NONE;
      S0rddp = PoSTagEncoderAndDecoder::NONE;
      S0la = 0;
      S0ra = 0;
      S0ls = 0;
      S0rs = 0;
    }

    if (item.stack.size() > 2) {
      _has_S1 = true;
      int S1 = item.stack[item.stack.size() - 2];

      S1w = item.sentence_ref->at(S1);
      S1p = item.postags[S1];
      S1la = item.nr_left_children[S1];
      S1ra = item.nr_right_children[S1];
      S1ls = item.nr_left_descendant[S1];
      S1rs = item.nr_right_descendant[S1];

      if ( _LEGEAL_RANGE_(item.left_most_child[S1]) ) {
        int S1ld = item.left_most_child[S1];

        S1ldw = item.sentence_ref->at(S1ld);
        S1ldp = item.postags[S1ld];

        if ( _LEGEAL_RANGE_(item.left_2nd_most_child[S1]) ) {
          int S1l2d = item.left_2nd_most_child[S1];

          S1l2dw = item.sentence_ref->at(S1l2d);
          S1l2dp = item.postags[S1l2d];
        } else {
          S1l2dw = WordEncoderAndDecoder::NONE;
          S1l2dp = PoSTagEncoderAndDecoder::NONE;
        }

        if ( _LEGEAL_RANGE_(item.left_most_child[S1ld]) ) {
          int S1ldd = item.left_most_child[S1ld];
          S1lddw = item.sentence_ref->at(S1ldd);
          S1lddp = item.postags[S1ldd];
        } else {
          S1lddw = WordEncoderAndDecoder::NONE;
          S1lddp = PoSTagEncoderAndDecoder::NONE;
        }
      } else {
        S1ldw  = WordEncoderAndDecoder::NONE;
        S1ldp  = PoSTagEncoderAndDecoder::NONE;
        S1l2dw = WordEncoderAndDecoder::NONE;
        S1l2dp = PoSTagEncoderAndDecoder::NONE;
        S1lddw = WordEncoderAndDecoder::NONE;
        S1lddp = PoSTagEncoderAndDecoder::NONE;
      }

      if ( _LEGEAL_RANGE_(item.right_most_child[S1]) ) {
        int S1rd = item.right_most_child[S1];

        S1rdw = item.sentence_ref->at(S1rd);
        S1rdp = item.postags[S1rd];

        if ( _LEGEAL_RANGE_(item.right_2nd_most_child[S1]) ) {
          int S1r2d = item.right_2nd_most_child[S1];

          S1r2dw = item.sentence_ref->at(S1r2d);
          S1r2dp = item.postags[S1r2d];
        } else {
          S1l2dw = WordEncoderAndDecoder::NONE;
          S1l2dp = PoSTagEncoderAndDecoder::NONE;
        }

        if ( _LEGEAL_RANGE_(item.right_most_child[S1rd]) ) {
          S1rddw = item.sentence_ref->at(S1rd);
          S1rddp = item.postags[S1rd];
        } else {
          S1rddw = WordEncoderAndDecoder::NONE;
          S1rddp = PoSTagEncoderAndDecoder::NONE;
        }
      } else {
        S1rdw  = WordEncoderAndDecoder::NONE;
        S1rdp  = PoSTagEncoderAndDecoder::NONE;
        S1r2dw = WordEncoderAndDecoder::NONE;
        S1r2dp = PoSTagEncoderAndDecoder::NONE;
        S1rddw = WordEncoderAndDecoder::NONE;
        S1rddp = PoSTagEncoderAndDecoder::NONE;
      }
    } else {
      _has_S1 = false;
      S1w    = WordEncoderAndDecoder::NONE;
      S1p    = PoSTagEncoderAndDecoder::NONE;
      S1ldw  = WordEncoderAndDecoder::NONE;
      S1ldp  = PoSTagEncoderAndDecoder::NONE;
      S1l2dw = WordEncoderAndDecoder::NONE;
      S1l2dp = PoSTagEncoderAndDecoder::NONE;
      S1lddw = WordEncoderAndDecoder::NONE;
      S1lddp = PoSTagEncoderAndDecoder::NONE;
      S1rdw  = WordEncoderAndDecoder::NONE;
      S1rdp  = PoSTagEncoderAndDecoder::NONE;
      S1r2dw = WordEncoderAndDecoder::NONE;
      S1r2dp = PoSTagEncoderAndDecoder::NONE;
      S1rddw = WordEncoderAndDecoder::NONE;
      S1rddp = PoSTagEncoderAndDecoder::NONE;
      S1la = 0;
      S1ra = 0;
      S1ls = 0;
      S1rs = 0;
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
    }
  }

  bool is_begin_state() const {
    return _is_begin_state;
  }

  bool has_S1() const {
    return _has_S1;
  }

  bool has_W1() const {
    return _has_S1;
  }

  bool      _is_begin_state;
  bool      _has_S1;
  bool      _has_W1;

  word_t    S0w, S0ldw, S0rdw, S0lddw, S0rddw, S0l2dw, S0r2dw;
  postag_t  S0p, S0ldp, S0rdp, S0lddp, S0rddp, S0l2dp, S0r2dp;
  int       S0la, S0ra;
  int       S0ls, S0rs;

  word_t    S1w, S1ldw, S1rdw, S1lddw, S1rddw, S1l2dw, S1r2dw;
  postag_t  S1p, S1ldp, S1rdp, S1lddp, S1rddp, S1l2dp, S1r2dp;
  int       S1la, S1ra;
  int       S1ls, S1rs;

  word_t    W0, W1;
  postag_t  P0, P1;
};

}

}

#endif  //  end for __SR_CONTEXT_H__
