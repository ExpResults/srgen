#ifndef __SR_UTILS_H__
#define __SR_UTILS_H__

#include "types/settings.h"
#include "types/instance.h"

namespace ZGen {

namespace ShiftReduce {

void shuffle_instance(const dependency_t & instance,
    dependency_t & shuffled_instance,
    std::vector<int> & order);

}

}

#endif  //  end for __SR_UTILS_H__
