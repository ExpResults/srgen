#include "pipe.h"
#include "weight.h"
#include "context.h"

#include <boost/log/trivial.hpp>

namespace SR {

int Pipe::get_state_packed_score(const StateItem & item,
    const action_sequence_t & possible_actions,
    packed_score_t & scores) {

  scores.clear();

  Context ctx(item);

  for (int i = 0; i < possible_actions.size(); ++ i) {
    const action::action_t & act = possible_actions[i];

    // S0w
    scores[act] += get_score<us_map_t, us_t>(weight.S0w, us_t(ctx.S0w, act), true, 0);

    // S0p
    scores[act] += get_score<us_map_t, us_t>(weight.S0p, us_t(ctx.S0p, act), true, 0);

    // S0ldw
    scores[act] += get_score<us_map_t, us_t>(weight.S0ldw, us_t(ctx.S0ldw, act), true, 0);

    // S0ldp
    scores[act] += get_score<us_map_t, us_t>(weight.S0ldp, us_t(ctx.S0ldp, act), true, 0);

    // S0l2dw
    scores[act] += get_score<us_map_t, us_t>(weight.S0l2dw, us_t(ctx.S0l2dw, act), true, 0);

    // S0l2dp
    scores[act] += get_score<us_map_t, us_t>(weight.S0l2dp, us_t(ctx.S0l2dp, act), true, 0);

    // S0lddw
    scores[act] += get_score<us_map_t, us_t>(weight.S0lddw, us_t(ctx.S0lddw, act), true, 0);

    // S0lddp
    scores[act] += get_score<us_map_t, us_t>(weight.S0lddp, us_t(ctx.S0lddp, act), true, 0);

    // S0rdw
    scores[act] += get_score<us_map_t, us_t>(weight.S0rdw, us_t(ctx.S0rdw, act), true, 0);
  
    // S0rdp
    scores[act] += get_score<us_map_t, us_t>(weight.S0rdp, us_t(ctx.S0rdp, act), true, 0);

    // S0r2dw
    scores[act] += get_score<us_map_t, us_t>(weight.S0r2dw, us_t(ctx.S0r2dw, act), true, 0);

    // S0r2dp
    scores[act] += get_score<us_map_t, us_t>(weight.S0r2dp, us_t(ctx.S0r2dp, act), true, 0);

    // S0rddw
    scores[act] += get_score<us_map_t, us_t>(weight.S0rddw, us_t(ctx.S0rddw, act), true, 0);

    // S0rddp
    scores[act] += get_score<us_map_t, us_t>(weight.S0rddp, us_t(ctx.S0rddp, act), true, 0);
  }

  return 0;
}


int Pipe::update_state_score(const StateItem & item,
    const action::action_t & act, int now, int scale) {

  BOOST_LOG_TRIVIAL(trace) << "UPDATING: " << (void *)(&item) << ", " << act << " " << scale;

  Context ctx(item);

  // S0w
  update_score<us_map_t, us_t>(weight.S0w, us_t(ctx.S0w, act), now, scale);

  // S0p
  update_score<us_map_t, us_t>(weight.S0p, us_t(ctx.S0p, act), now, scale);

  // S0ldw
  update_score<us_map_t, us_t>(weight.S0ldw, us_t(ctx.S0ldw, act), now, scale);

  // S0ldp
  update_score<us_map_t, us_t>(weight.S0ldp, us_t(ctx.S0ldp, act), now, scale);

  // S0l2dw
  update_score<us_map_t, us_t>(weight.S0l2dw, us_t(ctx.S0l2dw, act), now, scale);

  // S0l2dp
  update_score<us_map_t, us_t>(weight.S0l2dp, us_t(ctx.S0l2dp, act), now, scale);

  // S0rdw
  update_score<us_map_t, us_t>(weight.S0rdw, us_t(ctx.S0rdw, act), now, scale);

  // S0rdp
  update_score<us_map_t, us_t>(weight.S0rdp, us_t(ctx.S0rdp, act), now, scale);

  // S0r2dw
  update_score<us_map_t, us_t>(weight.S0r2dw, us_t(ctx.S0r2dw, act), now, scale);

  // S0r2dp
  update_score<us_map_t, us_t>(weight.S0r2dp, us_t(ctx.S0r2dp, act), now, scale);

  // S0rddw
  update_score<us_map_t, us_t>(weight.S0rddw, us_t(ctx.S0rddw, act), now, scale);

  // S0rddp
  update_score<us_map_t, us_t>(weight.S0rddp, us_t(ctx.S0rddp, act), now, scale);
  return 1;
}

} //  end for namespace
