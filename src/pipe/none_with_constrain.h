#ifndef __SR_NONE_WITH_CONSTRAIN_PIPE__
#define __SR_NONE_WITH_CONSTRAIN_PIPE__

namespace ZGen {

namespace ShiftReduce {

class NoneWithConstrainPipe : public Pipe {
public:
  NoneWithConstrainPipe(int beam_size);
  ~NoneWithConstrainPipe();

protected:
  int config_sentence(const dependency_t* input);
};

} //  end for namespace ShiftReduce 

} //  end for namespace ZGen

#endif  //  end for __SR_NONE_WITH_CONSTRAIN_PIPE__
