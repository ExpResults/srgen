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
  ifs.close();
  return true;
}

bool Weight::flush_weight(int now) {
  BOOST_FOREACH(us_map_t::value_type & v, S0w) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0p) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0ldw) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0ldp) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0l2dw) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0l2dp) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0lddw) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0lddp) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0rdw) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0rdp) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0r2dw) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0r2dp) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0rddw) {
    v.second.flush(now);
  }

  BOOST_FOREACH(us_map_t::value_type & v, S0rddp) {
    v.second.flush(now);
  }
  return true;
}

}

