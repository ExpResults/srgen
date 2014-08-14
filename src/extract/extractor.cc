#include <boost/log/trivial.hpp>
#include "pipe/pipe.h"
#include "extract/context.h"
#include "extract/shortcut.h"

namespace ZGen {

namespace ShiftReduce {

int Pipe::get_state_packed_score(const StateItem & item,
    const action_sequence_t & possible_actions,
    packed_score_t & scores) {

  scores.clear();

  Context ctx(item);

  for (int i = 0; i < possible_actions.size(); ++ i) {
    const action::action_t & act = possible_actions[i];

    __GUS(S0w);      __GUS(S0p);
    __GUS(S0ldw);    __GUS(S0ldp);    __GUS(S0ldl);
    __GUS(S0l2dw);   __GUS(S0l2dp);   __GUS(S0l2dl);
    __GUS(S0lddw);   __GUS(S0lddp);   __GUS(S0lddl);
    __GUS(S0rdw);    __GUS(S0rdp);    __GUS(S0rdl);
    __GUS(S0r2dw);   __GUS(S0r2dp);   __GUS(S0r2dl);
    __GUS(S0rddw);   __GUS(S0rddp);   __GUS(S0rddl);

    // __GBS(S0r2dw, S0rdw); __GBS(S0r2dp, S0rdp); __GBS(S0r2dl, S0rdl);
    __GBS(S0ldw, S0l2dw); __GBS(S0ldp, S0l2dp); __GBS(S0ldl, S0l2dl);

    __GBS(S0w, S0la); __GBS(S0p, S0la);
    __GBS(S0w, S0ra); __GBS(S0p, S0ra);
    __GBS(S0w, S0ls); __GBS(S0p, S0ls);
    __GBS(S0w, S0rs); __GBS(S0p, S0rs);

    __GBS(S0w, S0ldw); __GBS(S0w, S0ldp);
    __GBS(S0p, S0ldw); __GBS(S0p, S0ldp);
    __GBS(S0w, S0rdw); __GBS(S0w, S0rdp);
    __GBS(S0p, S0rdw); __GBS(S0p, S0rdp);

    __GTS(S0w,S0p,S0ldw); __GTS(S0w,S0p,S0ldp); __GTS(S0w,S0ldw,S0ldp);  __GTS(S0p,S0ldw,S0ldp);
    __GTS(S0w,S0p,S0rdw); __GTS(S0w,S0p,S0rdp); __GTS(S0w,S0rdw,S0rdp);  __GTS(S0p,S0rdw,S0rdp);

    if (ctx.has_S1()) {
      __GUS(S1w);      __GUS(S1p);
      __GUS(S1ldw);    __GUS(S1ldp);    __GUS(S1ldl);
      __GUS(S1l2dw);   __GUS(S1l2dp);   __GUS(S1l2dl);
      __GUS(S1lddw);   __GUS(S1lddp);   __GUS(S1lddl);
      __GUS(S1rdw);    __GUS(S1rdp);    __GUS(S1rdl);
      __GUS(S1r2dw);   __GUS(S1r2dp);   __GUS(S1r2dl);
      __GUS(S1rddw);   __GUS(S1rddp);   __GUS(S1rddl);

      __GBS(S1r2dw, S1rdw);
      __GBS(S1r2dp, S1rdp);
      __GBS(S1r2dl, S1rdl);

      __GBS(S1w, S1la); __GBS(S1p, S1la);
      __GBS(S1w, S1ra); __GBS(S1p, S1ra);
      __GBS(S1w, S1ls); __GBS(S1p, S1ls);
      __GBS(S1w, S1rs); __GBS(S1p, S1rs);

      __GBS(S1w, S1ldw); __GBS(S1w, S1ldp);
      __GBS(S1p, S1ldw); __GBS(S1p, S1ldp);
      __GBS(S1w, S1rdw); __GBS(S1w, S1rdp);
      __GBS(S1p, S1rdw); __GBS(S1p, S1rdp);

      __GTS(S1w,S1p,S1ldw); __GTS(S1w,S1p,S1ldp); __GTS(S1w,S1ldw,S1ldp);  __GTS(S1p,S1ldw,S1ldp);
      __GTS(S1w,S1p,S1rdw); __GTS(S1w,S1p,S1rdp); __GTS(S1w,S1rdw,S1rdp);  __GTS(S1p,S1rdw,S1rdp);

      __GBS(S0w, S1w);  __GBS(S0p, S1p);
      __GBS(S0w, S1p);  __GBS(S0p, S1w);
    }

    __GUS(W0);          __GUS(P0);
    __GBS(W0, W1);      __GBS(P0, P1);
    __GTS(W0, W1, W2);  __GTS(P0, P1, P2);
  }

  return 0;
}


int Pipe::update_state_score(const StateItem & item,
    const action::action_t & act, int now, int scale) {

  BOOST_LOG_TRIVIAL(trace) << "UPDATING: " << (void *)(&item) << ", " << act << " " << scale;

  Context ctx(item);
  __UUS(S0w);      __UUS(S0p);
  __UUS(S0ldw);    __UUS(S0ldp);    __UUS(S0ldl);
  __UUS(S0l2dw);   __UUS(S0l2dp);   __UUS(S0l2dl);
  __UUS(S0lddw);   __UUS(S0lddp);   __UUS(S0lddl);
  __UUS(S0rdw);    __UUS(S0rdp);    __UUS(S0rdl);
  __UUS(S0r2dw);   __UUS(S0r2dp);   __UUS(S0r2dl);
  __UUS(S0rddw);   __UUS(S0rddp);   __UUS(S0rddl);

  // __UBS(S0r2dw, S0rdw); __UBS(S0r2dp, S0rdp); __UBS(S0r2dl, S0rdl);
  __UBS(S0ldw, S0l2dw); __UBS(S0ldp, S0l2dp); __UBS(S0ldl, S0l2dl);

  __UBS(S0w, S0la); __UBS(S0p, S0la);
  __UBS(S0w, S0ra); __UBS(S0p, S0ra);
  __UBS(S0w, S0ls); __UBS(S0p, S0ls);
  __UBS(S0w, S0rs); __UBS(S0p, S0rs);

  __UBS(S0w, S0ldw); __UBS(S0w, S0ldp);
  __UBS(S0p, S0ldw); __UBS(S0p, S0ldp);
  __UBS(S0w, S0rdw); __UBS(S0w, S0rdp);
  __UBS(S0p, S0rdw); __UBS(S0p, S0rdp);

  __UTS(S0w,S0p,S0ldw); __UTS(S0w,S0p,S0ldp); __UTS(S0w,S0ldw,S0ldp);  __UTS(S0p,S0ldw,S0ldp);
  __UTS(S0w,S0p,S0rdw); __UTS(S0w,S0p,S0rdp); __UTS(S0w,S0rdw,S0rdp);  __UTS(S0p,S0rdw,S0rdp);

  if (ctx.has_S1()) {
    __UUS(S1w);      __UUS(S1p);
    __UUS(S1ldw);    __UUS(S1ldp);    __UUS(S1ldl);
    __UUS(S1l2dw);   __UUS(S1l2dp);   __UUS(S1l2dl);
    __UUS(S1lddw);   __UUS(S1lddp);   __UUS(S1lddl);
    __UUS(S1rdw);    __UUS(S1rdp);    __UUS(S1rdl);
    __UUS(S1r2dw);   __UUS(S1r2dp);   __UUS(S1r2dl);
    __UUS(S1rddw);   __UUS(S1rddp);   __UUS(S1rddl);

    __UBS(S1r2dw, S1rdw);
    __UBS(S1r2dp, S1rdp);
    __UBS(S1r2dl, S1rdl);

    __UBS(S1w, S1la); __UBS(S1p, S1la);
    __UBS(S1w, S1ra); __UBS(S1p, S1ra);
    __UBS(S1w, S1ls); __UBS(S1p, S1ls);
    __UBS(S1w, S1rs); __UBS(S1p, S1rs);

    __UBS(S1w, S1ldw); __UBS(S1w, S1ldp);
    __UBS(S1p, S1ldw); __UBS(S1p, S1ldp);
    __UBS(S1w, S1rdw); __UBS(S1w, S1rdp);
    __UBS(S1p, S1rdw); __UBS(S1p, S1rdp);

    __UTS(S1w,S1p,S1ldw); __UTS(S1w,S1p,S1ldp); __UTS(S1w,S1ldw,S1ldp);  __UTS(S1p,S1ldw,S1ldp);
    __UTS(S1w,S1p,S1rdw); __UTS(S1w,S1p,S1rdp); __UTS(S1w,S1rdw,S1rdp);  __UTS(S1p,S1rdw,S1rdp);

    __UBS(S0w, S1w);  __UBS(S0p, S1p);
    __UBS(S0w, S1p);  __UBS(S0p, S1w);
  }

  __UUS(W0);          __UUS(P0);
  __UBS(W0, W1);      __UBS(P0, P1);
  __UTS(W0, W1, W2);  __UTS(P0, P1, P2);
  return 1;
}

}

} //  end for namespace
