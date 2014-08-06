#ifndef __SR_IO_H__
#define __SR_IO_H__

#include "settings.h"

namespace ZGen {

namespace ShiftReduce {

int read_from_tok(std::istream & is,
    std::vector< dependency_t > & data);

int read_from_dep(std::istream & is,
    std::vector< dependency_t > & data);

void write_dep_instance(std::ostream & os,
    const dependency_t & pares);

}

}
#endif  //  end for __SR_IO_H__
