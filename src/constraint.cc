#include "constraint.h"
#include <iostream>
#include <fstream>

#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/split.hpp"

namespace ZGen {

namespace ShiftReduce {

Constraint::Constraint(const char * postag_dict_file) {
  namespace algo = boost::algorithm;

  std::ifstream ifs(postag_dict_file);

  if (ifs.good()) {
    char buffer[kMaxLengthOfBuffer];
    std::vector<std::string> tokens;

    while (ifs.getline(buffer, kMaxLengthOfBuffer)) {
      algo::split(tokens, buffer, boost::is_any_of("\t "),
          boost::token_compress_on);

      if (tokens.size() != 2) {
        continue;
      }

      word_t    word = WordEngine::get_mutable_instance().insert(tokens[0].c_str());
      postag_t  postag = PoSTagEngine::get_const_instance().encode(tokens[1].c_str());

      if (postag < kStartIndexOfValidPoSTag || postag >= kNumberOfPoSTags) {
        continue;
      }

      postag_constrains[word].set(postag);
    }
  } else {
    init_default_possible_tags();
  }
}


void Constraint::init_default_possible_tags() {
  word_t word;

  word = WordEngine::get_mutable_instance().insert("$");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::DOLLAR);

  word = WordEngine::get_mutable_instance().insert("``");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::L_QUOTE);

  word = WordEngine::get_mutable_instance().insert("''");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::R_QUOTE);

  word = WordEngine::get_mutable_instance().insert("-LRB-");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::L_BRACKET);

  word = WordEngine::get_mutable_instance().insert("-RRB-");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::R_BRACKET);

  word = WordEngine::get_mutable_instance().insert(",");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::COMMA);

  word = WordEngine::get_mutable_instance().insert(".");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::PERIOD);

  word = WordEngine::get_mutable_instance().insert(":");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::COLUM);

  word = WordEngine::get_mutable_instance().insert("#");
  postag_constrains[word].set(PoSTagEncoderAndDecoder::SHARP);
}


void Constraint::get_possible_tags(const char * word,
    std::vector< postag_t > & possible_tags) {

  possible_tags.clear();

  // __ => NP
  if (word[0] == '_' && word[0] == '_') {
    possible_tags.push_back(PoSTagEncoderAndDecoder::NP);
    return;
  }

  map_t::const_iterator itx = postag_constrains.find(
      WordEngine::get_const_instance().encode(word));

  if (itx != postag_constrains.end()) {
    const postag_mask_t & mask = itx->second;
    for (int i = 0; i < mask.size(); ++ i) {
      if (mask.test(i)) {
        possible_tags.push_back(i);
      }
    }
    return;
  }

  for (int i = kStartIndexOfValidPoSTag; i < kNumberOfPoSTags; ++ i) {
    possible_tags.push_back(i);
  }
}

}

}
