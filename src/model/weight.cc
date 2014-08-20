#include <fstream>
#include "model/weight.h"
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

#define __OA_U(name)                oa << name;
#define __OA_B(name1, name2)        oa << name1##name2;
#define __OA_T(name1, name2, name3) oa << name1##name2##name3;
#define __OA_U_EXT(name)            __APPENDIX_U(__OA_U, name)
#define __OA_B_EXT(name1, name2)    __APPENDIX_B(__OA_B, name1, name2)
#define __OA_B_MATRIX_2_2(name1, name2, name3, name4) \
  __BATCH_FUNCTION_2_MATRIX_2_2(__OA_B, name1, name2, name3, name4)
#define __OA_B_MATRIX_2_3(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_2_MATRIX_2_3(__OA_B, name1, name2, name3, name4, name5)
#define __OA_B_MATRIX_3_3(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_2_MATRIX_3_3(__OA_B, name1, name2, name3, name4, name5, name6)
#define __OA_T_CIRCLE_4(name1, name2, name3, name4) \
  __BATCH_FUNCTION_3_CIRCLE_4(__OA_T, name1, name2, name3, name4)
#define __OA_T_MATRIX_2_2_1(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_3_MATRIX_2_2_1(__OA_T, name1, name2, name3, name4, name5)

#define __IA_U(name)                ia >> name;
#define __IA_B(name1, name2)        ia >> name1##name2;
#define __IA_T(name1, name2, name3) ia >> name1##name2##name3;
#define __IA_U_EXT(name)            __APPENDIX_U(__IA_U, name)
#define __IA_B_EXT(name1, name2)    __APPENDIX_B(__IA_B, name1, name2)
#define __IA_B_MATRIX_2_2(name1, name2, name3, name4) \
  __BATCH_FUNCTION_2_MATRIX_2_2(__IA_B, name1, name2, name3, name4)
#define __IA_B_MATRIX_2_3(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_2_MATRIX_2_3(__IA_B, name1, name2, name3, name4, name5)
#define __IA_B_MATRIX_3_3(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_2_MATRIX_3_3(__IA_B, name1, name2, name3, name4, name5, name6)
#define __IA_T_CIRCLE_4(name1, name2, name3, name4) \
  __BATCH_FUNCTION_3_CIRCLE_4(__IA_T, name1, name2, name3, name4)
#define __IA_T_MATRIX_2_2_1(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_3_MATRIX_2_2_1(__IA_T, name1, name2, name3, name4, name5)

#define __FL_U(name)                __FUM(name)
#define __FL_B(name1, name2)        __FBM(name1##name2)
#define __FL_T(name1, name2, name3) __FTM(name1##name2##name3)
#define __FL_U_EXT(name)            __APPENDIX_U(__FL_U, name)
#define __FL_B_EXT(name1, name2)    __APPENDIX_B(__FL_B, name1, name2)
#define __FL_B_MATRIX_2_2(name1, name2, name3, name4) \
  __BATCH_FUNCTION_2_MATRIX_2_2(__FL_B, name1, name2, name3, name4)
#define __FL_B_MATRIX_2_3(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_2_MATRIX_2_3(__FL_B, name1, name2, name3, name4, name5)
#define __FL_B_MATRIX_3_3(name1, name2, name3, name4, name5, name6) \
  __BATCH_FUNCTION_2_MATRIX_3_3(__FL_B, name1, name2, name3, name4, name5, name6)
#define __FL_T_CIRCLE_4(name1, name2, name3, name4) \
  __BATCH_FUNCTION_3_CIRCLE_4(__FL_T, name1, name2, name3, name4)
#define __FL_T_MATRIX_2_2_1(name1, name2, name3, name4, name5) \
  __BATCH_FUNCTION_3_MATRIX_2_2_1(__FL_T, name1, name2, name3, name4, name5)

