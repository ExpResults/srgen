#ifndef __SR_POSTAG_PIPE_H__
#define __SR_POSTAG_PIPE_H__

#include "pipe/pipe.h"

namespace ZGen {

namespace ShiftReduce {

//
class PoSTagPipe : public Pipe {
public:
  PoSTagPipe(int beam_size);

protected:
  /**
   * Get the possible actions from the input item.
   *
   *  @param[in]  item          The input item
   *  @param[out] actions       The possible actions
   *  @return     int
   */
  int get_possible_actions(const StateItem & item, action_collection_t & actions);

  /**/
  int config_sentence(const dependency_t* input);

  /**/
  int config_initial_lattice();
};

}

}

#endif  //  end for __SR_POSTAG_PIPE_H__
