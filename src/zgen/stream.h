#ifndef __ZGEN_SR_STREAM_H__
#define __ZGEN_SR_STREAM_H__

#include <iostream>
#include "zgen/opt.h"

/**/
std::istream* get_input_stream(option_t& opts);

/**/
std::ostream* get_output_stream(option_t& opts, bool train_mode);

#endif  //  end for __ZGEN_SR_STREAM_H__
