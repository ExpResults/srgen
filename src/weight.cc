#include <fstream>
#include "weight.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/foreach.hpp>

namespace SR {

bool Weight::save_weight(const char * filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    return false;
  }

  boost::archive::text_oarchive oa(ofs);
  oa << S0w;
  oa << S0p;
  oa << S0ldw;
  oa << S0ldp;
  oa << S0l2dw;
  oa << S0l2dp;
  oa << S0lddw;
  oa << S0lddp;
  oa << S0rdw;
  oa << S0rdp;
  oa << S0r2dw;
  oa << S0r2dp;
  oa << S0rddw;
  oa << S0rddp;
  oa << S0wS0la;
  oa << S0pS0la;
  oa << S0wS0ra;
  oa << S0pS0ra;
  oa << S1w;
  oa << S1p;
  oa << S1ldw;
  oa << S1ldp;
  oa << S1l2dw;
  oa << S1l2dp;
  oa << S1lddw;
  oa << S1lddp;
  oa << S1rdw;
  oa << S1rdp;
  oa << S1r2dw;
  oa << S1r2dp;
  oa << S1rddw;
  oa << S1rddp;
  oa << S1wS1la;
  oa << S1pS1la;
  oa << S1wS1ra;
  oa << S1pS1ra;
  oa << S0wS1w;
  oa << S0pS1p;
  oa << S0wS1p;
  oa << S0pS1w;
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
  ia >> S0ldw;
  ia >> S0ldp;
  ia >> S0l2dw;
  ia >> S0l2dp;
  ia >> S0lddw;
  ia >> S0lddp;
  ia >> S0rdw;
  ia >> S0rdp;
  ia >> S0r2dw;
  ia >> S0r2dp;
  ia >> S0rddw;
  ia >> S0rddp;
  ia >> S0wS0la;
  ia >> S0pS0la;
  ia >> S0wS0ra;
  ia >> S0pS0ra;
  ia >> S1w;
  ia >> S1p;
  ia >> S1ldw;
  ia >> S1ldp;
  ia >> S1l2dw;
  ia >> S1l2dp;
  ia >> S1lddw;
  ia >> S1lddp;
  ia >> S1rdw;
  ia >> S1rdp;
  ia >> S1r2dw;
  ia >> S1r2dp;
  ia >> S1rddw;
  ia >> S1rddp;
  ia >> S1wS1la;
  ia >> S1pS1la;
  ia >> S1wS1ra;
  ia >> S0pS0ra;
  ia >> S0wS1w;
  ia >> S0pS1p;
  ia >> S0wS1p;
  ia >> S0pS1w;
  ifs.close();
  return true;
}

bool Weight::flush_weight(int now) {
#define FLUSH_US_MAP(name) BOOST_FOREACH(us_map_t::value_type & v, (name)) { \
  v.second.flush(now); \
}

#define FLUSH_BS_MAP(name) BOOST_FOREACH(bs_map_t::value_type & v, (name)) { \
  v.second.flush(now); \
}

  FLUSH_US_MAP( S0w);
  FLUSH_US_MAP( S0p);
  FLUSH_US_MAP( S0ldw);
  FLUSH_US_MAP( S0ldp);
  FLUSH_US_MAP( S0l2dw);
  FLUSH_US_MAP( S0l2dp);
  FLUSH_US_MAP( S0lddw);
  FLUSH_US_MAP( S0lddp);
  FLUSH_US_MAP( S0rdw);
  FLUSH_US_MAP( S0rdp);
  FLUSH_US_MAP( S0r2dw);
  FLUSH_US_MAP( S0r2dp);
  FLUSH_US_MAP( S0rddw);
  FLUSH_US_MAP( S0rddp);
  FLUSH_BS_MAP( S0wS0la);
  FLUSH_BS_MAP( S0pS0la);
  FLUSH_BS_MAP( S0wS0ra);
  FLUSH_BS_MAP( S0pS0ra);
  FLUSH_US_MAP( S1w);
  FLUSH_US_MAP( S1p);
  FLUSH_US_MAP( S1ldw);
  FLUSH_US_MAP( S1ldp);
  FLUSH_US_MAP( S1l2dw);
  FLUSH_US_MAP( S1l2dp);
  FLUSH_US_MAP( S1lddw);
  FLUSH_US_MAP( S1lddp);
  FLUSH_US_MAP( S1rdw);
  FLUSH_US_MAP( S1rdp);
  FLUSH_US_MAP( S1r2dw);
  FLUSH_US_MAP( S1r2dp);
  FLUSH_US_MAP( S1rddw);
  FLUSH_US_MAP( S1rddp);
  FLUSH_BS_MAP( S1wS1la);
  FLUSH_BS_MAP( S1pS1la);
  FLUSH_BS_MAP( S1wS1ra);
  FLUSH_BS_MAP( S1pS1ra);
  FLUSH_BS_MAP( S0wS1w);
  FLUSH_BS_MAP( S0pS1p);
  FLUSH_BS_MAP( S0wS1p);
  FLUSH_BS_MAP( S0pS1w);
  return true;
}

}