namespace ZGen {

namespace ShiftReduce {

bool BasicWeight::save_weight(const char * filename) {
  std::ofstream ofs(filename);
  if (!ofs.good()) {
    return false;
  }

  boost::archive::text_oarchive oa(ofs);

  __OA_U_EXT(S0);                                   // 1
  __OA_B_MATRIX_2_3(S0w, S0p, S0ldw, S0ldp, S0ldl); // 2
  __OA_B_MATRIX_2_3(S0w, S0p, S0rdw, S0rdp, S0rdl); // |
  __OA_T_MATRIX_2_2_1(S0w, S0p, S0ldw, S0ldp, S0S0ldDist);
  __OA_T_MATRIX_2_2_1(S0w, S0p, S0rdw, S0rdp, S0S0rdDist);
  __OA_T_CIRCLE_4(S0w, S0p, S0ldw, S0ldp);          // 3
  __OA_T_CIRCLE_4(S0w, S0p, S0rdw, S0rdp);          // |
  __OA_B_MATRIX_2_2(S0w, S0p, S0la, S0ra);          // 4
  __OA_B_MATRIX_2_2(S0w, S0p, S0ls, S0rs);          // |
  __OA_B_EXT(S0ld, S0l2d);                          // 5
  __OA_U_EXT(S1);                                   // 6
  __OA_B_MATRIX_2_3(S1w, S1p, S1ldw, S1ldp, S1ldl); // 7
  __OA_B_MATRIX_2_3(S1w, S1p, S1rdw, S1rdp, S1rdl); // |
  __OA_T_MATRIX_2_2_1(S1w, S1p, S1ldw, S1ldp, S1S1ldDist);
  __OA_T_MATRIX_2_2_1(S1w, S1p, S1rdw, S1rdp, S1S1rdDist);
  __OA_T_CIRCLE_4(S1w, S1p, S1ldw, S1ldp);          // 8
  __OA_T_CIRCLE_4(S1w, S1p, S1rdw, S1rdp);          // |
  __OA_B_MATRIX_2_2(S1w, S1p, S1la, S1ra);          // 9
  __OA_B_MATRIX_2_2(S1w, S1p, S1ls, S1rs);          // |
  __OA_B_EXT(S1r2d, S1rd);                          // 10
  __OA_B_MATRIX_2_2(S0w, S0p, S1w, S1p);            // 11
  __OA_T_CIRCLE_4(S0w, S0p, S1w, S1p);              // 12
  __OA_T_MATRIX_2_2_1(S0w, S0p, S1w, S1p, S0S1Dist);// 13
  oa << W0 << P0;
  oa << W0W1 << P0P1;
  oa << W0W1W2 << P0P1P2;
  oa << S0lvl0 << S0lvl1 << S0lvl2;
  oa << S1lvl0 << S1lvl1 << S1lvl2;
  ofs.close();

  return true;
}

bool BasicWeight::load_weight(const char * filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    return false;
  }

  boost::archive::text_iarchive ia(ifs);
  __IA_U_EXT(S0);                                   // 1
  __IA_B_MATRIX_2_3(S0w, S0p, S0ldw, S0ldp, S0ldl); // 2
  __IA_B_MATRIX_2_3(S0w, S0p, S0rdw, S0rdp, S0rdl); // |
  __IA_T_MATRIX_2_2_1(S0w, S0p, S0ldw, S0ldp, S0S0ldDist);
  __IA_T_MATRIX_2_2_1(S0w, S0p, S0rdw, S0rdp, S0S0rdDist);
  __IA_T_CIRCLE_4(S0w, S0p, S0ldw, S0ldp);          // 3
  __IA_T_CIRCLE_4(S0w, S0p, S0rdw, S0rdp);          // |
  __IA_B_MATRIX_2_2(S0w, S0p, S0la, S0ra);          // 4
  __IA_B_MATRIX_2_2(S0w, S0p, S0ls, S0rs);          // |
  __IA_B_EXT(S0ld, S0l2d);                          // 5
  __IA_U_EXT(S1);                                   // 6
  __IA_B_MATRIX_2_3(S1w, S1p, S1ldw, S1ldp, S1ldl); // 7
  __IA_B_MATRIX_2_3(S1w, S1p, S1rdw, S1rdp, S1rdl); // |
  __IA_T_MATRIX_2_2_1(S1w, S1p, S1ldw, S1ldp, S1S1ldDist);
  __IA_T_MATRIX_2_2_1(S1w, S1p, S1rdw, S1rdp, S1S1rdDist);
  __IA_T_CIRCLE_4(S1w, S1p, S1ldw, S1ldp);          // 8
  __IA_T_CIRCLE_4(S1w, S1p, S1rdw, S1rdp);          // |
  __IA_B_MATRIX_2_2(S1w, S1p, S1la, S1ra);          // 9
  __IA_B_MATRIX_2_2(S1w, S1p, S1ls, S1rs);          // |
  __IA_B_EXT(S1r2d, S1rd);                          // 10
  __IA_B_MATRIX_2_2(S0w, S0p, S1w, S1p);            // 11
  __IA_T_CIRCLE_4(S0w, S0p, S1w, S1p);              // 12
  __IA_T_MATRIX_2_2_1(S0w, S0p, S1w, S1p, S0S1Dist);// 13
  ia >> W0 >> P0;
  ia >> W0W1 >> P0P1;
  ia >> W0W1W2 >> P0P1P2;
  ia >> S0lvl0 >> S0lvl1 >> S0lvl2;
  ia >> S1lvl0 >> S1lvl1 >> S1lvl2;

