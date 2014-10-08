#ifndef __FULL_WITH_TOPDOWN_FEATURE_H__
#define __FULL_WITH_TOPDOWN_FEATURE_H__

#include "pipe/full.h"

namespace ZGen {

namespace ShiftReduce {

const double eps = 1e-6;

class FullWithTopDownFeaturePipe: public FullPipe {
public:
  //
  FullWithTopDownFeaturePipe(bool learn, int beam_size);

  //
  ~FullWithTopDownFeaturePipe();

  //
  int config_sentence(const dependency_t* input);
protected:
  /**/
  int get_state_packed_score(const StateItem& item,
      const action_sequence_t& possible_actions,
      packed_score_t& scores);

  /**/
  int update_state_score(const StateItem& item,
      const action::action_t& act, int now, int scale);

private:
  typedef std::pair<double, double> direction_score_t;
  std::vector<direction_score_t> directions;

  int hist(double probability);
};

} //  end for namespace ShiftReduce
} //  end for namespace ZGen


#endif  //  end for __FULL_WITH_TOPDOWN_FEATURE_H__
