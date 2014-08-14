#ifndef __SR_STATE_HPP__
#define __SR_STATE_HPP__

#include <iostream>
#include <bitset>
#include <cstring>

#include "types/settings.h"
#include "types/action.h"

namespace ZGen {

namespace ShiftReduce {

class StateItem {

public:
  // The default constructor,
  StateItem()
    : instance_ref(0),
    N(0) {
    clear();
  }

  // The constructor with sentence reference.
  StateItem(const dependency_t * _instance_ref)
    : instance_ref(_instance_ref),
    N(_instance_ref->size()) {
    clear();
  }

  /**
   * Set the sentence reference to the current StateItem, it won't
   * check whether the reference is seted.
   *
   *  @param[in] sentence_ref The pointer to the sentence
   */
  void set_instance_reference(const dependency_t * _instance_ref);

  /**
   * Clear the state item.
   */
  void clear();

public:
  // Operators
  StateItem & operator = (const StateItem & item);

  // less than
  bool operator < (const StateItem & other) const;

  // greater than
  bool operator > (const StateItem & other) const;

  friend std::ostream & operator << (std::ostream & os, const StateItem & item);

  // access
  int stack_top() const;

public:
  int N;  // The total number of word for thesentence.

  const dependency_t * instance_ref;
  // The reference to the sentence

  const StateItem * previous;

  floatval_t score;
  // The score for the current state

  std::vector<int> stack;
  // The stack for the current state

  std::vector< word_t >   word_sequence;
  //

  std::vector< postag_t > postag_sequence;
  //

  std::bitset<kMaxNumberOfWords> left_arced;
  // Used to specified if left arc has performed on the stack top

  std::bitset<kMaxNumberOfWords> buffer;
  // The word indices in buffer for current state.

  int postags[kMaxNumberOfWords];
  // The postags

  int heads[kMaxNumberOfWords];
  // The tree(heads) for the current status

  int deprels[kMaxNumberOfWords];
  // The tree(deprels) for the current state

  int nr_left_children[kMaxNumberOfWords];
  // The number of left children.

  int nr_right_children[kMaxNumberOfWords];
  // The number of right children.

  int nr_left_descendant[kMaxNumberOfWords];
  // The sum of left children

  int nr_right_descendant[kMaxNumberOfWords];
  // The sum of right children

  int left_most_child[kMaxNumberOfWords];
  // CACHE: use to store the left most child for the word.

  int right_most_child[kMaxNumberOfWords];
  // CACHE: use to store the right most child for the word.

  int left_2nd_most_child[kMaxNumberOfWords];
  // CACHE: use to store the 2nd-left-most child.

  int right_2nd_most_child[kMaxNumberOfWords];
  // CACHE: use to store the 2nd-right-most child.

  action::action_t last_action;
  // The action
public:

  /**
   * Copy other state into the current state.
   *
   *  @param[in]  other   The other state.
   */
  void copy(const StateItem & other);


  /**
   * Perform shift on current state, result in a new state on itself.
   *
   *  @param[in]  postag    The PoSTag (represented in integer).
   *  @param[in]  word      The word (represented in integer).
   *  @param[in]  word_id   The index of the word in this sentence.
   *  @return     bool      If successfully shift, return true. 
   *                        otherwise false.
   */
  bool shift(postag_t postag,
      word_t word,
      int word_rank);


  /**
   * Perform shift on current state and result in a new state.
   *
   *  @param[in]  postag    The PoSTag (represented in integer).
   *  @param[in]  word      The word (represented in integer).
   *  @param[in]  word_id   The index of the word in this sentence.
   *  @param[out] other     The resulted in state.
   *  @return     bool      If successfully shift, return true, 
   *                        otherwise false
   */
  bool shift(postag_t postag, word_t word,
      int word_id, StateItem & other) const; 


  /**
   * Perform left arc on current state, result in a new state on itself.
   *
   *  @param[in]  deprel    The dependency relation.
   *  @return     bool      If successfully performed action, return true;
   *                        Otherwise false.
   */
  bool left_arc(deprel_t deprel);

  /**
   */
  bool left_arc(deprel_t deprel, StateItem & other) const;

  /**
   */
  bool right_arc(deprel_t deprel);

  /**
   */
  bool right_arc(deprel_t deprel, StateItem & other) const;
};

} //  end for namespace

} //  end for namespace ZGen
#endif  //  end for __SR_STATE_HPP__
