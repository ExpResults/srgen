#ifndef __SR_ACTION_H__
#define __SR_ACTION_H__

#include <iostream>
#include "settings.h"

#include <boost/tuple/tuple.hpp>
#include <boost/functional/hash.hpp>

namespace SR {

namespace action {

// The action type
//  - 1. <int> action name prefix
//  - 2. <int> postag[on SH] / deprel[on LA and RA]
//  - 3. <int> word
class action_t {
public:
  action_t() 
    : first(0), second(0), third(0) {
      seed = 0;
  }

  int act() const {
    return first;
  }

  int rel() const {
    return second;
  }

  int word() const {
    return third;
  }

  action_t(int _first, int _second, int _third)
    : first(_first), second(_second), third(_third) {
    seed = 0;
    boost::hash_combine(seed, _first);
    boost::hash_combine(seed, _second);
    boost::hash_combine(seed, _third);
  }

  inline std::size_t hash() const {
    return seed;
  }

  bool operator == (const action_t & a) const {
    return (a.seed == seed && 
        a.first == first &&
        a.second == second &&
        a.third == third);
  }

  bool operator != (const action_t & a) const {
    return !((*this) == a);
  }

  friend std::ostream & operator << (std::ostream & os, const action_t & act) {
    os << "(" << ActionEngine::get_const_instance().decode(act.first)
      << ",";
    if (act.first == ActionEncoderAndDecoder::SH) {
      os << PoSTagEngine::get_const_instance().decode(act.second)
        << ","
        << WordEngine::get_const_instance().decode(act.third)
        << ")";
    } else {
      os << DeprelEngine::get_const_instance().decode(act.second)
        << ")";
    }

    return os;
  }


  friend std::size_t hash_value(const action_t & a) {
    return a.seed;
  }

private:
  int first, second, third;
  std::size_t seed;
};


/**
 * Get correct actions
 *
 *  @param[in]
 *  @parma[out]
 *  @return
 */
int get_correct_actions(const dependency_t & oracle,
    std::vector<action_t> & actions);

/**
 * Perform in order left to root, root to right search to get
 *
 *  @param[in]
 *  @param[in]
 *  @param[out]
 *  @return
 */
int get_correct_actions_travel(int root,
    const dependency_t & parse,
    const std::vector<std::vector<int> > & tree,
    std::vector<action_t> & actions);
}

}
#endif  //  end for __SR_ACTION_HPP__
