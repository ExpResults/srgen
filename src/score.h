#ifndef __SR_SCORE_H__
#define __SR_SCORE_H__

#include "action.h"
#include "settings.h"

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ZGen {

namespace ShiftReduce {

// Unigram
struct UnigramScore {
  UnigramScore() {}
  
  UnigramScore(int feat, const action::action_t & act)
    : payload(feat, act) {
  }

  bool operator == (const UnigramScore & a) const {
    return (a.payload.get<0>() == payload.get<0>() &&
        a.payload.get<1>().hash() == payload.get<1>().hash());
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned version) {
    ar & payload.get<0>() & payload.get<1>();
  }

  friend std::size_t hash_value(const UnigramScore & m) {
    std::size_t seed = 0;
    boost::hash_combine(seed, m.payload.get<0>());
    boost::hash_combine(seed, m.payload.get<1>().hash());
    return seed;
  }

  boost::tuples::tuple<int, action::action_t> payload;
};


// Bigram
struct BigramScore {
  BigramScore() {}
  
  BigramScore(int feat1, int feat2, const action::action_t & act)
    : payload(feat1, feat2, act) {
  }

  bool operator == (const BigramScore & a) const {
    return (a.payload.get<0>() == payload.get<0>() &&
        a.payload.get<1>() == payload.get<1>() &&
        a.payload.get<2>().hash() == payload.get<2>().hash());
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned version) {
    ar & payload.get<0>() & payload.get<1>() & payload.get<2>();
  }

  friend std::size_t hash_value(const BigramScore & m) {
    std::size_t seed = 0;
    boost::hash_combine(seed, m.payload.get<0>());
    boost::hash_combine(seed, m.payload.get<1>());
    boost::hash_combine(seed, m.payload.get<2>().hash());
    return seed;
  }

  boost::tuples::tuple<int, int, action::action_t> payload;
};


// Trigram
struct TrigramScore {
  TrigramScore() {}

  TrigramScore(int feat0, int feat1, int feat2, const action::action_t & act)
    : payload(feat0, feat1, feat2, act) {
  }

  bool operator == (const TrigramScore & a) const {
    return (a.payload.get<0>() == payload.get<0>() &&
        a.payload.get<1>() == payload.get<1>() &&
        a.payload.get<2>() == payload.get<2>() &&
        a.payload.get<3>().hash() == payload.get<3>().hash());
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned version) {
    ar & payload.get<0>() & payload.get<1>() & payload.get<2>() & payload.get<3>();
  }

  friend std::size_t hash_value(const TrigramScore & m) {
    std::size_t seed = 0;
    boost::hash_combine(seed, m.payload.get<0>());
    boost::hash_combine(seed, m.payload.get<1>());
    boost::hash_combine(seed, m.payload.get<2>());
    boost::hash_combine(seed, m.payload.get<3>().hash());
    return seed;
  }

  boost::tuples::tuple<int, int, int, action::action_t> payload;
};

typedef UnigramScore  us_t;
typedef BigramScore   bs_t;
typedef TrigramScore  ts_t;

}

}

#endif  //  end for __SR_SCORE_H__
