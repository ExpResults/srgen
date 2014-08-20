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

    __GUS_EXT(S0);                                          // 1
    __GBS_MATRIX_2_2(S0w, S0p, S0ldw, S0ldp/*, S0ldl*/);        // 2
    __GBS_MATRIX_2_2(S0w, S0p, S0rdw, S0rdp/*, S0rdl*/);        // |
    __GTS_MATRIX_2_2_1(S0w, S0p, S0ldw, S0ldp, S0S0ldDist);
    __GTS_MATRIX_2_2_1(S0w, S0p, S0rdw, S0rdp, S0S0rdDist);
    __GTS_CIRCLE_4(S0w, S0p, S0ldw, S0ldp);                 // 3
    __GTS_CIRCLE_4(S0w, S0p, S0rdw, S0rdp);                 // |
    __GBS_MATRIX_2_2(S0w, S0p, S0la, S0ra);                 // 4
    __GBS_MATRIX_2_2(S0w, S0p, S0ls, S0rs);                 // |
    __GBS_EXT(S0ld, S0l2d);                                 // 5
    if (ctx.has_S1()) {
      __GUS_EXT(S1);                                        // 6
      __GBS_MATRIX_2_2(S1w, S1p, S1ldw, S1ldp/*, S1ldl*/);      // 7
      __GBS_MATRIX_2_2(S1w, S1p, S1rdw, S1rdp/*, S1rdl*/);      // |
      __GTS_MATRIX_2_2_1(S1w, S1p, S1ldw, S1ldp, S1S1ldDist);
      __GTS_MATRIX_2_2_1(S1w, S1p, S1rdw, S1rdp, S1S1rdDist);
      __GTS_CIRCLE_4(S1w, S1p, S1ldw, S1ldp);               // 8
      __GTS_CIRCLE_4(S1w, S1p, S1rdw, S1rdp);               // |
      __GBS_EXT(S1r2d, S1rd);                               // 9
      __GBS_MATRIX_2_2(S1w, S1p, S1la, S1ra);               // 10
      __GBS_MATRIX_2_2(S1w, S1p, S1ls, S1rs);               // |
      __GBS_MATRIX_2_2(S0w, S0p, S1w, S1p);                 // 11
      __GTS_CIRCLE_4(S0w, S0p, S1w, S1p);                   // 12
      __GTS_MATRIX_2_2_1(S0w, S0p, S1w, S1p, S0S1Dist);     // 13
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

  __UUS_EXT(S0);                                            // 1
  __UBS_MATRIX_2_2(S0w, S0p, S0ldw, S0ldp/*, S0ldl*/);          // 2
  __UBS_MATRIX_2_2(S0w, S0p, S0rdw, S0rdp/*, S0rdl*/);          // |
  __UTS_MATRIX_2_2_1(S0w, S0p, S0ldw, S0ldp, S0S0ldDist);
  __UTS_MATRIX_2_2_1(S0w, S0p, S0rdw, S0rdp, S0S0rdDist);
  __UTS_CIRCLE_4(S0w, S0p, S0ldw, S0ldp);                   // 3
  __UTS_CIRCLE_4(S0w, S0p, S0rdw, S0rdp);                   // |
  __UBS_MATRIX_2_2(S0w, S0p, S0la, S0ra);                   // 4
  __UBS_MATRIX_2_2(S0w, S0p, S0ls, S0rs);                   // |
  __UBS_EXT(S0ld, S0l2d);                                   // 5
  if (ctx.has_S1()) {
    __UUS_EXT(S1);                                          // 6
    __UBS_MATRIX_2_2(S1w, S1p, S1ldw, S1ldp/*, S1ldl*/);        // 7
    __UBS_MATRIX_2_2(S1w, S1p, S1rdw, S1rdp/*, S1rdl*/);        // |
    __UTS_MATRIX_2_2_1(S1w, S1p, S1ldw, S1ldp, S1S1ldDist);
    __UTS_MATRIX_2_2_1(S1w, S1p, S1rdw, S1rdp, S1S1rdDist);
    __UTS_CIRCLE_4(S1w, S1p, S1ldw, S1ldp);                 // 8
    __UTS_CIRCLE_4(S1w, S1p, S1rdw, S1rdp);                 // |
    __UBS_MATRIX_2_2(S1w, S1p, S1la, S1ra);                 // 9
    __UBS_MATRIX_2_2(S1w, S1p, S1ls, S1rs);                 // |
    __UBS_EXT(S1r2d, S1rd);                                 // 10
    __UBS_MATRIX_2_2(S0w, S0p, S1w, S1p);                   // 11
    __UTS_CIRCLE_4(S0w, S0p, S1w, S1p);                     // 12
    __UTS_MATRIX_2_2_1(S0w, S0p, S1w, S1p, S0S1Dist);       // 13
  }

  __UUS(W0);          __UUS(P0);
  __UBS(W0, W1);      __UBS(P0, P1);
  __UTS(W0, W1, W2);  __UTS(P0, P1, P2);
  return 1;
}

}

} //  end for namespace
