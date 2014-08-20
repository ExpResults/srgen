#ifndef __SR_WEIGHT_H__
#define __SR_WEIGHT_H__

#include "types/settings.h"
#include "types/score_map.h"
#include "model/shortcut.h"

namespace ZGen {

namespace ShiftReduce {

struct BasicWeight {
  __STATE_U_EXT(S0);                                  //1 - S0 unigram
  __STATE_B_MATRIX_2_3(S0w, S0p, S0ldw, S0ldp, S0ldl);//2 - S0 modifiers' bigram
  __STATE_B_MATRIX_2_3(S0w, S0p, S0rdw, S0rdp, S0rdl);//|
  __STATE_T_MATRIX_2_2_1(S0w, S0p, S0ldw, S0ldp, S0S0ldDist);
  __STATE_T_MATRIX_2_2_1(S0w, S0p, S0rdw, S0rdp, S0S0rdDist);
  __STATE_T_CIRCLE_4(S0w, S0p, S0ldw, S0ldp);         //3 - S0 modifiers' trigram
  __STATE_T_CIRCLE_4(S0w, S0p, S0rdw, S0rdp);         //|
  __STATE_B_MATRIX_2_2(S0w, S0p, S0la, S0ra);         //4 - S0 modifiers' count
  __STATE_B_MATRIX_2_2(S0w, S0p, S0ls, S0rs);         //|
  __STATE_B_EXT(S0ld, S0l2d);                         //5 - S0 modifiers' dependency order
  __STATE_U_EXT(S1);                                  //6 - S1 unigram
  __STATE_B_MATRIX_2_3(S1w, S1p, S1ldw, S1ldp, S1ldl);//7 - S1 modifiers' bigram
  __STATE_B_MATRIX_2_3(S1w, S1p, S1rdw, S1rdp, S1rdl);//|
  __STATE_T_MATRIX_2_2_1(S1w, S1p, S1ldw, S1ldp, S1S1ldDist);
  __STATE_T_MATRIX_2_2_1(S1w, S1p, S1rdw, S1rdp, S1S1rdDist);
  __STATE_T_CIRCLE_4(S1w, S1p, S1ldw, S1ldp);         //8 - S1 modifiers' trigram
  __STATE_T_CIRCLE_4(S1w, S1p, S1rdw, S1rdp);         //|
  __STATE_B_MATRIX_2_2(S1w, S1p, S1la, S1ra);         //9 - S1 modifiers' count
  __STATE_B_MATRIX_2_2(S1w, S1p, S1ls, S1rs);         //|
  __STATE_B_EXT(S1r2d, S1rd);                         //10 - S1 modifiers' dependency order
  __STATE_B_MATRIX_2_2(S0w, S0p, S1w, S1p);           //11 - S0, S1's bigram
  __STATE_T_CIRCLE_4(S0w, S0p, S1w, S1p);             //12 - S0, S1's trigram
  __STATE_T_MATRIX_2_2_1(S0w, S0p, S1w, S1p,S0S1Dist);//13 - S0, S1, distance trigram

  // Linearization feature
  us_map_t    W0, P0;
  bs_map_t    W0W1, P0P1;
  ts_map_t    W0W1W2, P0P1P2;

  // High level guidance feature.
  us_map_t    S0lvl0, S0lvl1, S0lvl2;
  us_map_t    S1lvl0, S1lvl1, S1lvl2;

  //
  bool flush_weight(int now);

  //
  bool save_weight(const char * filename);

  //
  bool load_weight(const char * filename);

};

typedef BasicWeight  weight_t;


template<typename Mapped, typename Entry>
floatval_t get_score(const Mapped & mapped, const Entry & entry,
    bool avg, floatval_t o = 0.) {

  typename Mapped::const_iterator itx = mapped.find(entry);

  if (itx == mapped.end()) {
    return o;
  }

  if (avg) {
    return itx->second.w_sum;
  } else {
    return itx->second.w;
  }
}


template<typename Mapped, typename Entry>
void update_score(Mapped & mapped, const Entry & entry,
    int now, floatval_t scale = 1.) {
  param_t & param = mapped[entry];
  param.add(now, scale);
}


template<typename Mapped, typename Entry>
void flush_score(Mapped & mapped, const Entry & entry, int now) {
  param_t & param = mapped[entry];

  param.flush(now);
}

}

}

#endif  //  end for __SR_WEIGHT_H__
