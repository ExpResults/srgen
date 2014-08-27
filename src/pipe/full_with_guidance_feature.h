#ifndef __SR_FULL_PIPE_WITH_GUIDANCE_FEATURE_H__
#define __SR_FULL_PIPE_WITH_GUIDANCE_FEATURE_H__

#include "pipe/full.h"

namespace ZGen {

namespace ShiftReduce {

// The full pipe with guidance.
class FullWithGuidanceFeaturePipe: public FullPipe {
public:
  //
  FullWithGuidanceFeaturePipe(int beam_size);

  //
  ~FullWithGuidanceFeaturePipe();

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
  DependencyTreeWithGuidance tree_with_guidance;
};

}

}

#endif  //  end for __SR_FULL_PIPE_WITH_GUIDANCE_FEATURE_H__
