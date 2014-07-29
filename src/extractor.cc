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

    scores[act] += get_score<us_map_t, us_t>(weight.S0w, us_t(ctx.S0w, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0p, us_t(ctx.S0p, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0ldw, us_t(ctx.S0ldw, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0ldp, us_t(ctx.S0ldp, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0l2dw, us_t(ctx.S0l2dw, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0l2dp, us_t(ctx.S0l2dp, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0lddw, us_t(ctx.S0lddw, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0lddp, us_t(ctx.S0lddp, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0rdw, us_t(ctx.S0rdw, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0rdp, us_t(ctx.S0rdp, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0r2dw, us_t(ctx.S0r2dw, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0r2dp, us_t(ctx.S0r2dp, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0rddw, us_t(ctx.S0rddw, act), true, 0);
    scores[act] += get_score<us_map_t, us_t>(weight.S0rddp, us_t(ctx.S0rddp, act), true, 0);
    scores[act] += get_score<bs_map_t, bs_t>(weight.S0wS0la,
        bs_t(ctx.S0w, ctx.S0la, act), true, 0);
    scores[act] += get_score<bs_map_t, bs_t>(weight.S0pS0la,
        bs_t(ctx.S0p, ctx.S0la, act), true, 0);
    scores[act] += get_score<bs_map_t, bs_t>(weight.S0wS0ra,
        bs_t(ctx.S0w, ctx.S0ra, act), true, 0);
    scores[act] += get_score<bs_map_t, bs_t>(weight.S0pS0ra,
        bs_t(ctx.S0p, ctx.S0ra, act), true, 0);

    if (ctx.has_S1()) {
      scores[act] += get_score<us_map_t, us_t>(weight.S1w, us_t(ctx.S1w, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1p, us_t(ctx.S1p, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1ldw, us_t(ctx.S1ldw, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1ldp, us_t(ctx.S1ldp, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1l2dw, us_t(ctx.S1l2dw, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1l2dp, us_t(ctx.S1l2dp, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1lddw, us_t(ctx.S1lddw, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1lddp, us_t(ctx.S1lddp, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1rdw, us_t(ctx.S1rdw, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1rdp, us_t(ctx.S1rdp, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1r2dw, us_t(ctx.S1r2dw, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1r2dp, us_t(ctx.S1r2dp, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1rddw, us_t(ctx.S1rddw, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.S1rddp, us_t(ctx.S1rddp, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S1wS1la,
          bs_t(ctx.S1w, ctx.S1la, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S1pS1la,
          bs_t(ctx.S1p, ctx.S1la, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S1wS1ra,
          bs_t(ctx.S1w, ctx.S1ra, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S1pS1ra,
          bs_t(ctx.S1p, ctx.S1ra, act), true, 0);

      scores[act] += get_score<bs_map_t, bs_t>(weight.S0wS1w,
          bs_t(ctx.S0w, ctx.S1w, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S0pS1p,
          bs_t(ctx.S0p, ctx.S1p, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S0wS1p,
          bs_t(ctx.S0w, ctx.S1p, act), true, 0);
      scores[act] += get_score<bs_map_t, bs_t>(weight.S0pS1w,
          bs_t(ctx.S0p, ctx.S1w, act), true, 0);
    }
  }

  return 0;
}


int Pipe::update_state_score(const StateItem & item,
    const action::action_t & act, int now, int scale) {

  BOOST_LOG_TRIVIAL(trace) << "UPDATING: " << (void *)(&item) << ", " << act << " " << scale;

  Context ctx(item);

  update_score<us_map_t, us_t>(weight.S0w, us_t(ctx.S0w, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0p, us_t(ctx.S0p, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0ldw, us_t(ctx.S0ldw, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0ldp, us_t(ctx.S0ldp, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0l2dw, us_t(ctx.S0l2dw, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0l2dp, us_t(ctx.S0l2dp, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0rdw, us_t(ctx.S0rdw, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0rdp, us_t(ctx.S0rdp, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0r2dw, us_t(ctx.S0r2dw, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0r2dp, us_t(ctx.S0r2dp, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0rddw, us_t(ctx.S0rddw, act), now, scale);
  update_score<us_map_t, us_t>(weight.S0rddp, us_t(ctx.S0rddp, act), now, scale);
  update_score<bs_map_t, bs_t>(weight.S0wS0la, bs_t(ctx.S0w, ctx.S0la, act), now, scale);
  update_score<bs_map_t, bs_t>(weight.S0pS0la, bs_t(ctx.S0p, ctx.S0la, act), now, scale);
  update_score<bs_map_t, bs_t>(weight.S0wS0ra, bs_t(ctx.S0w, ctx.S0ra, act), now, scale);
  update_score<bs_map_t, bs_t>(weight.S0pS0ra, bs_t(ctx.S0p, ctx.S0ra, act), now, scale);

  if (ctx.has_S1()) {
    update_score<us_map_t, us_t>(weight.S1w, us_t(ctx.S1w, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1p, us_t(ctx.S1p, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1ldw, us_t(ctx.S1ldw, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1ldp, us_t(ctx.S1ldp, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1l2dw, us_t(ctx.S1l2dw, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1l2dp, us_t(ctx.S1l2dp, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1lddw, us_t(ctx.S1lddw, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1lddp, us_t(ctx.S1lddp, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1rdw, us_t(ctx.S1rdw, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1rdp, us_t(ctx.S1rdp, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1r2dw, us_t(ctx.S1r2dw, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1r2dp, us_t(ctx.S1r2dp, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1rddw, us_t(ctx.S1rddw, act), now, scale);
    update_score<us_map_t, us_t>(weight.S1rddp, us_t(ctx.S1rddp, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S1wS1la, bs_t(ctx.S1w, ctx.S1la, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S1pS1la, bs_t(ctx.S1p, ctx.S1la, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S1wS1ra, bs_t(ctx.S1w, ctx.S1ra, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S1pS1ra, bs_t(ctx.S1p, ctx.S1ra, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S0wS1w, bs_t(ctx.S0w, ctx.S1w, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S0pS1p, bs_t(ctx.S0p, ctx.S1p, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S0wS1p, bs_t(ctx.S0w, ctx.S1p, act), now, scale);
    update_score<bs_map_t, bs_t>(weight.S0pS1w, bs_t(ctx.S0p, ctx.S1w, act), now, scale);
  }
  return 1;
}

} //  end for namespace
