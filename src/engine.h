#ifndef __SR_ENGINE_H__
#define __SR_ENGINE_H__

#include "serialization/unordered_map.h"

#include <boost/unordered_map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/singleton.hpp>

namespace SR {

// Use to encode PoSTag name into integer and decode integer
// into PoSTag name.
class PoSTagEncoderAndDecoder {
public:
  PoSTagEncoderAndDecoder() {}

  const char * decode(int id) const;
  int encode(const char * name) const;

  static const char * PENN_POS_NAME[];

  enum PENN_POS_ID {
    NONE = 0, BEGIN, END,
    DOLLAR, L_QUOTE, R_QUOTE,
    L_BRACKET, R_BRACKET, COMMA,
    PERIOD, COLUM, SHART,
    CC, CD, DT, EX, FW,
    IN, JJ, JJR, JJS, LS,
    MD, NN, NNP, NNPS, NNS, NP,
    PDT, POS, PRP, PRP_DOLLAR,
    RB, RBR, RBS, RP,
    SYM, TO, UH,
    VB, VBD, VBG, VBN, VBP, VBZ,
    WDT, WP, WP_DOLLAR, WRB, kMaxIndexOfPoSTag,
  };
};


// Use to encode dependency relation name into integer and decode
// integer into dependency relation name.
class DeprelsEncoderAndDecoder {
public:
  DeprelsEncoderAndDecoder() {}

  const char * decode(int id) const;
  int encode(const char * name) const;

  static const char * PENN_REL_NAME[];

  enum PENN_REL_ID {
    NONE = 0, AMOD, DEP, NMOD, OBJ,
    P,        PMOD, PRD, SBAR, SUB,
    VC,       VMOD, kMaxIndexOfDependencyRelation
  };
};


// Use to encode action prefix name into integer and decode integer
// into action prefix name.
class ActionEncoderAndDecoder {
public:
  ActionEncoderAndDecoder() {}

  const char * decode(int id) const;
  int encode(const char * name) const;

  static const char * ACTION_NAME[];

  enum ACTION_ID {
    NONE = 0, LA, RA, SH, kMaxIndexOfAction
  };
};



// Provide an equal function for character array.
typedef struct CharArrayEqualFunc {
  bool operator() (const char * s1, const char * s2) const {
    return (strcmp(s1, s2) == 0);
  }
} char_array_equal;


// Provide hash function for character array.
typedef struct CharArrayHashFunc {
  size_t operator() (const char * s) const {
    unsigned int hashTemp = 0;
    while (*s) {
      hashTemp = hashTemp * 101 + *s ++;
    }
    return (size_t(hashTemp));
  }

  bool operator() (const char * s1, const char * s2) const {
    return (strcmp(s1, s2) < 0);
  }
} char_array_hash;


// Use to encode word into integer and decode integer into
// word.
class WordEncoderAndDecoder {
public:
  WordEncoderAndDecoder()
    : grand_id(0) {
    id2name.clear();
    name2id.clear();

    insert("-NONE-");
    insert("-BEGIN-");
    insert("-END-");
  }

  ~WordEncoderAndDecoder() {
  }

  int insert(const char * name);

  const char * decode(int id) const;

  int encode(const char * name) const;

  enum SPECIAL_WORD {
    NONE, BEGIN, END
  };

  typedef boost::unordered_map<std::string, int> map_t;

  std::vector<std::string> id2name;
  map_t name2id;
  int grand_id;

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & grand_id;
    ar & name2id;
    ar & id2name;
  }
};

typedef boost::serialization::singleton<PoSTagEncoderAndDecoder>  PoSTagEngine;
typedef boost::serialization::singleton<DeprelsEncoderAndDecoder> DeprelEngine;
typedef boost::serialization::singleton<ActionEncoderAndDecoder>  ActionEngine;
typedef boost::serialization::singleton<WordEncoderAndDecoder>    WordEngine;

/**
 * Save the word engine into file.
 *
 *  @param[in]  filename  The name of the file.
 *  @return     bool      Return true on success, otherwise false.
 */
bool save_word_engine(const char * filename);

/**
 * Load the word engine from file.
 *
 *  @param[in]  filename  The name of the file.
 *  @return     bool      Return true on success, otherwise false.
 */
bool load_word_engine(const char * filename);

}

#endif  //  end for __SR_ENGINE_H__