  ifs.close();
  return true;
}

bool BasicWeight::flush_weight(int now) {
  __FL_U_EXT(S0);                                   // 1
  __FL_B_MATRIX_2_3(S0w, S0p, S0ldw, S0ldp, S0ldl); // 2
  __FL_B_MATRIX_2_3(S0w, S0p, S0rdw, S0rdp, S0rdl); // |
  __FL_T_MATRIX_2_2_1(S0w, S0p, S0ldw, S0ldp, S0S0ldDist);
  __FL_T_MATRIX_2_2_1(S0w, S0p, S0rdw, S0rdp, S0S0rdDist);
  __FL_T_CIRCLE_4(S0w, S0p, S0ldw, S0ldp);          // 3
  __FL_T_CIRCLE_4(S0w, S0p, S0rdw, S0rdp);          // |
  __FL_B_MATRIX_2_2(S0w, S0p, S0la, S0ra);          // 4
  __FL_B_MATRIX_2_2(S0w, S0p, S0ls, S0rs);          // |
  __FL_B_EXT(S0ld, S0l2d);                          // 5
  __FL_U_EXT(S1);                                   // 6
  __FL_B_MATRIX_2_3(S1w, S1p, S1ldw, S1ldp, S1ldl); // 7
  __FL_B_MATRIX_2_3(S1w, S1p, S1rdw, S1rdp, S1rdl); // |
  __FL_T_MATRIX_2_2_1(S1w, S1p, S1ldw, S1ldp, S1S1ldDist);
  __FL_T_MATRIX_2_2_1(S1w, S1p, S1rdw, S1rdp, S1S1rdDist);
  __FL_T_CIRCLE_4(S1w, S1p, S1ldw, S1ldp);          // 8
  __FL_T_CIRCLE_4(S1w, S1p, S1rdw, S1rdp);          // |
  __FL_B_MATRIX_2_2(S1w, S1p, S1la, S1ra);          // 9
  __FL_B_MATRIX_2_2(S1w, S1p, S1ls, S1rs);          // |
  __FL_B_EXT(S1r2d, S1rd);                          // 10
  __FL_B_MATRIX_2_2(S0w, S0p, S1w, S1p);            // 11
  __FL_T_CIRCLE_4(S0w, S0p, S1w, S1p);              // 12
  __FL_T_MATRIX_2_2_1(S0w, S0p, S1w, S1p, S0S1Dist);// 13

  __FUM(W0); __FUM( P0 );
  __FBM( W0W1 ); __FBM(P0P1);
  __FTM(W0W1W2); __FTM( P0P1P2 );

  __FUM(S0lvl0); __FUM(S0lvl1); __FUM(S0lvl2);
  __FUM(S1lvl0); __FUM(S1lvl1); __FUM(S1lvl2);

  return true;
}

#undef __FUM
#undef __FBM
#undef __FTM
}

}


