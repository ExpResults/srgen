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
#include "utils.h"
// #include "debug.h"

struct Option {
  enum { NONE, POSTAG, PARTIAL, FULL};

  // [The input type
  int input_type;

  // [Path to the model.
  std::string model_path;

  // [The input path.
  std::string input_path;

  // [The output path
  std::string output_path;

  // [AUX: the postag dict path, only actived at NONE mode.
  std::string postag_dict_path;

  // [MISC
  int display_interval;

  int beam_size;

  bool use_stdin;

  bool use_stdout;
};

typedef Option option_t;

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
bool parse_config(boost::program_options::variables_map & vm,
    option_t & opts) {

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

  if (!vm.count("output")) {
    opts.use_stdout = true;
  } else {
    opts.use_stdout = false;
    opts.output_path = vm["output"].as<std::string>();
  }

  // [Parse beam size
  opts.beam_size = 32;
  if (vm.count("beam")) {
    opts.beam_size = vm["beam"].as<int>();
  }

  if (opts.input_type == option_t::NONE) {
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


int main(int argc, char * argv[]) {
  namespace po = boost::program_options;
  std::string usage = "A SHIFT-REDUCE sentence generator (yijia_liu@sutd.edu.sg)\n\n";
  usage += "Usage: zgen-srg [learn|test] [options]\n\n";
  usage += "OPTIONS";
  po::options_description optparser(usage);

  optparser.add_options()
    ("help,h", "Show help information.")
    ("model,m",     po::value<std::string>(), "The path to the model.")
    ("type,t",      po::value<std::string>(), "The generation type.")
    ("input,i",     po::value<std::string>(), "The path to the input file.")
    ("output,o",    po::value<std::string>(), "The path to the output file.")
    ("reference,r", po::value<std::string>(), "The path to the reference file.")
    ("constrain,c", po::value<std::string>(), "The path to the constraints file.")
    ("posdict,p",   po::value<std::string>(), "The path to the postag dict.")
    ("display,d",   po::value<int>(),         "The display interval.")
    ("beam,b",      po::value<int>(),         "The size for beam.")
    ("verbose,v",                             "Logging every detail.")
    ;

  bool train_mode = false;

  if (argc == 1) {
    std::cerr << optparser << std::endl;
    return 1;
  } else if (strcmp(argv[1], "learn") == 0) {
    train_mode = true;
  } else if (strcmp(argv[1], "test") == 0) {
    train_mode = false;
  } else {
    std::cerr << "Unknown mode: " << argv[1] << std::endl;
    std::cerr << optparser << std::endl;
    return 1;
  }

  po::variables_map vm;
  po::store(po::parse_command_line(argc- 1, argv+ 1, optparser), vm);

  option_t opts;
  if (!parse_config(vm, opts)) {
    std::cerr << optparser << std::endl;
    return 1;
  }

  BOOST_LOG_TRIVIAL(info) << "";
  BOOST_LOG_TRIVIAL(info) << "SHIFT-REDUCE generator ["
    << (train_mode ? "train" : "test") <<"] v0.1";
  BOOST_LOG_TRIVIAL(info) << "----------------------------------";

  namespace SR = ZGen::ShiftReduce;

  // [Allocate a pipe.
  SR::Pipe * pipe = NULL;

  switch (opts.input_type) {
    case option_t::NONE:
      pipe = new SR::NonePipe(opts.postag_dict_path.c_str(), opts.beam_size);
      break;
    case option_t::POSTAG:
      pipe = new SR::PoSTagPipe(opts.beam_size);
      break;
    case option_t::PARTIAL:
      // pipe = new SR::PartialPipe(opts.beam_size);
      break;
    case option_t::FULL:
      pipe = new SR::FullPipe(opts.beam_size);
      break;
    default:
      BOOST_LOG_TRIVIAL(error) << "Unknown error.";
      return 1;
  }

  // [Load in the model.
  bool model_loaded = false;
  BOOST_LOG_TRIVIAL(info) << "SRG: loading model from: " << opts.model_path;
  if (!pipe->load_model(opts.model_path.c_str())) {
    BOOST_LOG_TRIVIAL(info) << "SRG: model doesn't exists.";
  } else {
    model_loaded = true;
  }

  // [Read the data
  std::vector<SR::dependency_t> data;

  std::istream * is = NULL;
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

  SR::read_from_dep((*is), data);
  BOOST_LOG_TRIVIAL(info) << "SRG: [" << data.size() << "] input instances are loaded.";

  std::ostream * os = NULL;
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

  for (int i = 0; i < data.size(); ++ i) {
    std::vector<SR::action::action_t> gold_actions;
    std::vector<int> order;
    SR::dependency_t & sentence = data[i];
    SR::dependency_t shuffled_sentence;
    SR::dependency_t parse;

    if (train_mode) {
      SR::action::get_correct_actions(sentence, gold_actions);
      BOOST_LOG_TRIVIAL(trace) << "GOT gold actions for #" << i << " inst.";
      for (int j = 0; j < gold_actions.size(); ++ j) {
        BOOST_LOG_TRIVIAL(trace) << "GOLD action step #" << j << " : " << gold_actions[j];
      }
    } else {
      shuffle_instance(sentence, shuffled_sentence, order);
      sentence = shuffled_sentence;
    }

    pipe->work(sentence, gold_actions, parse, i + 1);

    if (!train_mode) {
      SR::write_dep_instance((*os), parse);
    }

    if ((i + 1) % opts.display_interval == 0) {
      BOOST_LOG_TRIVIAL(info) << "PARSED #" << (i + 1) << " instances.";
    }
  }

  if (train_mode) {
    pipe->finish_training(data.size() + 1);
    pipe->save_model(opts.model_path.c_str());
    BOOST_LOG_TRIVIAL(info) << "MODEL saved.";
  }

  BOOST_LOG_TRIVIAL(info) << ".......... EXIT ..........";

  return 0;
}
