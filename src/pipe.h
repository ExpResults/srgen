#ifndef __SR_PIPE_H__
#define __SR_PIPE_H__

#include "action.h"
#include "state.h"
#include "settings.h"
#include "weight.h"
#include "constraint.h"
#include "dependency.h"

namespace ZGen {

namespace ShiftReduce {

// The basic pipe class
class Pipe {
public:
  typedef std::vector<action::action_t> action_collection_t;
  // a list of actions;

  typedef std::vector<action::action_t> action_sequence_t;
  //

  typedef boost::unordered_map<action::action_t, floatval_t>  packed_score_t;
  //

  typedef boost::tuples::tuple<StateItem *, action::action_t, floatval_t> scored_transition_t;
  //

public:
  Pipe(int beam_size);

  virtual ~Pipe();

  /**
   * Perform tree linearization
   *
   *  @param[in]  input         The unordered full tree.
   *  @param[in]  gold_actions  The oracle action.
   *  @param[out] output        The output dependency parse
   *  @param[in]  now           The time sequence factor, used in updating parameter.
   */
  void work(const unordered_dependency_t & input,
            const action_sequence_t & gold_actions,
            dependency_t & output,
            int now);

  //
  void finish_training(int now);

  //
  bool save_model(const char * prefix);

  //
  bool load_model(const char * prefix);

protected:

  /**
   * Get possible actions from the input item.
   *
   *  @param[in]  item          The input item
   *  @param[out] actions       The possible actions
   *  @return     int           The number of possible actons
   */
  virtual int get_possible_actions(const StateItem & item,
      action_collection_t & actions) = 0;


  /**
   * The interface for configuring the sentence onto the pipe.
   * It provides flexiable interface for handling cache.
   *
   *  @param[in]  sentence    The input sentence.
   *  @param[in]  now         The timestamp
   */
  virtual int config_sentence(const dependency_t & sentence, int now) = 0;


  // The timestamp for currently processed instance.
  int timestamp;

  // The reference to the currently processed instance.
  const dependency_t* input_ref;

private:
  /**
   * Packed the score
   *
   *  @param[in]  item      The StateItem * item
   *  @param[in]  actions   The possible_actions
   *  @param[out] score     The packed score
   *
   */
  int get_state_packed_score(const StateItem & item,
      const action_collection_t & possible_actions,
      packed_score_t & score);


  /**
   * Update the score according to the state and action.
   *
   *  @param[in]  item      The StateItem * item
   *  @param[in]  act       The action
   *  @param[in]  now       The timestamp
   *  @param[in]  scale     The scale for score.
   */
  int update_state_score(const StateItem & item,
      const action::action_t & act, int now, int scale);


  /**
   * Insert a candidate transition into the array.
   *
   *  @param[in]  trans               The candidate transition.
   *  @param[in]  current_beam_size   The current size of beam.
   *  @param[in]  max_beam_size       The maximum size of beam.
   */
  int extend_candidate_transition(const scored_transition_t & trans,
      const int current_beam_size,
      const int max_beam_size);


  /**
   * Clear the candidate transition array. Set all the score to infinite.
   */
  void clear_candidate_transition();


  /**
   * Perform the certain transition.
   *
   *  @param[in]  from          The from state
   *  @param[in]  act           The action
   *  @param[out] to            The resulted in state.
   */
  void transit(const StateItem & from,
       const action::action_t & act,
       floatval_t score,
       StateItem & to);

  /**
   * Back-trace from the given state, collect the state chain.
   *
   *  @param[in]  predicted_state   The final predicated state.
   *  @param[in]  correct_state     The final correct state.
   *  @param[in]  now               The timestamp.
   *  @param[in]  add               The increase scale.
   *  @param[in]  sub               The decrease scale.
   */
  bool collect_state_chain_and_update_score(const StateItem * predicted_state,
      const StateItem * correct_state, int now, floatval_t add, floatval_t sub);

  /**
   * Get the correct state in the randomly access. The begin pointer
   * and end pointer should not be a const pointer since it may be
   * modified if act is SHIFT and duplicate work is shifted.
   *
   *  @param[in]  act                 The action
   *  @param[in]  prev_correct_state  The previous correct.
   *  @param[in]  begin               The begin pointer.
   *  @param[in]  end                 The end pointer.
   *  @return     StateItem *
   */
  StateItem * search_correct_state(const action::action_t & act,
      StateItem * previous_correct_state,
      StateItem * begin,
      StateItem * end);

  /**
   * Get the best state in the randomly access.
   *
   *  @param[in]  begin         The begin pointer
   *  @param[in]  end           The end pointer.
   *  @return     StateItem *
   */
  const StateItem * search_best_state(const StateItem * begin,
      const StateItem * end);

private:
  int max_beam_size;
  //

  StateItem * lattice;
  //

  StateItem * lattice_index[kMaxSteps];
  //

  action_sequence_t possible_actions;
  //

  packed_score_t packed_score;
  //

  weight_t weight;
  //

  scored_transition_t candidate_transitions[kMaxBeamSize];
  //
};


//
class NonePipe: public Pipe {
public:
  NonePipe(const char * postag_dict_path,
      int beam_size);

protected:
  /**
   * Get the possible actions from the input item.
   *
   *  @param[in]  item          The input item
   *  @param[out] actions       The possible actions
   *  @return     int           The number of possible actons
   */
  int get_possible_actions(const StateItem & item, action_collection_t & actions);

  /**
   * Config the sentence onto the Pipe
   *
   *  @param[in]  input       The input
   *  @param[in]  now         The now
   */
  int config_sentence(const dependency_t & input, int now);

  constraint_t constraint;
};


//
class PoSTagPipe : public Pipe {
public:
  PoSTagPipe(int beam_size);

protected:
  /**
   * Get the possible actions from the input item.
   *
   *  @param[in]  item          The input item
   *  @param[out] actions       The possible actions
   *  @return     int
   */
  int get_possible_actions(const StateItem & item, action_collection_t & actions);

  /**
   *
   *
   *
   *
   */
  int config_sentence(const dependency_t & input, int now);
};


//
class FullPipe : public Pipe {
public:
  FullPipe(int beam_size);
  ~FullPipe();

protected:
  /**
   *
   *
   *
   *
   */
  int get_possible_actions(const StateItem & item, action_collection_t & actions);

  /**
   * Config the input onto this pipe. This function
   * cache tree structure information in the tree.
   *
   *  @param[in]  input   The input dependency
   *  @param[in]  now     The timestamp
   */
  int config_sentence(const dependency_t & input, int now);

private:
  DependencyTree cache;

};

}

}
#endif  //  end for __SR_PIPE_H__
