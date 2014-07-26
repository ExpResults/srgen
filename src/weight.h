#ifndef __SR_WEIGHT_H__
#define __SR_WEIGHT_H__

#include "settings.h"
#include "score_map.h"

namespace SR {

// Typedef
typedef UnigramScore    us_t;
typedef BigramScore     bs_t;
typedef TrigramScore    ts_t;

typedef UnigramScoreMap us_map_t;
typedef BigramScoreMap  bs_map_t;
typedef TrigramScoreMap ts_map_t;


struct Weight {
  us_map_t    S0w;
  us_map_t    S0p;
  bs_map_t    S0wS0p;
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

  int elapsed = now - param.w_time;
  floatval_t upd = scale;
  floatval_t cur_val = param.w;

  param.w = cur_val + upd;
  param.w_sum += elapsed * cur_val + upd;
  param.w_time = now;
}

}

#endif  //  end for __SR_WEIGHT_H__
