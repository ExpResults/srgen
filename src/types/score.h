#ifndef __SR_SCORE_H__
#define __SR_SCORE_H__

#include "types/settings.h"
#include "types/action.h"

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ZGen {

namespace ShiftReduce {

// Unigram
struct UnigramScore {
  UnigramScore() {}
  
  UnigramScore(int feat, const action::action_t& act)
    : rep(feat, act) {
  }

  bool operator == (const UnigramScore & a) const {
    return (a.rep.get<0>() == rep.get<0>() &&
        a.rep.get<1>() == rep.get<1>());
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned version) {
    ar & rep.get<0>() & rep.get<1>();
  }

  friend std::size_t hash_value(const UnigramScore & m) {
    std::size_t seed = 0;
    boost::hash_combine(seed, m.rep.get<0>());
    boost::hash_combine(seed, m.rep.get<1>());
    return seed;
  }

  boost::tuples::tuple<int, action::action_t> rep;
};


// Bigram
struct BigramScore {
  BigramScore() {}
  
  BigramScore(int feat1, int feat2, const action::action_t& act)
    : rep(feat1, feat2, act) {
  }

  bool operator == (const BigramScore & a) const {
    return (a.rep.get<0>() == rep.get<0>() &&
        a.rep.get<1>() == rep.get<1>() &&
        a.rep.get<2>() == rep.get<2>());
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned version) {
    ar & rep.get<0>() & rep.get<1>() & rep.get<2>();
  }

  friend std::size_t hash_value(const BigramScore & m) {
    std::size_t seed = 0;
    boost::hash_combine(seed, m.rep.get<0>());
    boost::hash_combine(seed, m.rep.get<1>());
    boost::hash_combine(seed, m.rep.get<2>());
    return seed;
  }

  boost::tuples::tuple<int, int, action::action_t> rep;
};


// Trigram
struct TrigramScore {
  TrigramScore() {}

  TrigramScore(int feat0, int feat1, int feat2, const action::action_t& act)
    : rep(feat0, feat1, feat2, act) {
  }

  bool operator == (const TrigramScore & a) const {
    return (a.rep.get<0>() == rep.get<0>() &&
        a.rep.get<1>() == rep.get<1>() &&
        a.rep.get<2>() == rep.get<2>() &&
        a.rep.get<3>() == rep.get<3>());
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned version) {
    ar & rep.get<0>() & rep.get<1>() & rep.get<2>() & rep.get<3>();
  }

  friend std::size_t hash_value(const TrigramScore & m) {
    std::size_t seed = 0;
    boost::hash_combine(seed, m.rep.get<0>());
    boost::hash_combine(seed, m.rep.get<1>());
    boost::hash_combine(seed, m.rep.get<2>());
    boost::hash_combine(seed, m.rep.get<3>());
    return seed;
  }

  boost::tuples::tuple<int, int, int, action::action_t> rep;
};

typedef UnigramScore  us_t;
typedef BigramScore   bs_t;
typedef TrigramScore  ts_t;

}

}

#endif  //  end for __SR_SCORE_H__
