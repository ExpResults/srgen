#include "pipe.h"
#include "weight.h"
#include "context.h"

#include <boost/log/trivial.hpp>

#define __GET_UNISCORE(name) do { \
  if (ctx.name) { \
    scores[act] += get_score<us_map_t, us_t>(weight.name, \
        us_t(ctx.name, act), true, 0); \
  } \
} while (0);

#define __GET_BISCORE(name1, name2) do { \
  if (ctx.name1 && ctx.name2) { \
    scores[act] += get_score<bs_map_t, bs_t>(weight.name1##name2, \
        bs_t(ctx.name1, ctx.name2, act), true, 0); \
  } \
} while (0);

#define __GET_TRISCORE(name1, name2, name3) do { \
  if (ctx.name1 && ctx.name2 && ctx.name3) { \
    scores[act] += get_score<ts_map_t, ts_t>(weight.name1##name2##name3, \
        ts_t(ctx.name1, ctx.name2, ctx.name3, act), true, 0); \
  } \
} while (0);

#define __UPDATE_UNISCORE(name) do { \
  if (ctx.name) { \
    update_score<us_map_t, us_t>(weight.name, \
        us_t(ctx.name, act), now, scale); \
  } \
} while (0);

#define __UPDATE_BISCORE(name1, name2) do { \
  if (ctx.name1 && ctx.name2) { \
    update_score<bs_map_t, bs_t>(weight.name1##name2, \
        bs_t(ctx.name1, ctx.name2, act), now, scale); \
  } \
} while (0);

#define __UPDATE_TRISCORE(name1, name2, name3) do { \
  if (ctx.name1 && ctx.name2 && ctx.name3) { \
    update_score<ts_map_t, ts_t>(weight.name1##name2##name3, \
        ts_t(ctx.name1, ctx.name2, ctx.name3, act), now, scale); \
  } \
} while (0);

