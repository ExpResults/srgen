#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include "io.h"
#include "pipe.h"
#include "action.h"
#include "settings.h"
#include "debug.h"

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

int main(int argc, char * argv[]) {
  namespace po = boost::program_options;
  std::string usage = "A SHIFT-REDUCE sentence generator\n\n";
  usage += "OPTIONS";
  po::options_description optparser(usage);

  optparser.add_options()
    ("help,h", "Show help information.")
    ("model,m", po::value<std::string>(), "The path to the model.")
    ("input,i", po::value<std::string>(), "The path to the input file.")
    ("output,o", po::value<std::string>(), "The path to the output file.")
    ("reference,t", po::value<std::string>(), "The path to the reference file.")
    ("constrain,c", po::value<std::string>(), "The path to the constraints file.")
    ("display,d", po::value<int>(), "The display interval.")
    ("verbose,v", "Logging every detail.")
    ;

  po::variables_map opts;
  po::store(po::parse_command_line(argc, argv, optparser), opts);

  if (opts.count("help")) {
    std::cerr << optparser << std::endl;
    return 1;
  }

  // [initialize the boost logging
  init_boost_log(opts.count("verbose") != 0);

  BOOST_LOG_TRIVIAL(info) << "";
  BOOST_LOG_TRIVIAL(info) << "SHIFT-REDUCE generator v0.1";
  BOOST_LOG_TRIVIAL(info) << "---------------------------";

  // [Allocate a pipe.
  SR::Pipe pipe = SR::Pipe();

  // [Load in the model.
  bool model_loaded = false;
  if (!pipe.load_model(opts["model"].as<std::string>().c_str())) {
    BOOST_LOG_TRIVIAL(info) << "MODEL not exists.";
  } else {
    model_loaded = true;
  }

  // [Read the data
  std::vector<SR::sentence_t> data;
  if (opts.count("input")) {
    std::ifstream ifs;
    ifs.open(opts["input"].as<std::string>().c_str());

    if (ifs.good()) {
      BOOST_LOG_TRIVIAL(info) << "INPUT File : " << opts["input"].as<std::string>();
      int ret = SR::read_raw_data(ifs, data);
    } else {
      BOOST_LOG_TRIVIAL(warning) << "INPUT File open failed. Use STDIN instead";
      int ret = SR::read_raw_data(std::cin, data);
    }
  } else {
    BOOST_LOG_TRIVIAL(warning) << "INPUT File is not provided. Use STDIN instead";
    int ret = SR::read_raw_data(std::cin, data);
  }

  BOOST_LOG_TRIVIAL(info) << "LOAD sentences is done.";
  BOOST_LOG_TRIVIAL(info) << "[" << data.size() << "] sentences loaded.";

  // [Read the reference
  std::ifstream rfs;
  std::vector<SR::dependency_t> trees;
  bool train_mode = false;
  if (opts.count("reference")) {
    rfs.open(opts["reference"].as<std::string>().c_str());

    if (rfs.good()) {
      BOOST_LOG_TRIVIAL(info) << "REFERENCE File : " << opts["reference"].as<std::string>();
      BOOST_LOG_TRIVIAL(info) << "TRAIN mode actived.";
      int ret = SR::read_dep_data(rfs, trees);
      BOOST_LOG_TRIVIAL(info) << "[" << ret << "] seference are loaded.";
      train_mode = true;
    } else {
      BOOST_LOG_TRIVIAL(warning) << "REFERENCE File not specifed.";
      BOOST_LOG_TRIVIAL(info) << "TEST mode actived.";
      train_mode = false;
    }
  } else {
    if (!model_loaded) {
      BOOST_LOG_TRIVIAL(warning) << "MODEL not loaded, but specifed test";
      exit(1);
    }
  }

  int display_interval = 1000;
  if (opts.count("display")) {
    display_interval = opts["display"].as<int>();
  }

  std::ofstream ofs;
  bool use_stdout = true;
  if (!train_mode && opts.count("output")) {
    ofs.open(opts["output"].as<std::string>().c_str());
    if (ofs.good()) {
      BOOST_LOG_TRIVIAL(info) << "OUTPUT File : " << opts["output"].as<std::string>();
      use_stdout = false;
    } else {
      BOOST_LOG_TRIVIAL(warning) << "OUTPUT File : " << opts["output"].as<std::string>()
        << " open failed, Use STDOUT instead.";
    }
  } else if (!train_mode) {
    BOOST_LOG_TRIVIAL(warning) << "OUTPUT File not specifed, Use STDOUT instead.";
  }

  // debug_word_engine();

  for (int i = 0; i < data.size(); ++ i) {
    std::vector<SR::action::action_t> gold_actions;
    SR::sentence_t & sentence = data[i];
    SR::dependency_t parse;

    if (train_mode) {
      SR::action::get_correct_actions(trees[i], gold_actions);

      BOOST_LOG_TRIVIAL(trace) << "GOT gold actions for #" << i << " inst.";
      for (int j = 0; j < gold_actions.size(); ++ j) {
        BOOST_LOG_TRIVIAL(trace) << "GOLD action step #" << j << " : " << gold_actions[j];
      }
    }

    pipe.work(sentence, gold_actions, parse, i + 1);

    if (!train_mode) {
      if (use_stdout) {
        SR::write_dep_instance(std::cout, parse);
      } else {
        SR::write_dep_instance(ofs, parse);
      }
    }

    if ((i + 1) % display_interval == 0) {
      BOOST_LOG_TRIVIAL(info) << "PARSED #" << (i + 1) << " instances.";
    }
  }

  if (train_mode) {
    pipe.finish_training(data.size() + 1);
    pipe.save_model(opts["model"].as<std::string>().c_str());
    BOOST_LOG_TRIVIAL(info) << "MODEL saved.";
  }

  BOOST_LOG_TRIVIAL(info) << ".......... EXIT ..........";

  return 0;
}
