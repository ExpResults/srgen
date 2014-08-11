#include <fstream>
#include "weight.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/foreach.hpp>

#define __FUM(name) do { \
  BOOST_FOREACH(us_map_t::value_type & v, (name)) { \
    v.second.flush(now); \
  } \
} while (0);

#define __FBM(name) do { \
  BOOST_FOREACH(bs_map_t::value_type & v, (name)) { \
    v.second.flush(now); \
  } \
} while (0);

#define __FTM(name) do { \
  BOOST_FOREACH(ts_map_t::value_type & v, (name)) { \
    v.second.flush(now); \
  } \
} while(0);

namespace ZGen {

namespace ShiftReduce {

bool Weight::save_weight(const char * filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    return false;
  }

  boost::archive::text_oarchive oa(ofs);
  oa << S0w << S0p;
  oa << S0ldw << S0ldp << S0ldl;
  oa << S0lddw << S0lddp << S0lddl;
  oa << S0l2dw << S0l2dp << S0l2dl;
  oa << S0rdw << S0rdp << S0rdl;
  oa << S0rddw << S0rddp << S0rddl;
  oa << S0r2dw << S0r2dp << S0r2dl;

  // oa << S0r2dwS0rdw << S0r2dpS0rdp << S0r2dlS0rdl;
  oa << S0ldwS0l2dw << S0ldpS0l2dp << S0ldlS0l2dl;

  oa << S0wS0la << S0pS0la;
  oa << S0wS0ra << S0pS0ra;
  oa << S0wS0ls << S0pS0ls;
  oa << S0wS0rs << S0pS0rs;

  oa << S0wS0ldw << S0wS0ldp << S0pS0ldw << S0pS0ldp;
  oa << S0wS0rdw << S0wS0rdp << S0pS0rdw << S0pS0rdp;
  oa << S0wS0pS0ldw << S0wS0pS0ldp << S0wS0ldwS0ldp << S0pS0ldwS0ldp;
  oa << S0wS0pS0rdw << S0wS0pS0rdp << S0wS0rdwS0rdp << S0pS0rdwS0rdp;

  oa << S1w << S1p;
  oa << S1ldw << S1ldp << S1ldl;
  oa << S1lddw << S1lddp << S1lddl;
  oa << S1l2dw << S1l2dp << S1l2dl;
  oa << S1rdw << S1rdp << S1rdl;
  oa << S1rddw << S1rddp << S1rddl;
  oa << S1r2dw << S1r2dp << S1r2dl;

  oa << S1wS1la << S1pS1la;
  oa << S1wS1ra << S1pS1ra;
  oa << S1wS1ls << S1pS1ls;
  oa << S1wS1rs << S1pS1rs;

  oa << S1wS1ldw << S1wS1ldp << S1pS1ldw << S1pS1ldp;
  oa << S1wS1rdw << S1wS1rdp << S1pS1rdw << S1pS1rdp;
  oa << S1wS1pS1ldw << S1wS1pS1ldp << S1wS1ldwS1ldp << S1pS1ldwS1ldp;
  oa << S1wS1pS1rdw << S1wS1pS1rdp << S1wS1rdwS1rdp << S1pS1rdwS1rdp;

  oa << S1r2dwS1rdw << S1r2dpS1rdp << S1r2dlS1rdl;

  oa << S0wS1w << S0pS1p;
  oa << S0wS1p << S0pS1w;
  oa << S0wS0pS1w << S0wS0pS1p << S0wS1wS1p << S0pS1wS1p;

  oa << W0 << P0;
  oa << W0W1 << P0P1;
  oa << W0W1W2 << P0P1P2;

  ofs.close();

  return true;
}

bool Weight::load_weight(const char * filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    return false;
  }

  boost::archive::text_iarchive ia(ifs);
  ia >> S0w >> S0p;
  ia >> S0ldw >> S0ldp >> S0ldl;
  ia >> S0lddw >> S0lddp >> S0lddl;
  ia >> S0l2dw >> S0l2dp >> S0l2dl;
  ia >> S0rdw >> S0rdp >> S0rdl;
  ia >> S0rddw >> S0rddp >> S0rddl;
  ia >> S0r2dw >> S0r2dp >> S0r2dl;

  // ia >> S0r2dwS0rdw >> S0r2dpS0rdp >> S0r2dlS0rdl;
  ia >> S0ldwS0l2dw >> S0ldpS0l2dp >> S0ldlS0l2dl;

  ia >> S0wS0la >> S0pS0la;
  ia >> S0wS0ra >> S0pS0ra;
  ia >> S0wS0ls >> S0pS0ls;
  ia >> S0wS0rs >> S0pS0rs;

  ia >> S0wS0ldw >> S0wS0ldp >> S0pS0ldw >> S0pS0ldp;
  ia >> S0wS0rdw >> S0wS0rdp >> S0pS0rdw >> S0pS0rdp;
  ia >> S0wS0pS0ldw >> S0wS0pS0ldp >> S0wS0ldwS0ldp >> S0pS0ldwS0ldp;
  ia >> S0wS0pS0rdw >> S0wS0pS0rdp >> S0wS0rdwS0rdp >> S0pS0rdwS0rdp;

  ia >> S1w >> S1p;
  ia >> S1ldw >> S1ldp >> S1ldl;
  ia >> S1lddw >> S1lddp >> S1lddl;
  ia >> S1l2dw >> S1l2dp >> S1l2dl;
  ia >> S1rdw >> S1rdp >> S1rdl;
  ia >> S1rddw >> S1rddp >> S1rddl;
  ia >> S1r2dw >> S1r2dp >> S1r2dl;

