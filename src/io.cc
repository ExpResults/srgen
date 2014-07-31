#include "io.h"

#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/split.hpp"

#include "settings.h"
#include "engine.h"

namespace ZGen {

namespace ShiftReduce {

/**
 * Read tokens from file and store them in the data structure
 *
 *  @param[in]  opts  The options, input field is what we concern.
 *  @param[out] data  The raw data stored in the file.
 *  @return     int   The number of instances.
 */
int read_raw_data(std::istream & is,
    std::vector< sentence_t > & data) {
  namespace algo = boost::algorithm;
  char buffer[kMaxLengthOfBuffer];
  std::vector<std::string> tokens;

  data.clear();
  while (is.getline(buffer, kMaxLengthOfBuffer)) {
    algo::split(tokens, buffer, boost::is_any_of("\t "),
        boost::token_compress_on);

    std::vector< word_t > instance;
    for (std::vector<std::string>::iterator token = tokens.begin();
        token != tokens.end(); ++ token) {
      instance.push_back(
          WordEngine::get_mutable_instance().insert((*token).c_str()));
    }

    data.push_back(instance);
  }

  return (int)data.size();
}

/**
 * Read dependency parse reference from file.
 *
 *  @param[in]  is    The file stream
 *  @parma[out] trees The dependency parse trees.
 *  @return     int   The number of dependency tree read.
 */
int read_dep_data(std::istream & is,
    std::vector<DependencyParse> & trees) {
  namespace algo = boost::algorithm;

  trees.clear();
  std::string data_context((std::istreambuf_iterator<char>(is)),
      std::istreambuf_iterator<char>());

  boost::regex INSTANCE_DELIMITER("\n\n");
  boost::regex ITEM_DELIMITER("\n");
  boost::sregex_token_iterator instance(data_context.begin(),
      data_context.end(), INSTANCE_DELIMITER, -1);
  boost::sregex_token_iterator eos;

  // Loop over the instances
  while (instance != eos) {
    std::string instance_context = (*instance);

    boost::sregex_token_iterator item(instance_context.begin(),
        instance_context.end(), ITEM_DELIMITER, -1);

    DependencyParse parse = DependencyParse();
    while (item != eos) {
      std::string item_context = (*item);
      std::vector<std::string> items;

      algo::split(items, item_context, boost::is_any_of("\t "),
          boost::token_compress_on);

      parse.push_back(
          WordEngine::get_mutable_instance().insert(items[0].c_str()),
          PoSTagEngine::get_const_instance().encode(items[1].c_str()),
          atoi(items[2].c_str()),
          DeprelEngine::get_const_instance().encode(items[3].c_str()));
      item ++;
    }

    trees.push_back(parse);
    instance ++;
  }

  return (int)trees.size();
}


void write_dep_instance(std::ostream & os,
    const dependency_t & parse) {
  int N = parse.forms.size();

  for (int i = 0; i < N; ++ i) {
    os << WordEngine::get_const_instance().decode(parse.forms.at(i))
      << "\t"
      << PoSTagEngine::get_const_instance().decode(parse.postags.at(i))
      << "\t"
      << parse.heads.at(i)
      << "\t"
      << DeprelEngine::get_const_instance().decode(parse.deprels.at(i))
      << std::endl;
  }
  os << std::endl;
}

}   //  end for namespace

}   //  end for namespace ZGen
