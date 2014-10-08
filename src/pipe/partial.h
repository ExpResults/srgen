#ifndef __SR_PIPE_PARTIAL_H__
#define __SR_PIPE_PARTIAL_H__

#include "pipe/pipe.h"
#include "types/tree.h"

namespace ZGen {

namespace ShiftReduce {

class PartialPipe: public Pipe {
public:
  PartialPipe(const char * postag_dict_path,
      bool learn,
      bool ouptut_label,
      int beam_size);
  ~PartialPipe();

protected:
  /**/
  int get_possible_actions(const StateItem& item, action_collection_t& actions);

  /**/
  int config_sentence(const dependency_t* input);

  /**/
  int config_initial_lattice();

protected:
  DependencyForest forest;

private:
  /**/
  bool legal_shift(const StateItem& item, int j);

  bool legal_left_arc(const StateItem& item);

  bool legal_right_arc(const StateItem& item);
};

} //  end for namespace ShiftReduce

} //  end for namespace ZGen

#endif  //  end for __SR_PIPE_PARTIAL_H__
