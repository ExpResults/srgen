#include <iostream>
#include <fstream>
#include "boost/program_options.hpp"
#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/expressions.hpp"

#include "io.h"
#include "pipe.h"
#include "action.h"
#include "settings.h"

void init_boost_log() {
  namespace logging = boost::log;
  logging::core::get()->set_filter (
      logging::trivial::severity >= logging::trivial::trace
      );
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
    ;

  po::variables_map opts;
  po::store(po::parse_command_line(argc, argv, optparser), opts);

  if (opts.count("help")) {
    std::cerr << optparser << std::endl;
    return 1;
  }

  // [initialize the boost logging
  init_boost_log();

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
  BOOST_LOG_TRIVIAL(info) << data.size() << " sentences are loaded.";

  // [Read the reference
  std::ifstream rfs;
  bool train_mode = false;
  std::vector<SR::dependency_t> trees;
  if (opts.count("reference")) {
    rfs.open(opts["reference"].as<std::string>().c_str());

    if (rfs.good()) {
      BOOST_LOG_TRIVIAL(info) << "REFERENCE File : " << opts["reference"].as<std::string>();
      BOOST_LOG_TRIVIAL(info) << "TRAIN mode actived.";
      train_mode = true;
      int ret = SR::read_dep_data(rfs, trees);
      BOOST_LOG_TRIVIAL(info) << ret << " reference are loaded.";
    } else {
      BOOST_LOG_TRIVIAL(warning) << "REFERENCE File not specifed"
        << "TEST mode actived.";
      train_mode = false;
    }
  }

  //
  SR::Pipe pipe = SR::Pipe();

  for (int i = 0; i < data.size(); ++ i) {
    if (i != 1) {
      continue;
    }

    std::vector<SR::action::action_t> gold_actions;
    SR::sentence_t & sentence = data[i];
    SR::dependency_t parse;

    if (train_mode) {
      SR::action::get_correct_actions(trees[i], gold_actions);
      BOOST_LOG_TRIVIAL(info) << "GOT gold actions for #" << i << " inst.";
    }

    pipe.work(sentence, gold_actions, parse, i);
  }

  std::ofstream ofs;
  if (opts.count("output")) {

  }
  return 0;
}
