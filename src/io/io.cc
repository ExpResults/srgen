#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "types/instance.h"
#include "types/settings.h"
#include "types/engine.h"
#include "io/io.h"

namespace ZGen {

namespace ShiftReduce {

/**
 * Read tokens from file and store them in the data structure
 *
 *  @param[in]  opts  The options, input field is what we concern.
 *  @param[out] data  The raw data stored in the file.
 *  @return     int   The number of instances.
 */
int read_from_tok(std::istream & is,
    std::vector< dependency_t > & data) {
  namespace algo = boost::algorithm;
  char buffer[kMaxLengthOfBuffer];
  std::vector<std::string> tokens;

  data.clear();
  while (is.getline(buffer, kMaxLengthOfBuffer)) {
    algo::split(tokens, buffer, boost::is_any_of("\t "),
        boost::token_compress_on);

    dependency_t instance;
    sentence_t & forms = instance.forms;

    for (std::vector<std::string>::iterator token = tokens.begin();
        token != tokens.end(); ++ token) {
      forms.push_back(WordEngine::get_mutable_instance().insert((*token).c_str()));
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
int read_from_dep(std::istream & is,
    std::vector< dependency_t > & data) {
  namespace algo = boost::algorithm;

  data.clear();
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

    dependency_t parse;
    while (item != eos) {
      std::string item_context = (*item);
      std::vector<std::string> items;
      std::vector<word_t> extended_words;

      algo::split(items, item_context, boost::is_any_of("\t "),
          boost::token_compress_on);

      word_t form = WordEncoderAndDecoder::NONE;
      postag_t postag = PoSTagEngine::get_const_instance().encode(items[1].c_str());
      deprel_t deprel = DeprelEngine::get_const_instance().encode(items[3].c_str());
      dependency_t::range_t phrase;

      bool is_phrase;

      if (algo::starts_with(items[0], "__") && algo::ends_with(items[0], "__")) {
        // Detect the word is pre-recognized noun phrase, decode the phrase.
        std::vector<std::string> tokens;

        algo::trim_left_if(items[0], boost::is_any_of("_"));
        algo::trim_right_if(items[0], boost::is_any_of("_"));
        // Store the splitted token into chunk.
        algo::split(tokens, items[0], boost::is_any_of("_"), boost::token_compress_on);

        for (int j = 0; j < tokens.size(); ++ j) {
          // temporally store the word in the extended words.
          word_t word = WordEngine::get_mutable_instance().insert(tokens[j].c_str());
          extended_words.push_back(word);

          // If the current word is the last word of the phrase, set it as head.
          if (j == tokens.size() - 1) {
            form = word;
          }
        }

        // maintain the phrase.
        phrase.first = parse.words.size();
        phrase.second = phrase.first + tokens.size();

        is_phrase = true;
      } else {
        word_t word = WordEngine::get_mutable_instance().insert(items[0].c_str());

        form = word;
        extended_words.push_back(word);
        phrase.first = parse.words.size();
        phrase.second = phrase.first + 1;

        is_phrase = false;
      }

      if (items.size() == 4) {
        parse.push_back(form, postag, atoi(items[2].c_str()),
            deprel, extended_words, phrase, is_phrase);
      } else if (items.size() > 4) {
        parse.push_back(form, postag, atoi(items[2].c_str()),
            deprel, extended_words, phrase, is_phrase, items[4]);
      }

      item ++;
    }

    data.push_back(parse);
    instance ++;
  }

  return (int)data.size();
}


void write_dep_instance(std::ostream & os,
    const dependency_t & parse) {
  int N = parse.forms.size();

  for (int i = 0; i < N; ++ i) {
    if (parse.is_phrases.at(i)) {
      os << "__";
      for (int j = parse.phrases.at(i).first; j < parse.phrases.at(i).second; ++ j) {
        os << WordEngine::get_const_instance().decode(parse.words[j]);
        if (j + 1 < parse.phrases[i].second) {
          os << "_";
        }
      }
      os << "__";
    } else {
      os << WordEngine::get_const_instance().decode(parse.forms.at(i));
    }

    os << "\t"
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
