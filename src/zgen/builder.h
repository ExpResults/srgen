#ifndef __ZGEN_BUILDER_H__
#define __ZGEN_BUILDER_H__

#include "zgen/opt.h"
#include "pipe/pipe.h"

namespace SR = ZGen::ShiftReduce;

SR::Pipe* build_pipe(const option_t& opts);

#endif  //  end for __ZGEN_BUILDER_H__
