#include "engine.h"
#include <cstring>
#include <fstream>

#include "serialization/unordered_map.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ZGen {

namespace ShiftReduce {

// The PoSTag encoder/decoder
const char *
PoSTagEncoderAndDecoder::PENN_POS_NAME[] = {
   "-NONE-",
   "-BEGIN-",
   "-END-",
   "$", "``", "''",
   "-LRB-", "-RRB-", ",", // note that brackets can sometimes be ( and )
   ".", ":", "#",
   "CC", "CD", "DT", "EX", "FW",
   "IN", "JJ", "JJR", "JJS", "LS",
   "MD", "NN", "NNP", "NNPS", "NNS", "NP",
   "PDT", "POS", "PRP", "PRP$",
   "RB", "RBR", "RBS", "RP",
   "SYM", "TO", "UH",
   "VB", "VBD", "VBG", "VBN", "VBP", "VBZ",
   "WDT", "WP", "WP$", "WRB"
};


const char *
PoSTagEncoderAndDecoder::decode(int id) const {
  if (id < 0 || id > kMaxIndexOfPoSTag) {
    return 0;
  }
  return PENN_POS_NAME[id];
}

#define EQ(token) (0 == strcmp(name, token))

int
PoSTagEncoderAndDecoder::encode(const char * name) const {
  if (EQ("-NONE-"))       { return NONE;      }
  else if (EQ("-BEGIN-")) { return BEGIN;     }
  else if (EQ("-END-"))   { return END;       }
  else if (EQ("$"))       { return DOLLAR;    }
  else if (EQ("``"))      { return L_QUOTE;   }
  else if (EQ("''"))      { return R_QUOTE;   }
  else if (EQ("-LRB-"))   { return L_BRACKET; }
  else if (EQ("-RRB-"))   { return R_BRACKET; }
  else if (EQ(","))       { return COMMA;     }
  else if (EQ("."))       { return PERIOD;    }
  else if (EQ(":"))       { return COLUM;     }
  else if (EQ("#"))       { return SHARP;     }
  else if (EQ("CC"))      { return CC;        }
  else if (EQ("CD"))      { return CD;        }
  else if (EQ("DT"))      { return DT;        }
  else if (EQ("EX"))      { return EX;        }
  else if (EQ("FW"))      { return FW;        }
  else if (EQ("IN"))      { return IN;        }
  else if (EQ("JJ"))      { return JJ;        }
  else if (EQ("JJR"))     { return JJR;       }
  else if (EQ("JJS"))     { return JJS;       }
  else if (EQ("LS"))      { return LS;        }
  else if (EQ("MD"))      { return MD;        }
  else if (EQ("NN"))      { return NN;        }
  else if (EQ("NNP"))     { return NNP;       }
  else if (EQ("NNPS"))    { return NNPS;      }
  else if (EQ("NNS"))     { return NNS;       }
  else if (EQ("NP"))      { return NP;        }
  else if (EQ("PDT"))     { return PDT;       }
  else if (EQ("POS"))     { return POS;       }
  else if (EQ("PRP"))     { return PRP;       }
  else if (EQ("PRP$"))    { return PRP_DOLLAR;}
  else if (EQ("RB"))      { return RB;        }
  else if (EQ("RBR"))     { return RBR;       }
  else if (EQ("RBS"))     { return RBS;       }
  else if (EQ("RP"))      { return RP;        }
  else if (EQ("SYM"))     { return SYM;       }
  else if (EQ("TO"))      { return TO;        }
  else if (EQ("UH"))      { return UH;        }
  else if (EQ("VB"))      { return VB;        }
  else if (EQ("VBD"))     { return VBD;       }
  else if (EQ("VBG"))     { return VBG;       }
  else if (EQ("VBN"))     { return VBN;       }
  else if (EQ("VBP"))     { return VBP;       }
  else if (EQ("VBZ"))     { return VBZ;       }
  else if (EQ("WDT"))     { return WDT;       }
  else if (EQ("WP"))      { return WP;        }
  else if (EQ("WP$"))     { return WP_DOLLAR; }
  else if (EQ("WRB"))     { return WRB;       }
  return NONE;
}


// The dependency name encoder/decoder
const char *
DeprelsEncoderAndDecoder::PENN_REL_NAME[]  = {
  "-NONE-", "AMOD", "DEP", "NMOD", "OBJ",
  "P",      "PMOD", "PRD", "SBAR", "SUB",
  "VC",     "VMOD",
};


const char *
DeprelsEncoderAndDecoder::decode(int id) const {
  if (id < 0 || id > kMaxIndexOfDependencyRelation) {
    return 0;
  }
  return PENN_REL_NAME[id];
}

int
DeprelsEncoderAndDecoder::encode(const char * name) const {
  if (EQ("-NONE-"))     { return NONE;  }
  else if (EQ("AMOD"))  { return AMOD;  }
  else if (EQ("DEP"))   { return DEP;   }
  else if (EQ("NMOD"))  { return NMOD;  }
  else if (EQ("OBJ"))   { return OBJ;   }
  else if (EQ("P"))     { return P;     }
  else if (EQ("PMOD"))  { return PMOD;  }
  else if (EQ("PRD"))   { return PRD;   }
  else if (EQ("SBAR"))  { return SBAR;  }
  else if (EQ("SUB"))   { return SUB;   }
  else if (EQ("VC"))    { return VC;    }
  else if (EQ("VMOD"))  { return VMOD;  }
  return NONE;
}


// The action name encoder/decoder
const char *
ActionEncoderAndDecoder::ACTION_NAME[] = {
  "-NONE-", "LA", "RA", "SH",
};


const char *
ActionEncoderAndDecoder::decode(int id) const {
  if (id < 0 || id > kMaxIndexOfAction) {
    return 0;
  }
  return ACTION_NAME[id];
}


int
ActionEncoderAndDecoder::encode(const char * name) const {
  if (EQ("-NONE-"))   { return NONE;  }
  else if (EQ("LA"))  { return LA;    }
  else if (EQ("RA"))  { return RA;    }
  else if (EQ("SH"))  { return SH;    }
  return -1;
}


// The word encoder/decoder
int
WordEncoderAndDecoder::insert(const char * name) {
  std::string key(name);
  map_t::const_iterator itx = name2id.find(key);

  if (itx != name2id.end()) {
    return itx->second;
  }

  id2name.push_back(key);
  name2id[key] = grand_id;
  return grand_id ++;
}


const char *
WordEncoderAndDecoder::decode(int id) const {
  // std::cout << id2name.size() << std::endl;
  // std::cout << id << std::endl;
  if (id < 0 || id >= grand_id) {
    return 0;
  }
  return id2name.at(id).c_str();
}


int
WordEncoderAndDecoder::encode(const char * name) const {
  map_t::const_iterator itx = name2id.find(std::string(name));
  if (itx != name2id.end()) {
    return itx->second;
  }
  return -1;
}

#undef EQ

bool
save_word_engine(const char * filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    return false;
  }
  
  boost::archive::text_oarchive oa(ofs);
  oa << WordEngine::get_const_instance();
  ofs.close();
  return true;
}


bool
load_word_engine(const char * filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    return false;
  }

  WordEngine::get_mutable_instance().id2name.clear();
  WordEngine::get_mutable_instance().name2id.clear();

  boost::archive::text_iarchive ia(ifs);
  ia >> WordEngine::get_mutable_instance();
  ifs.close();
  return true;
}


}

}
