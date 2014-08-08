#include <fstream>
#include "weight.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/foreach.hpp>

#define FLUSH_US_MAP(name) BOOST_FOREACH(us_map_t::value_type & v, (name)) { \
  v.second.flush(now); \
}

#define FLUSH_BS_MAP(name) BOOST_FOREACH(bs_map_t::value_type & v, (name)) { \
  v.second.flush(now); \
}

#define FLUSH_TS_MAP(name) BOOST_FOREACH(ts_map_t::value_type & v, (name)) { \
  v.second.flush(now); \
}

namespace ZGen {

namespace ShiftReduce {

bool Weight::save_weight(const char * filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    return false;
  }

  boost::archive::text_oarchive oa(ofs);
  // 
  oa << S0w << S0p;
  //
  oa << S0ldw << S0ldp << S0ldl << S0l2dw << S0l2dp << S0l2dl << S0lddw << S0lddp << S0lddl;
  oa << S0rdw << S0rdp << S0rdl << S0r2dw << S0r2dp << S0r2dl << S0rddw << S0rddp << S0rddl;
  oa << S0r2dwS0rdw << S0r2dpS0rdp << S0r2dlS0rdl;
  oa << S0ldwS0l2dw << S0ldpS0l2dp << S0ldlS0l2dl;
  //
  oa << S0wS0la << S0pS0la << S0wS0ra << S0pS0ra;
  oa << S0wS0ls << S0pS0ls << S0wS0rs << S0pS0rs;
  // 
  oa << S1w << S1p;
  //
  oa << S1ldw << S1ldp << S1ldl << S1l2dw << S1l2dp << S1l2dl << S1lddw << S1lddp << S1lddl;
  oa << S1rdw << S1rdp << S1rdl << S1r2dw << S1r2dp << S1l2dl << S1rddw << S1rddp << S1rddl;
  oa << S1r2dwS1rdw << S1r2dpS1rdp << S1r2dlS1rdl;
  //
  oa << S1wS1la << S1pS1la << S1wS1ra << S1pS1ra;
  oa << S1pS1ls << S1wS1rs << S1pS1rs << S1pS1rs;
  // 
  oa << S0wS1p << S0pS1p << S0wS1p << S0pS1w;
  //
  oa << W0 << P0 << W0W1 << P0P1 << W0W1W2 << P0P1P2;
  ofs.close();

  return true;
}

bool Weight::load_weight(const char * filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    return false;
  }

  boost::archive::text_iarchive ia(ifs);
  // 
  ia >> S0w >> S0p;
  //
  ia >> S0ldw >> S0ldp >> S0ldl >> S0l2dw >> S0l2dp >> S0l2dl >> S0lddw >> S0lddp >> S0lddl;
  ia >> S0rdw >> S0rdp >> S0rdl >> S0r2dw >> S0r2dp >> S0r2dl >> S0rddw >> S0rddp >> S0rddl;
  ia >> S0r2dwS0rdw >> S0r2dpS0rdp >> S0r2dlS0rdl;
  ia >> S0ldwS0l2dw >> S0ldpS0l2dp >> S0ldlS0l2dl;
  //
  ia >> S0wS0la >> S0pS0la >> S0wS0ra >> S0pS0ra;
  ia >> S0wS0ls >> S0pS0ls >> S0wS0rs >> S0pS0rs;
  // 
  ia >> S1w >> S1p;
  //
  ia >> S1ldw >> S1ldp >> S1ldl >> S1l2dw >> S1l2dp >> S1l2dl >> S1lddw >> S1lddp >> S1lddl;
  ia >> S1rdw >> S1rdp >> S1rdl >> S1r2dw >> S1r2dp >> S1r2dl >> S1rddw >> S1rddp >> S1rddl;
  ia >> S1r2dwS1rdw >> S1r2dpS1rdp >> S1r2dlS1rdl;
  //
  ia >> S1wS1la >> S1pS1la >> S1wS1ra >> S1pS1ra;
  ia >> S1pS1ls >> S1wS1rs >> S1pS1rs >> S1pS1rs;
  // 
  ia >> S0wS1p >> S0pS1p >> S0wS1p >> S0pS1w;
  ia >> W0 >> P0 >> W0W1 >> P0P1 >> W0W1W2 >> P0P1P2;
  ifs.close();
  return true;
}

