#include "zgen/opt.h"
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

//
void init_boost_log(bool verbose) {
  namespace logging = boost::log;
  namespace src = boost::log::sources;
  namespace expr = boost::log::expressions;
  namespace keywords = boost::log::keywords;

  logging::add_console_log (
      std::clog,
      keywords::format = (
        expr::stream
          << expr::format_date_time< boost::posix_time::ptime >(
            "TimeStamp", 
            "%Y-%m-%d %H:%M:%S")
          << " [" << logging::trivial::severity << "] "
          << expr::smessage
        )
      );

  if (verbose) {
    logging::core::get()->set_filter (
        logging::trivial::severity >= logging::trivial::trace
        );
  } else {
    logging::core::get()->set_filter (
        logging::trivial::severity > logging::trivial::trace
        );
  }

  logging::add_common_attributes();
}


// Parse the configuration.
bool parse_config(boost::program_options::variables_map& vm,
    option_t& opts) {

  // If show help, return false.
  if (vm.count("help")) {
    return false;
  }

  // [initialize the boost logging
  init_boost_log(vm.count("verbose") != 0);

  // [Parse the option 
  if (!vm.count("type")) {
    BOOST_LOG_TRIVIAL(error) << "Type [none|postag|partial|full] must be specified.";
    return false;
  } else {
    if (vm["type"].as<std::string>() == "none") {
      opts.input_type = option_t::NONE;
    } else if (vm["type"].as<std::string>() == "postag") {
      opts.input_type = option_t::POSTAG;
    } else if (vm["type"].as<std::string>() == "partial") {
      opts.input_type = option_t::PARTIAL;
    } else if (vm["type"].as<std::string>() == "full") {
      opts.input_type = option_t::FULL;
    } else if (vm["type"].as<std::string>() == "full-guide") {
      opts.input_type = option_t::FULL_WITH_GUIDANCE;
    } else {
      BOOST_LOG_TRIVIAL(error) << "Unknown type [" << vm["type"].as<std::string>() << "]";
      return false;
    }
  }

  if (!vm.count("model")) {
    BOOST_LOG_TRIVIAL(error) << "Model path must be specified.";
    return false;
  } else {
    opts.model_path = vm["model"].as<std::string>();
  }

  if (!vm.count("input")) {
    opts.use_stdin = true;
  } else {
    opts.input_path = vm["input"].as<std::string>();
    opts.use_stdin = false;
  }

  if (!vm.count("reference")) {
    opts.reference_path = "";
  } else {
    opts.reference_path = vm["reference"].as<std::string>();
  }

  if (!vm.count("output")) {
    opts.use_stdout = true;
  } else {
    opts.use_stdout = false;
    opts.output_path = vm["output"].as<std::string>();
  }

  if (!vm.count("unlabeled")) {
    opts.output_label = false;
  } else {
    opts.output_label = true;
  }

  // [Parse beam size
  opts.beam_size = 64;
  if (vm.count("beam")) {
    opts.beam_size = vm["beam"].as<int>();
  }

  if (opts.input_type == option_t::NONE || opts.input_type == option_t::PARTIAL) {
    // If the input type is none, perform none reference generation. At the condition,
    // the postags dict is loaded to speed up the training.
    if (vm.count("posdict")) {
      opts.postag_dict_path = vm["posdict"].as<std::string>();
    } else {
      BOOST_LOG_TRIVIAL(warning) << "SRG: No postag dict is specified.";
    }
  }

  opts.display_interval = 1000;
  if (vm.count("display")) {
    opts.display_interval = vm["display"].as<int>();
  }

  return true;
}


