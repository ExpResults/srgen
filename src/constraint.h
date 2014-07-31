#ifndef __SR_CONSTRAINT_H__
#define __SR_CONSTRAINT_H__

#include "settings.h"

#include "engine.h"

#include <boost/unordered_map.hpp>

namespace ZGen {

namespace ShiftReduce {

class Constraint {
public:
  Constraint() {
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

  void possible_tags_for_word(const char * word,
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

private:
  typedef std::bitset<kNumberOfPoSTags>                 postag_mask_t;
  typedef boost::unordered_map< word_t, postag_mask_t > map_t;

  map_t                                                 postag_constrains;
};

typedef Constraint constraint_t;

}

}

#endif  //  end for __SR_CONSTRAINT_H__