  ia >> S1wS1la >> S1pS1la;
  ia >> S1wS1ra >> S1pS1ra;
  ia >> S1wS1ls >> S1pS1ls;
  ia >> S1wS1rs >> S1pS1rs;

  ia >> S1wS1ldw >> S1wS1ldp >> S1pS1ldw >> S1pS1ldp;
  ia >> S1wS1rdw >> S1wS1rdp >> S1pS1rdw >> S1pS1rdp;
  ia >> S1wS1pS1ldw >> S1wS1pS1ldp >> S1wS1ldwS1ldp >> S1pS1ldwS1ldp;
  ia >> S1wS1pS1rdw >> S1wS1pS1rdp >> S1wS1rdwS1rdp >> S1pS1rdwS1rdp;

  ia >> S1r2dwS1rdw >> S1r2dpS1rdp >> S1r2dlS1rdl;

  ia >> S0wS1w >> S0pS1p;
  ia >> S0wS1p >> S0pS1w;
  ia >> S0wS0pS1w >> S0wS0pS1p >> S0wS1wS1p >> S0pS1wS1p;

  ia >> W0 >> P0;
  ia >> W0W1 >> P0P1;
  ia >> W0W1W2 >> P0P1P2;

  ifs.close();
  return true;
}

bool Weight::flush_weight(int now) {
  __FUM( S0w);     __FUM( S0p);
  __FUM( S0ldw);   __FUM( S0ldp);   __FUM( S0ldl);
  __FUM( S0lddw);  __FUM( S0lddp);  __FUM( S0lddl);
  __FUM( S0l2dw);  __FUM( S0l2dp);  __FUM( S0l2dl);
  __FUM( S0rdw);   __FUM( S0rdp);   __FUM( S0rdl);
  __FUM( S0rddw);  __FUM( S0rddp);  __FUM( S0rddl);
  __FUM( S0r2dw);  __FUM( S0r2dp);  __FUM( S0r2dl);

  // ia __FBM( S0r2dwS0rdw __FBM( S0r2dpS0rdp __FBM( S0r2dlS0rdl;
  __FBM( S0ldwS0l2dw); __FBM( S0ldpS0l2dp); __FBM( S0ldlS0l2dl);

  __FBM( S0wS0la); __FBM( S0pS0la);
  __FBM( S0wS0ra); __FBM( S0pS0ra);
  __FBM( S0wS0ls); __FBM( S0pS0ls);
  __FBM( S0wS0rs); __FBM( S0pS0rs);

  __FBM(S0wS0ldw); __FBM(S0wS0ldp); __FBM(S0pS0ldw); __FBM(S0pS0ldp);
  __FBM(S0wS0rdw); __FBM(S0wS0rdp); __FBM(S0pS0rdw); __FBM(S0pS0rdp);

  __FTM(S0wS0pS0ldw); __FTM(S0wS0pS0ldp); __FTM(S0wS0ldwS0ldp); __FTM(S0pS0ldwS0ldp);
  __FTM(S0wS0pS0rdw); __FTM(S0wS0pS0rdp); __FTM(S0wS0rdwS0rdp); __FTM(S0pS0rdwS0rdp);

  __FUM(S1w);     __FUM(S1p);
  __FUM( S1ldw ); __FUM( S1ldp ); __FUM( S1ldl);
  __FUM( S1lddw ); __FUM( S1lddp ); __FUM( S1lddl);
  __FUM( S1l2dw ); __FUM( S1l2dp ); __FUM( S1l2dl);
  __FUM( S1rdw ); __FUM( S1rdp ); __FUM( S1rdl);
  __FUM( S1rddw ); __FUM( S1rddp ); __FUM( S1rddl);
  __FUM( S1r2dw ); __FUM( S1r2dp ); __FUM( S1r2dl);

  __FBM(S1wS1la); __FBM(S1pS1la);
  __FBM( S1wS1ra ); __FBM( S1pS1ra);
  __FBM( S1wS1ls ); __FBM( S1pS1ls);
  __FBM( S1wS1rs ); __FBM( S1pS1rs);

  __FBM( S1wS1ldw ); __FBM( S1wS1ldp ); __FBM( S1pS1ldw ); __FBM( S1pS1ldp);
  __FBM( S1wS1rdw ); __FBM( S1wS1rdp ); __FBM( S1pS1rdw ); __FBM( S1pS1rdp);

  __FTM(S1wS1pS1ldw); __FTM(S1wS1pS1ldp); __FTM(S1wS1ldwS1ldp); __FTM(S1pS1ldwS1ldp);
  __FTM(S1wS1pS1rdw); __FTM(S1wS1pS1rdp); __FTM(S1wS1rdwS1rdp); __FTM(S1pS1rdwS1rdp);

  __FBM(S1r2dwS1rdw); __FBM(S1r2dpS1rdp); __FBM(S1r2dlS1rdl);
  __FBM( S0wS1w); __FBM( S0pS1p);
  __FBM( S0wS1p); __FBM( S0pS1w);
  __FTM( S0wS0pS1w); __FTM( S0wS0pS1p); __FTM( S0wS1wS1p); __FTM( S0pS1wS1p);

  __FUM(W0); __FUM( P0 );
  __FBM( W0W1 ); __FBM(P0P1);
  __FTM(W0W1W2); __FTM( P0P1P2 );
  return true;
}

#undef __FUM
#undef __FBM
#undef __FTM
}

}


