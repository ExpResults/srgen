#ifndef __ZGEN_SR_OPT_H__
#define __ZGEN_SR_OPT_H__

#include <iostream>
#include <string>
#include <boost/program_options.hpp>

//
struct Option {
  enum { NONE, PARTIAL, FULL,
    FULL_WITH_GUIDANCE_FEATURE,
    FULL_WITH_TOPDOWN_FEATURE,
    FULL_WITH_TOPDOWN_CONSTRAIN,
  };

  // [The input type
  int input_type;

  // [Path to the model.
  std::string model_path;

  // [The input path.
  std::string input_path;

  // [The output path
  std::string output_path;

  // [The reference path
  std::string reference_path;

  // [AUX: the postag dict path, only actived at NONE mode.
  std::string postag_dict_path;

  // [MISC
  int display_interval;

  int beam_size;

  bool output_label;

  bool shuffle_instance;

  bool use_stdin;

  bool use_stdout;
};

typedef Option option_t;

bool parse_config(boost::program_options::variables_map& vm,
    option_t& opts);

#endif  //  end for __ZGEN_SR_OPT_H__
