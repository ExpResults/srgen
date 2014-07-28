#ifndef __SR_ACTION_H__
#define __SR_ACTION_H__

#include <iostream>
#include "settings.h"

#include <boost/tuple/tuple.hpp>
#include <boost/functional/hash.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace SR {

namespace action {

class action_t {
public:
  action_t() 
    : prefix(0), label(0), word(0), index(-1) {
    seed = 0;
  }

  action_t(int _prefix, int _label, int _word)
    : prefix(_prefix), label(_label), word(_word), index(-1) {

    seed = 0;
    boost::hash_combine(seed, _prefix);
    boost::hash_combine(seed, _label);
    boost::hash_combine(seed, _word);
  }

  action_t(int _prefix, int _label, int _word, int _index)
    : prefix(_prefix), label(_label), word(_word), index(_index) {

    seed = 0;
    // Only hash the first three element since index is only
    // a auxilary element.
    boost::hash_combine(seed, _prefix);
    boost::hash_combine(seed, _label);
    boost::hash_combine(seed, _word);
  }

  inline std::size_t hash() const {
    return seed;
  }

  action_t & operator = (const action_t & a) {
    seed    = a.seed;
    prefix  = a.prefix;
    label   = a.label;
    word    = a.word;
    index   = a.index;

    return (*this);
  }

  bool operator == (const action_t & a) const {
    return (a.seed == seed && 
        a.prefix == prefix &&
        a.label == label &&
        a.word == word);
  }

  bool operator != (const action_t & a) const {
    return !((*this) == a);
  }

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int /* file_version */) {
    ar & prefix & label & word & index & seed;
  }

  friend std::ostream & operator << (std::ostream & os, const action_t & act) {
    os << "(" << ActionEngine::get_const_instance().decode(act.prefix)
      << ", ";

    if (act.prefix == ActionEncoderAndDecoder::SH) {
      os << PoSTagEngine::get_const_instance().decode(act.label)
        << ", "
        << WordEngine::get_const_instance().decode(act.word)
        << ", [" << act.index << "]"
        << ")";
    } else {
      os << DeprelEngine::get_const_instance().decode(act.label)
        << ")";
    }
    return os;
  }

  friend std::size_t hash_value(const action_t & a) {
    return a.seed;
  }

  int prefix;
  // The action prefix (SHIFT, LEFT-ARC, RIGHT-ARC)

  int label;
  // The relation field. It represent PoSTag (on SHIFT) and
  // dependency relation (on LEFT-ARC and RIGHT-ARC)

  int word;
  // The word inded.

  int index;
  // extra field for auxilary in the SHIFT action.

  std::size_t seed;
};

/**
 * Judge if the given action is SHIFT
 *
 *  @param[in]  act   The action
 *  @return     bool  If the action is SHIFT, return true;
 *                    otherwise false.
 */

bool is_shift(const action_t & act);

/**
 * Judge if the given action is LEFT-ARC
 *
 *  @param[in]  act   The action
 *  @return     bool  If the action is LEFT-ARC, return true;
 *                    otherwise false.
 */
bool is_left_arc(const action_t & act);


/**
 * Judge if the given action is LEFT-ARC
 *
 *  @param[in]  act   The action
 *  @return     bool  If the action is LEFT-ARC, return true;
 *                    otherwise false.
 */
bool is_right_arc(const action_t & act);


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