bool Weight::flush_weight(int now) {
  //
  FLUSH_US_MAP( S0w);     FLUSH_US_MAP( S0p);
  //
  FLUSH_US_MAP( S0ldw);   FLUSH_US_MAP( S0ldp);     FLUSH_US_MAP(S0ldl);
  FLUSH_US_MAP( S0l2dw);  FLUSH_US_MAP( S0l2dp);    FLUSH_US_MAP(S0l2dl);
  FLUSH_US_MAP( S0lddw);  FLUSH_US_MAP( S0lddp);    FLUSH_US_MAP(S0lddl);
  //
  FLUSH_US_MAP( S0rdw);   FLUSH_US_MAP( S0rdp);     FLUSH_US_MAP(S0rdl);
  FLUSH_US_MAP( S0r2dw);  FLUSH_US_MAP( S0r2dp);    FLUSH_US_MAP(S0r2dl);
  FLUSH_US_MAP( S0rddw);  FLUSH_US_MAP( S0rddp);    FLUSH_US_MAP(S0rddl);
  //
  FLUSH_BS_MAP( S0r2dwS0rdw);
  FLUSH_BS_MAP( S0r2dpS0rdp);
  FLUSH_BS_MAP( S0r2dlS0rdl);
  //
  FLUSH_BS_MAP( S0ldwS0l2dw);
  FLUSH_BS_MAP( S0ldpS0l2dp);
  FLUSH_BS_MAP( S0ldlS0l2dl);
  //
  FLUSH_BS_MAP( S0wS0la); FLUSH_BS_MAP( S0pS0la);
  FLUSH_BS_MAP( S0wS0ra); FLUSH_BS_MAP( S0pS0ra);
  //
  FLUSH_BS_MAP( S0wS0ls); FLUSH_BS_MAP( S0pS0ls);
  FLUSH_BS_MAP( S0wS0rs); FLUSH_BS_MAP( S0pS0rs);
  //
  FLUSH_US_MAP( S1w);     FLUSH_US_MAP( S1p);
  //
  FLUSH_US_MAP( S1ldw);   FLUSH_US_MAP( S1ldp);     FLUSH_US_MAP(S1ldl);
  FLUSH_US_MAP( S1l2dw);  FLUSH_US_MAP( S1l2dp);    FLUSH_US_MAP(S1l2dl);
  FLUSH_US_MAP( S1lddw);  FLUSH_US_MAP( S1lddp);    FLUSH_US_MAP(S1lddl);
  //
  FLUSH_US_MAP( S1rdw);   FLUSH_US_MAP( S1rdp);     FLUSH_US_MAP(S1rdl);
  FLUSH_US_MAP( S1r2dw);  FLUSH_US_MAP( S1r2dp);    FLUSH_US_MAP(S1r2dl);
  FLUSH_US_MAP( S1rddw);  FLUSH_US_MAP( S1rddp);    FLUSH_US_MAP(S1rddl);
  //
  FLUSH_BS_MAP( S1r2dwS1rdw);
  FLUSH_BS_MAP( S1r2dpS1rdp);
  FLUSH_BS_MAP( S1r2dlS1rdl);
  //
  FLUSH_BS_MAP( S1wS1la); FLUSH_BS_MAP( S1pS1la);
  FLUSH_BS_MAP( S1wS1ra); FLUSH_BS_MAP( S1pS1ra);
  //
  FLUSH_BS_MAP( S1wS1ls); FLUSH_BS_MAP( S1pS1ls);
  FLUSH_BS_MAP( S1wS1rs); FLUSH_BS_MAP( S1pS1rs);
  // 
  FLUSH_BS_MAP( S0wS1w);  FLUSH_BS_MAP( S0pS1p);
  FLUSH_BS_MAP( S0wS1p);  FLUSH_BS_MAP( S0pS1w);
  //
  FLUSH_US_MAP( W0);      FLUSH_US_MAP( P0);
  FLUSH_BS_MAP( W0W1);    FLUSH_BS_MAP( P0P1);
  FLUSH_TS_MAP( W0W1W2);  FLUSH_TS_MAP( P0P1P2);
  return true;
}

}

}

#undef FLUSH_US_MAP
#undef FLUSH_BS_MAP
#undef FLUSH_TS_MAP
