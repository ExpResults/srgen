#ifndef __SR_IO_H__
#define __SR_IO_H__

#include "settings.h"

namespace SR {

int read_raw_data(std::istream & is,
    std::vector< sentence_t > & data);

int read_dep_data(std::istream & is,
    std::vector< dependency_t > & data);

}

#endif  //  end for __SR_IO_H__
