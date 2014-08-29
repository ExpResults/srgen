#ifndef __SR_FULL_WITH_TOPDOWN_CONSTRAIN_H__
#define __SR_FULL_WITH_TOPDOWN_CONSTRAIN_H__

#include "pipe/full.h"

namespace ZGen {

namespace ShiftReduce {

class FullWithTopDownConstrainPipe: public FullPipe {
public:
  //
  FullWithTopDownConstrainPipe(int beam_size);

  //
  ~FullWithTopDownConstrainPipe();

protected:
  /**/
  int get_possible_actions(const StateItem& item, action_collection_t& actions);

  /**/
  int config_sentence(const dependency_t* input);
private:
  bool conform_constrain(const StateItem& item, int j);

  bool illegal_right_arc(int from, int to);

  bool illegal_left_arc(int from, int to);

  std::vector<int> direction;
};

} //  end for namespace Shiftreduce

} //  end for namespace ZGen

#endif  //  end for __SR_FULL_WITH_TOPDOWN_CONSTRAIN_H__
