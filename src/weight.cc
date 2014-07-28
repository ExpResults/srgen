#include <fstream>
#include "weight.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace SR {

bool Weight::save_weight(const char * filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    return false;
  }

  boost::archive::text_oarchive oa(ofs);
  oa << S0w;
  oa << S0p;
  oa << S0wS0p;
  ofs.close();

  return true;
}

bool Weight::load_weight(const char * filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    return false;
  }

  boost::archive::text_iarchive ia(ifs);
  ia >> S0w;
  ia >> S0p;
  ia >> S0wS0p;
  ifs.close();
  return true;
}

}

