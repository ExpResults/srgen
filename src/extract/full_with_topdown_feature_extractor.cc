#include "pipe/full_with_topdown_feature.h"

namespace ZGen {

namespace ShiftReduce {

int FullWithTopDownFeaturePipe::get_state_packed_score(const StateItem& item,
    const action_sequence_t& possible_actions,
    packed_score_t& scores) {
  Pipe::get_state_packed_score(item, possible_actions, scores);

  for (int a = 0; a < possible_actions.size(); ++ a) {
    const action::action_t& act = possible_actions[a];

    double to_root_min_left_prob = 1., to_root_max_right_prob = 0.;
    double to_child_min_right_prob = 1., to_child_max_left_prob = 0.;

    if (action::is_shift(act)) {
      int j = act.index;
      // illegal left arc
      for (int i = j; tree.head(i) != -1; i = tree.head(i)) {
        int h = tree.head(j);
        double left_prob = directions[h].second;
        double right_prob = directions[h].first;

        if (item.buffer.test(h)) {
          to_root_min_left_prob = std::min(to_root_min_left_prob, left_prob);
          to_root_max_right_prob = std::max(to_root_max_right_prob, right_prob);
        }
      }

      // illegal right arc
      const DependencyTree::edgeset_t& children = tree.children(j);
      for (int i = 0; i < children.size(); ++ i) {
        int c = children[i];
        double left_prob = directions[c].second;
        double right_prob = directions[c].first;

        if (item.buffer.test(c)) {
          to_child_min_right_prob = std::min(to_child_min_right_prob, right_prob);
          to_child_max_left_prob = std::max(to_child_max_left_prob, left_prob);
        }
      }

      int histed_to_root_min_left_prob = hist(to_root_min_left_prob);
      int histed_to_root_max_right_prob = hist(to_root_max_right_prob);
      int histed_to_child_min_right_prob = hist(to_child_min_right_prob);
      int histed_to_child_max_left_prob = hist(to_child_max_left_prob);

      scores[act] += get_score<us_map_t, us_t>(weight.ToRootMinLeftProb, \
          us_t(histed_to_root_min_left_prob, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.ToRootMaxRightProb, \
          us_t(histed_to_root_max_right_prob, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.ToChildMinRightProb, \
          us_t(histed_to_child_min_right_prob, act), true, 0);
      scores[act] += get_score<us_map_t, us_t>(weight.ToChildMaxLeftProb, \
          us_t(histed_to_child_max_left_prob, act), true, 0);
    }
  }
  return 0;
}

int FullWithTopDownFeaturePipe::update_state_score(const StateItem& item,
    const action::action_t& act, int now, int scale) {
  Pipe::update_state_score(item, act, now, scale);

  double to_root_min_left_prob = 1., to_root_max_right_prob = 0.;
  double to_child_min_right_prob = 1., to_child_max_left_prob = 0.;

  if (action::is_shift(act)) {
    int j = act.index;
    // illegal left arc
    for (int i = j; tree.head(i) != -1; i = tree.head(i)) {
      int h = tree.head(j);
      double left_prob = directions[h].second;
      double right_prob = directions[h].first;
      
      if (item.buffer.test(h)) {
        to_root_min_left_prob = std::min(to_root_min_left_prob, left_prob);
        to_root_max_right_prob = std::max(to_root_max_right_prob, right_prob);
      }
    }

    // illegal right arc
    const DependencyTree::edgeset_t& children = tree.children(j);
    for (int i = 0; i < children.size(); ++ i) {
      int c = children[i];
      double left_prob = directions[c].second;
      double right_prob = directions[c].first;

      if (item.buffer.test(c)) {
        to_child_min_right_prob = std::min(to_child_min_right_prob, right_prob);
        to_child_max_left_prob = std::max(to_child_max_left_prob, left_prob);
      }
    }

    int histed_to_root_min_left_prob = hist(to_root_min_left_prob);
    int histed_to_root_max_right_prob = hist(to_root_max_right_prob);
    int histed_to_child_min_right_prob = hist(to_child_min_right_prob);
    int histed_to_child_max_left_prob = hist(to_child_max_left_prob);

    update_score<us_map_t, us_t>(weight.ToRootMinLeftProb, \
        us_t(histed_to_root_min_left_prob, act), now, scale);
    update_score<us_map_t, us_t>(weight.ToRootMaxRightProb, \
        us_t(histed_to_root_max_right_prob, act), now, scale);
    update_score<us_map_t, us_t>(weight.ToChildMinRightProb, \
        us_t(histed_to_child_min_right_prob, act), now, scale);
    update_score<us_map_t, us_t>(weight.ToChildMaxLeftProb, \
        us_t(histed_to_child_max_left_prob, act), now, scale);
  }
  return 0;
}


} //  end for namespace ShiftReduce

} //  end for namespace ZGen
