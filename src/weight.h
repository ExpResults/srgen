#ifndef __SR_WEIGHT_H__
#define __SR_WEIGHT_H__

#include "settings.h"
#include "score_map.h"

namespace SR {

struct Weight {
  us_map_t    S0w,      S0p;
  us_map_t    S0ldw,    S0ldp;
  us_map_t    S0lddw,   S0lddp;
  us_map_t    S0l2dw,   S0l2dp;
  us_map_t    S0rdw,    S0rdp;
  us_map_t    S0rddw,   S0rddp;
  us_map_t    S0r2dw,   S0r2dp;

  bs_map_t    S0wS0la,  S0pS0la;
  bs_map_t    S0wS0ra,  S0pS0ra;
  // bs_map_t    S0wS0ls,  S0pS0ls;

  us_map_t    S1w,      S1p;
  us_map_t    S1ldw,    S1ldp;
  us_map_t    S1lddw,   S1lddp;
  us_map_t    S1l2dw,   S1l2dp;
  us_map_t    S1rdw,    S1rdp;
  us_map_t    S1rddw,   S1rddp;
  us_map_t    S1r2dw,   S1r2dp;

  bs_map_t    S1wS1la,  S1pS1la;
  bs_map_t    S1wS1ra,  S1pS1ra;
  // bs_map_t    S1wS1ls,  S1pS1ls;

  bs_map_t    S0wS1w,   S0pS1p;
  bs_map_t    S0wS1p,   S0pS1w;

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

#endif  //  end for __SR_WEIGHT_H__