namespace ZGen {

namespace ShiftReduce {

int Pipe::get_state_packed_score(const StateItem & item,
    const action_sequence_t & possible_actions,
    packed_score_t & scores) {

  scores.clear();

  Context ctx(item);

  for (int i = 0; i < possible_actions.size(); ++ i) {
    const action::action_t & act = possible_actions[i];

    // scores[act] += get_score<us_map_t, us_t>(weight.S0w, us_t(ctx.S0w, act), true, 0);
    __GET_UNISCORE(S0w);      __GET_UNISCORE(S0p);
    __GET_UNISCORE(S0ldw);    __GET_UNISCORE(S0ldp);    __GET_UNISCORE(S0ldl);
    __GET_UNISCORE(S0l2dw);   __GET_UNISCORE(S0l2dp);   __GET_UNISCORE(S0l2dl);
    __GET_UNISCORE(S0lddw);   __GET_UNISCORE(S0lddp);   __GET_UNISCORE(S0lddl);
    __GET_UNISCORE(S0rdw);    __GET_UNISCORE(S0rdp);    __GET_UNISCORE(S0rdl);
    __GET_UNISCORE(S0r2dw);   __GET_UNISCORE(S0r2dp);   __GET_UNISCORE(S0r2dl);
    __GET_UNISCORE(S0rddw);   __GET_UNISCORE(S0rddp);   __GET_UNISCORE(S0rddl);

    __GET_BISCORE(S0r2dw, S0rdw);
    __GET_BISCORE(S0r2dp, S0rdp);
    __GET_BISCORE(S0r2dl, S0rdl);

    __GET_BISCORE(S0ldw, S0l2dw);
    __GET_BISCORE(S0ldp, S0l2dp);
    __GET_BISCORE(S0ldl, S0l2dl);

    __GET_BISCORE(S0w, S0la); __GET_BISCORE(S0p, S0la);
    __GET_BISCORE(S0w, S0ra); __GET_BISCORE(S0p, S0ra);
    __GET_BISCORE(S0w, S0ls); __GET_BISCORE(S0p, S0ls);
    __GET_BISCORE(S0w, S0rs); __GET_BISCORE(S0p, S0rs);

    if (ctx.has_S1()) {
      __GET_UNISCORE(S1w);      __GET_UNISCORE(S1p);
      __GET_UNISCORE(S1ldw);    __GET_UNISCORE(S1ldp);    __GET_UNISCORE(S1ldl);
      __GET_UNISCORE(S1l2dw);   __GET_UNISCORE(S1l2dp);   __GET_UNISCORE(S1l2dl);
      __GET_UNISCORE(S1lddw);   __GET_UNISCORE(S1lddp);   __GET_UNISCORE(S1lddl);
      __GET_UNISCORE(S1rdw);    __GET_UNISCORE(S1rdp);    __GET_UNISCORE(S1rdl);
      __GET_UNISCORE(S1r2dw);   __GET_UNISCORE(S1r2dp);   __GET_UNISCORE(S1r2dl);
      __GET_UNISCORE(S1rddw);   __GET_UNISCORE(S1rddp);   __GET_UNISCORE(S1rddl);

      __GET_BISCORE(S1r2dw, S1rdw);
      __GET_BISCORE(S1r2dp, S1rdp);
      __GET_BISCORE(S1r2dl, S1rdl);

      __GET_BISCORE(S1w, S1la); __GET_BISCORE(S1p, S1la);
      __GET_BISCORE(S1w, S1ra); __GET_BISCORE(S1p, S1ra);
      __GET_BISCORE(S1w, S1ls); __GET_BISCORE(S1p, S1ls);
      __GET_BISCORE(S1w, S1rs); __GET_BISCORE(S1p, S1rs);

      __GET_BISCORE(S0w, S1w);  __GET_BISCORE(S0p, S1p);
      __GET_BISCORE(S0w, S1p);  __GET_BISCORE(S0p, S1w);
    }

    __GET_UNISCORE(W0);         __GET_UNISCORE(P0);
    __GET_BISCORE(W0, W1);      __GET_BISCORE(P0, P1);
    __GET_TRISCORE(W0, W1, W2); __GET_TRISCORE(P0, P1, P2);
  }

  return 0;
}


int Pipe::update_state_score(const StateItem & item,
    const action::action_t & act, int now, int scale) {

  BOOST_LOG_TRIVIAL(trace) << "UPDATING: " << (void *)(&item) << ", " << act << " " << scale;

  Context ctx(item);

  __UPDATE_UNISCORE(S0w);       __UPDATE_UNISCORE(S0p);
  __UPDATE_UNISCORE(S0ldw);     __UPDATE_UNISCORE(S0ldp);     __UPDATE_UNISCORE(S0ldl);
  __UPDATE_UNISCORE(S0l2dw);    __UPDATE_UNISCORE(S0l2dp);    __UPDATE_UNISCORE(S0l2dl);
  __UPDATE_UNISCORE(S0lddw);    __UPDATE_UNISCORE(S0lddp);    __UPDATE_UNISCORE(S0lddl);
  __UPDATE_UNISCORE(S0rdw);     __UPDATE_UNISCORE(S0rdp);     __UPDATE_UNISCORE(S0rdl);
  __UPDATE_UNISCORE(S0r2dw);    __UPDATE_UNISCORE(S0r2dp);    __UPDATE_UNISCORE(S0r2dl);
  __UPDATE_UNISCORE(S0rddw);    __UPDATE_UNISCORE(S0rddp);    __UPDATE_UNISCORE(S0rddl);

  __UPDATE_BISCORE(S0r2dw, S0rdw);
  __UPDATE_BISCORE(S0r2dp, S0rdp);
  __UPDATE_BISCORE(S0r2dl, S0rdl);

  __UPDATE_BISCORE(S0ldw, S0l2dw);
  __UPDATE_BISCORE(S0ldp, S0l2dp);
  __UPDATE_BISCORE(S0ldl, S0l2dl);

  __UPDATE_BISCORE(S0w, S0la);  __UPDATE_BISCORE(S0p, S0la);
  __UPDATE_BISCORE(S0w, S0ra);  __UPDATE_BISCORE(S0p, S0ra);
  __UPDATE_BISCORE(S0w, S0ls);  __UPDATE_BISCORE(S0p, S0ls);
  __UPDATE_BISCORE(S0w, S0rs);  __UPDATE_BISCORE(S0p, S0rs);

  if (ctx.has_S1()) {
    __UPDATE_UNISCORE(S1w);       __UPDATE_UNISCORE(S1p);
    __UPDATE_UNISCORE(S1ldw);     __UPDATE_UNISCORE(S1ldp);   __UPDATE_UNISCORE(S1ldl);
    __UPDATE_UNISCORE(S1l2dw);    __UPDATE_UNISCORE(S1l2dp);  __UPDATE_UNISCORE(S1l2dl);
    __UPDATE_UNISCORE(S1lddw);    __UPDATE_UNISCORE(S1lddp);  __UPDATE_UNISCORE(S1lddl);
    __UPDATE_UNISCORE(S1rdw);     __UPDATE_UNISCORE(S1rdp);   __UPDATE_UNISCORE(S1rdl);
    __UPDATE_UNISCORE(S1r2dw);    __UPDATE_UNISCORE(S1r2dp);  __UPDATE_UNISCORE(S1r2dl);
    __UPDATE_UNISCORE(S1rddw);    __UPDATE_UNISCORE(S1rddp);  __UPDATE_UNISCORE(S1rddl);

    __UPDATE_BISCORE(S1r2dw, S1rdw);
    __UPDATE_BISCORE(S1r2dp, S1rdp);
    __UPDATE_BISCORE(S1r2dl, S1rdl);

    __UPDATE_BISCORE(S1w, S1la);  __UPDATE_BISCORE(S1p, S1la);
    __UPDATE_BISCORE(S1w, S1ra);  __UPDATE_BISCORE(S1p, S1ra);
    __UPDATE_BISCORE(S1w, S1ls);  __UPDATE_BISCORE(S1p, S1ls);
    __UPDATE_BISCORE(S1w, S1rs);  __UPDATE_BISCORE(S1p, S1rs);

    __UPDATE_BISCORE(S0w, S1w);   __UPDATE_BISCORE(S0p, S1p);
    __UPDATE_BISCORE(S0w, S1p);   __UPDATE_BISCORE(S0p, S1w);
    //
  }

  __UPDATE_UNISCORE(W0);          __UPDATE_UNISCORE(P0);
  __UPDATE_BISCORE(W0, W1);       __UPDATE_BISCORE(P0, P1);
  __UPDATE_TRISCORE(W0, W1, W2);  __UPDATE_TRISCORE(P0, P1, P2);
  return 1;
}

}

} //  end for namespace
