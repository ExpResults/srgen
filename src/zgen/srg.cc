#include <iostream>
#include <fstream>
#include <boost/log/trivial.hpp>
#include "io/io.h"
#include "pipe/pipe.h"
#include "types/action.h"
#include "types/instance.h"
#include "types/settings.h"
#include "util/utils.h"
#include "zgen/opt.h"
#include "zgen/builder.h"
#include "zgen/stream.h"

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
    ("unlabeled,u",                           "Specified to perform unlabeled parse.")
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

  // Log some information
  BOOST_LOG_TRIVIAL(info) << "";
  BOOST_LOG_TRIVIAL(info) << "SHIFT-REDUCE generator ["<< (train_mode?"train":"test")<<"] v0.1";
  BOOST_LOG_TRIVIAL(info) << "----------------------------------";
  BOOST_LOG_TRIVIAL(info) << "REPORT: beam-size : " << opts.beam_size;
  BOOST_LOG_TRIVIAL(info) << "REPORT: input-type: " << vm["type"].as<std::string>();
  BOOST_LOG_TRIVIAL(info) << "REPORT: out-deprel: " << opts.output_label;

  namespace SR = ZGen::ShiftReduce;

  // [Allocate a pipe.
  SR::Pipe* pipe = build_pipe(opts);
  if (pipe == NULL) {
    BOOST_LOG_TRIVIAL(error) << "SRG: Failed to allocate pipe";
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

  // [Read input the data
  std::vector<SR::dependency_t> data;
  std::istream* is = get_input_stream(opts);
  SR::read_from_dep((*is), data);
  BOOST_LOG_TRIVIAL(info) << "SRG: [" << data.size() << "] input instances are loaded.";

  // [Read the reference data.
  std::vector<SR::dependency_t> refer;
  if (train_mode) {
    std::ifstream rfs(opts.reference_path.c_str());
    BOOST_LOG_TRIVIAL(info) << "SRG: loading reference from " << opts.reference_path;
    if (!rfs.good()) {
      BOOST_LOG_TRIVIAL(error) << "SRG: Failed to open reference file in train model.";
      return 1;
    }
    SR::read_from_dep(rfs, refer);
    BOOST_LOG_TRIVIAL(info) << "SRG: [" << refer.size() << "] reference are loaded.";
  }

  if (data.size() != refer.size()) {
    BOOST_LOG_TRIVIAL(error) << "SRG: input size ("  << data.size()
      << ") not equal to reference size(" << refer.size();
    return 1;
  }

  std::ostream* os = get_output_stream(opts, train_mode);

  int M = data.size();
  for (int i = 0; i < M; ++ i) {
    std::vector<SR::action::action_t> gold_actions;
    std::vector<int> order;
    const SR::dependency_t * sentence = &data[i];
    SR::dependency_t shuffled_sentence;
    SR::dependency_t parse;

    if (train_mode) {
      SR::action::get_correct_actions(refer[i], gold_actions);
      BOOST_LOG_TRIVIAL(trace) << "GOT gold actions for #" << i << " inst.";
      for (int j = 0; j < gold_actions.size(); ++ j) {
        BOOST_LOG_TRIVIAL(trace) << "GOLD action step #" << j << " : " << gold_actions[j];
      }
    } else {
      shuffle_instance((*sentence), shuffled_sentence, order);
      sentence = (&shuffled_sentence);
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
