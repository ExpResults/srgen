#include <iostream>

#include "types/action.h"
#include "types/engine.h"

#include <boost/log/trivial.hpp>

namespace ZGen {

namespace ShiftReduce {

namespace action {

bool is_shift(const action_t & act) {
  return (act.prefix == ActionEncoderAndDecoder::SH);
}


bool is_left_arc(const action_t & act) {
  return (act.prefix == ActionEncoderAndDecoder::LA);
}


bool is_right_arc(const action_t & act) {
  return (act.prefix == ActionEncoderAndDecoder::RA);
}


int get_correct_actions(const dependency_t & oracle,
    std::vector<action_t> & actions) {
  // Extract ordered-tree structure from the oracle parse.
  int N = oracle.forms.size();
  int root = -1;
  std::vector<std::vector<int> > tree(N);

  actions.clear();

  for (int i = 0; i < N; ++ i) {
    int head = oracle.heads[i];
    if (head == -1) {
      root = i;
    } else {
      tree[head].push_back(i);
    }
  }

  get_correct_actions_travel(root, oracle, tree, actions);
  return (int)actions.size();
}


int get_correct_actions_travel(int root,
    const dependency_t & parse,
    const std::vector<std::vector<int> > & tree,
    std::vector<action_t> & actions) {

  const std::vector<int> & children = tree[root];

  int i;
  for (i = 0; i < children.size() && children[i] < root; ++ i) {
    get_correct_actions_travel(children[i], parse, tree, actions);
  }

  actions.push_back( action_t(
        ActionEncoderAndDecoder::SH,
        parse.postags[root],
        parse.forms[root],
        root) );

  for (int j = i; j < children.size(); ++ j) {
    int child = children[j];
    get_correct_actions_travel(child, parse, tree, actions);
    actions.push_back( action_t(
          ActionEncoderAndDecoder::RA, parse.deprels[child], 0) );
  }

  for (int j = i - 1; j >= 0; -- j) {
    int child = children[j];
    actions.push_back( action_t(
         ActionEncoderAndDecoder::LA, parse.deprels[child], 0) );
  }

  return 0;
}

} //  end for namespace action

}

}
