#ifndef __SR_NONE_PIPE_H__
#define __SR_NONE_PIPE_H__

#include "pipe/pipe.h"

namespace ZGen {

namespace ShiftReduce {

//
class NonePipe: public Pipe {
public:
  NonePipe(const char * postag_dict_path,
      int beam_size);

protected:
  /**
   * Get the possible actions from the input item.
   *
   *  @param[in]  item          The input item
   *  @param[out] actions       The possible actions
   *  @return     int           The number of possible actons
   */
  int get_possible_actions(const StateItem & item, action_collection_t & actions);

  /**
   * Config the sentence onto the Pipe
   *
   *  @param[in]  input       The input
   *  @param[in]  now         The now
   */
  int config_sentence(const dependency_t* input);

  int config_initial_lattice();

  constraint_t constraint;
};

} //  end for namespace ShiftReduce

} //  end for namespace ZGen

#endif  //  end for __SR_NONE_PIPE_H__
