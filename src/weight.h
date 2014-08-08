#ifndef __SR_WEIGHT_H__
#define __SR_WEIGHT_H__

#include "settings.h"
#include "score_map.h"

namespace ZGen {

namespace ShiftReduce {

struct Weight {
  us_map_t    S0w,      S0p;
  us_map_t    S0ldw,    S0ldp,    S0ldl;
  us_map_t    S0lddw,   S0lddp,   S0lddl;
  us_map_t    S0l2dw,   S0l2dp,   S0l2dl;
  us_map_t    S0rdw,    S0rdp,    S0rdl;
  us_map_t    S0rddw,   S0rddp,   S0rddl;
  us_map_t    S0r2dw,   S0r2dp,   S0r2dl;

  bs_map_t    S0r2dwS0rdw, S0r2dpS0rdp, S0r2dlS0rdl;
  bs_map_t    S0ldwS0l2dw, S0ldpS0l2dp, S0ldlS0l2dl;

  bs_map_t    S0wS0la,  S0pS0la;
  bs_map_t    S0wS0ra,  S0pS0ra;
  bs_map_t    S0wS0ls,  S0pS0ls;
  bs_map_t    S0wS0rs,  S0pS0rs;

  us_map_t    S1w,      S1p;
  us_map_t    S1ldw,    S1ldp,    S1ldl;
  us_map_t    S1lddw,   S1lddp,   S1lddl;
  us_map_t    S1l2dw,   S1l2dp,   S1l2dl;
  us_map_t    S1rdw,    S1rdp,    S1rdl;
  us_map_t    S1rddw,   S1rddp,   S1rddl;
  us_map_t    S1r2dw,   S1r2dp,   S1r2dl;

  bs_map_t    S1wS1la,  S1pS1la;
  bs_map_t    S1wS1ra,  S1pS1ra;
  bs_map_t    S1wS1ls,  S1pS1ls;
  bs_map_t    S1wS1rs,  S1pS1rs;

  bs_map_t    S1r2dwS1rdw, S1r2dpS1rdp, S1r2dlS1rdl;

  bs_map_t    S0wS1w,   S0pS1p;
  bs_map_t    S0wS1p,   S0pS1w;

  us_map_t    W0, P0;
  bs_map_t    W0W1, P0P1;
  ts_map_t    W0W1W2, P0P1P2;

  bool flush_weight(int now);

  //
  bool save_weight(const char * filename);

  //
  bool load_weight(const char * filename);

};

typedef Weight  weight_t;


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
