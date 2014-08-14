#include <cstring>
#include "types/state.h"
#include "types/engine.h"
#include "types/action.h"

namespace ZGen {

namespace ShiftReduce {

std::ostream& operator << (std::ostream& os, const StateItem& item) {
  os << "ADDRESS:   " << (void *)(&item)  << std::endl;
  os << "PREV ADDR: " << (void *)(&item)  << std::endl;
  os << "SCORE:     " << item.score       << std::endl;
  return os;
}


StateItem& StateItem::operator = (const StateItem& other) {
  copy(other);
  return *(this);
}


bool StateItem::operator < (const StateItem& other) const {
  return score < other.score;
}


bool StateItem::operator > (const StateItem& other) const {
  return score > other.score;
}


int StateItem::stack_top() const {
  if (stack.size() < 1) {
    return -1;
  }
  return stack.back();
}


void StateItem::set_instance_reference(const dependency_t* ref) {
  instance_ref = ref;
  N = ref->size();
}


void StateItem::clear() {
  score = 0;

  previous = 0;

  // left_arced.reset();

  buffer.set();     // fill all words in buffer

  stack.clear();    // clear words in stack

  // Push a BEGIN symbol onto the stack.
  stack.push_back(-1);

  word_sequence.clear();
  //

  word_sequence.push_back(WordEncoderAndDecoder::BEGIN);
  //

  postag_sequence.clear();
  //

  postag_sequence.push_back(PoSTagEncoderAndDecoder::BEGIN);
  //

  //
  memset(rank, 0, sizeof(rank));

  // Set up heads to no-head
  memset(heads, -1, sizeof(heads));

  // Set up deprel to no-relation
  memset(deprels, 0, sizeof(deprels));

  // Set up postags to no-postags
  memset(postags, 0, sizeof(postags));

  // Set up number of left- and right-children to zero.
  memset(nr_left_children, 0, sizeof(nr_left_children));

  memset(nr_right_children, 0, sizeof(nr_right_children));

  memset(nr_left_descendant, 0, sizeof(nr_left_descendant));

  memset(nr_right_descendant, 0, sizeof(nr_right_descendant));

  // Set up the left-most and right-most child
  memset(left_most_child, -1, sizeof(left_most_child));

  memset(right_most_child, -1, sizeof(right_most_child));

  // Set up the left-2nd-most and right-2nd-most child
  memset(left_2nd_most_child, -1, sizeof(left_2nd_most_child));

  memset(right_2nd_most_child, -1, sizeof(right_2nd_most_child));
}


void StateItem::copy(const StateItem & other) {
  N = other.N;

  instance_ref = other.instance_ref;

  previous = other.previous;

  stack = other.stack;

  word_sequence = other.word_sequence;

  postag_sequence = other.postag_sequence;

  // left_arced = other.left_arced;

  buffer = other.buffer;

  score = other.score;

  last_action = other.last_action;

  #define _COPY(name) memcpy((name), other.name, sizeof(name));
  _COPY(rank);
  _COPY(postags);
  _COPY(heads);
  _COPY(deprels);
  _COPY(left_most_child);
  _COPY(left_2nd_most_child);
  _COPY(right_most_child);
  _COPY(right_2nd_most_child);
  _COPY(nr_left_children);
  _COPY(nr_right_children);
  _COPY(nr_left_descendant);
  _COPY(nr_right_descendant);
  #undef _COPY
}


bool StateItem::shift(postag_t label, word_t word, int index) {
  if (index < 0 || index >= instance_ref->size()) {
    return false;
  }

  if (false == buffer.test(index)) {
    return false;
  }

  // Push the word onto the stack
  stack.push_back(index);

  //std::cout << (*instance_ref) << std::endl;

  // Append the word and postag to the certain sequence.
  // Mainly handle the phrase.
  for (int i = instance_ref->phrases.at(index).first;
      i < instance_ref->phrases.at(index).second;
      ++ i) {
    word_sequence.push_back( instance_ref->words.at(i) );
    postag_sequence.push_back( label );
  }

  // Erase this word.
  buffer.set(index, 0);

  //
  rank[index] = word_sequence.size();

  // Set up the postag.
  postags[index] = label;

  return true;
}


bool StateItem::shift(postag_t label, word_t word, int index,
    StateItem & other) const {
  other.copy((*this));
  bool ret = other.shift(label, word, index);
  if (ret) {
    other.previous = this;
    other.last_action = action::action_t(ActionEncoderAndDecoder::SH, label, word, index);
  } else {
    other.clear();
  }

  return ret;
}


bool StateItem::left_arc(deprel_t deprel) {
  if (stack.size() < 3) {
    // less than two valid element and a pseudo element
    return false;
  }

  // Backup the stack top nodes
  int stack_size = stack.size();
  int top0 = stack[stack_size - 1];
  int top1 = stack[stack_size - 2];

  // Maintain the stack structure
  stack.pop_back();
  stack.back() = top0;

  // Link (top1 <- top0)
  heads[top1] = top0;
  deprels[top1] = deprel;

  // left_arced.set(top0, 1);

  // Main left child.
  if (left_most_child[top0] == -1) {

    // TP0 is left-isolate node
    left_most_child[top0] = top1;
  } else if (top1 < left_most_child[top0]) {

    // (TP1, LM0, TP0)
    left_2nd_most_child[top0] = left_most_child[top0];
    left_most_child[top0] = top1;
  } else if (top1 < left_2nd_most_child[top0]) {

    // (LM0, TP1, TP0)
    left_2nd_most_child[top0] = top1;
  }

  ++ nr_left_children[top0];
  nr_left_descendant[top0] += (nr_left_descendant[top1]
      + nr_right_descendant[top1]
      + (instance_ref->phrases[top1].second - instance_ref->phrases[top1].first));
  return true;
}

bool StateItem::left_arc(deprel_t deprel, StateItem & other) const {
  other.copy((*this));
  bool ret = other.left_arc(deprel);

  if (ret) {
    other.previous = this;
    other.last_action = action::action_t(ActionEncoderAndDecoder::LA, deprel, 0);
  } else {
    other.clear();
  }

  return ret;
}

bool StateItem::right_arc(deprel_t deprel) {
  if (stack.size() < 3) {
    //
    return false;
  }

  //
  int stack_size = stack.size();
  int top0 = stack[stack_size - 1];
  int top1 = stack[stack_size - 2];

  // Maintain the stack structure
  stack.pop_back();

  heads[top0] = top1;
  deprels[top0] = deprel;

  // Maintain left child.
  if (right_most_child[top1] == -1) {
    // TP1 is right-isolate node
    right_most_child[top1] = top0;
  } else if (right_most_child[top1] < top0) {
    // TP1, LM1, TP0
    right_2nd_most_child[top1] = right_most_child[top1];
    right_most_child[top1] = top0;
  } else if (right_2nd_most_child[top1] < top0) {
    // TP1, TP0, LM1
    right_2nd_most_child[top1] = top0;
  }

  ++ nr_right_children[top1];
  nr_right_descendant[top1] += (nr_right_descendant[top0] 
      + nr_left_descendant[top0]
      + (instance_ref->phrases[top0].second - instance_ref->phrases[top0].first));
  return true;
}

bool StateItem::right_arc(deprel_t deprel, StateItem & other) const {
  other.copy((*this));
  bool ret = other.right_arc(deprel);
  
  if (ret) {
    other.previous = this;
    other.last_action = action::action_t(ActionEncoderAndDecoder::RA, deprel, 0);
  } else {
    other.clear();
  }
  return ret;
}

} //  end for namespace ShiftReduce

} //  end for namespace
