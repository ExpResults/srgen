#include "zgen/stream.h"
#include <fstream>
#include <boost/log/trivial.hpp>

std::istream* get_input_stream(option_t& opts) {
  std::istream* is = NULL;

  if (!opts.use_stdin) {
    is = new std::ifstream(opts.input_path.c_str());
    if (!is->good()) {
      is = (&(std::cin));
      BOOST_LOG_TRIVIAL(warning) << "SRG: input file open failed, using STDIN instead";
    } else {
      BOOST_LOG_TRIVIAL(info) << "SRG: loading input from: " << opts.input_path;
    }
  } else {
    is = (&(std::cin));
    BOOST_LOG_TRIVIAL(warning) << "SRG: input file is not specified, using STDIN instead";
  }

  return is;
}

std::ostream* get_output_stream(option_t& opts, bool train_mode) {
  std::ostream* os = NULL;

  if (!train_mode) {
    if (!opts.use_stdout) {
      os = new std::ofstream(opts.output_path.c_str());
      if (os->good()) {
        BOOST_LOG_TRIVIAL(info) << "SRG: output result to: " << opts.output_path;
      } else {
        os = (&(std::cout));
        BOOST_LOG_TRIVIAL(warning) << "SRG: output file open failed, using STDOUT instead.";
      }
    } else {
      os = (&(std::cout));
      BOOST_LOG_TRIVIAL(warning) << "SRG: output file not specifed, using STDOUT instead.";
    }
  }

  return os;
}

