#ifndef __SR_STATE_HPP__
#define __SR_STATE_HPP__

#include <iostream>
#include <bitset>
#include <cstring>

#include "settings.h"
#include "action.h"

namespace SR {

class StateItem {

public:
  // The default constructor,
  StateItem()
    : sentence_ref(0),
    N(0) {
    clear();
  }

  // The constructor with sentence reference.
  StateItem(const sentence_t * _sentence_ref)
    : sentence_ref(_sentence_ref),
    N(_sentence_ref->size()) {
    clear();
  }

  /**
   * Set the sentence reference to the current StateItem, it won't
   * check whether the reference is seted.
   *
   *  @param[in] sentence_ref The pointer to the sentence
   */
  void set_sentence_reference(const sentence_t * _sentence_ref);

  /**
   * Clear the state item.
   */
  void clear();

public:
  // Operators
  StateItem & operator = (const StateItem & item);

  // less than
  bool operator < (const StateItem & other) const;

  bool operator > (const StateItem & other) const;

  // access
  int stack_top() const;

public:
  int N;  // The total number of word for thesentence.
  int C;  // The position current processed.

  const sentence_t    * sentence_ref;
  // The reference to the sentence

  const StateItem * previous;

  floatval_t score;         /*< The score for the current state */

  std::vector<int> stack;   /*< The stack for the current state */

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
   *  @param[in]  postag_id The index of the PoSTag.
   *  @param[in]  word_id   The index to the word.
   *  @return     bool      If successfully shift, return true. 
   *                        otherwise false.
   */
  bool shift(postag_t postag,
      word_t word);

  /**
   * Perform shift on current state and result in a new state.
   *
   *
   */
  bool shift(postag_t postag_id, word_t word, StateItem & other) const; 

  bool left_arc(deprel_t deprel);

  bool left_arc(deprel_t deprel, StateItem & other) const;

  bool right_arc(deprel_t deprel);

  bool right_arc(deprel_t deprel, StateItem & other) const;
};

} //  end for namespace

#endif  //  end for __SR_STATE_HPP__
