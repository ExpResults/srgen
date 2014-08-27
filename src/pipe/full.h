#ifndef __SR_FULL_PIPE_H__
#define __SR_FULL_PIPE_H__

#include "pipe/pipe.h"

namespace ZGen {

namespace ShiftReduce {

//
class FullPipe : public Pipe {
public:
  FullPipe(const char * postag_dict_path,
      bool ouptut_label,
      int beam_size);
  ~FullPipe();

protected:
  /**/
  int get_possible_actions(const StateItem & item, action_collection_t & actions);

  /**
   * Config the input onto this pipe. This function
   * cache tree structure information in the tree.
   *
   *  @param[in]  input   The input dependency
   *  @param[in]  now     The timestamp
   */
  virtual int config_sentence(const dependency_t* input);

  /**/
  int config_initial_lattice();
protected:
  DependencyTree tree;
};


}

}

#endif  //  end for __SR_FULL_PIPE_H__
