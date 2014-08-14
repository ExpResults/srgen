#include "pipe/full_with_guidance.h"
#include "extract/shortcut.h"

#define __INIT_LVL(prefix) int prefix##lvl0 = 0, prefix##lvl1 = 0, prefix##lvl2 = 0;

#define __SET_LVL(prefix) do { \
  prefix##lvl0 = tree_with_guidance.lvl0(prefix); \
  prefix##lvl1 = tree_with_guidance.lvl1(prefix); \
  prefix##lvl2 = tree_with_guidance.lvl2(prefix); \
} while (0);

namespace ZGen {

namespace ShiftReduce {

int FullWithGuidancePipe::get_state_packed_score(const StateItem& item,
    const action_sequence_t& possible_actions,
    packed_score_t& scores) {
  Pipe::get_state_packed_score(item, possible_actions, scores);

  __INIT_LVL(S0);
  __INIT_LVL(S1);

  int S0 = item.stack_top();
  if (S0 >= 0) {
    __SET_LVL(S0);
  }

  int S1 = item.stack.size() > 2 ? item.stack[item.stack.size()- 2] : -1;
  if (S1 >= 0) {
    __SET_LVL(S1);
  }

  for (int i = 0; i < possible_actions.size(); ++ i) {
    const action::action_t& act = possible_actions[i];
    __NC_GUS(S0lvl0); __NC_GUS(S0lvl1); __NC_GUS(S0lvl2);
    __NC_GUS(S1lvl1); __NC_GUS(S1lvl1); __NC_GUS(S1lvl2);
  }

  return 0;
}

int FullWithGuidancePipe::update_state_score(const StateItem& item,
    const action::action_t& act, int now, int scale) {
  Pipe::update_state_score(item, act, now, scale);

  __INIT_LVL(S0);
  __INIT_LVL(S1);

  int S0 = item.stack_top();
  if (S0 >= 0) {
    __SET_LVL(S0);
  }

  int S1 = item.stack.size() > 2 ? item.stack[item.stack.size()- 2] : -1;
  if (S1 >= 0) {
    __SET_LVL(S1);
  }

  __NC_UUS(S0lvl0); __NC_UUS(S0lvl1); __NC_UUS(S0lvl2);
  __NC_UUS(S1lvl0); __NC_UUS(S1lvl1); __NC_UUS(S1lvl2);
  return 1;
}

}

}
