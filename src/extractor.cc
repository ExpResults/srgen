#include "pipe.h"
#include "weight.h"

#include <boost/log/trivial.hpp>

namespace SR {

int Pipe::get_state_packed_score(const StateItem & item,
    const action_sequence_t & possible_actions,
    packed_score_t & scores) {
  scores.clear();

  int S0 = item.stack_top();

  word_t S0w = item.sentence_ref->at(S0);
  postag_t S0p = item.postags[S0];

  for (int i = 0; i < possible_actions.size(); ++ i) {
    const action::action_t & act = possible_actions[i];

    // S0w
    scores[act] += get_score<us_map_t, us_t>(weight.S0w, us_t(S0w, act), true, 0);

    // S0p
    scores[act] += get_score<us_map_t, us_t>(weight.S0p, us_t(S0p, act), true, 0);

    // S0wS0p
    scores[act] += get_score<bs_map_t, bs_t>(weight.S0wS0p, bs_t(S0w, S0p, act), true, 0);
  }

  return 0;
}


int Pipe::update_state_score(const StateItem & item,
    const action::action_t & act, int now, int scale) {
  int S0 = item.stack_top();
  BOOST_LOG_TRIVIAL(trace) << "UPDATE " << (void *)(&item) << ", " << act << " " << scale;

  word_t S0w = item.sentence_ref->at(S0);
  postag_t S0p = item.postags[S0];

  //
  update_score<us_map_t, us_t>(weight.S0w, us_t(S0w, act), now, scale);

  //
  update_score<us_map_t, us_t>(weight.S0p, us_t(S0p, act), now, scale);

  //
  update_score<bs_map_t, bs_t>(weight.S0wS0p, bs_t(S0w, S0p, act), now, scale);
  return 1;
}

} //  end for namespace
