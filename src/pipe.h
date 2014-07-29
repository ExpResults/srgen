#ifndef __SR_PIPE_H__
#define __SR_PIPE_H__

#include "action.h"
#include "state.h"
#include "settings.h"
#include "weight.h"

namespace SR {

class Pipe {
public:
  Pipe(int beam_size);

  typedef std::vector<action::action_t> action_collection_t;
  // a list of actions;

  typedef std::vector<action::action_t> action_sequence_t;
  //

  typedef boost::unordered_map<action::action_t, floatval_t>  packed_score_t;
  //

  typedef boost::tuples::tuple<StateItem *, action::action_t, floatval_t> scored_transition_t;
  //

  /**
   *
   *
   *
   *
   */
  void work(const sentence_t & input,
      const std::vector<action::action_t> & gold_actions,
      dependency_t & output,
      int now);


  void work(const sentence_with_postag_t & input,
      const std::vector<action::action_t> & gold_actions,
      dependency_t & output,
      int now);


  void work(const unordered_dependency_t & input,
      const std::vector<action::action_t> & gold_actions,
      dependency_t & output,
      int now);

  //
  void finish_training(int now);

  //
  bool save_model(const char * prefix);

  //
  bool load_model(const char * prefix);

private:

  /**
   * Get possible actions from the input item.
   *
   *  @param[in]  item          The input item
   *  @param[out] actions       The possible actions
   *  @return     int           The number of possible actons
   */
  int get_possible_actions(const StateItem & item,
      action_collection_t & actions);

  /**
   * Packed the score
   *
   *  @param[in]  item          The StateItem * item
   *  @param[in]  actions       The possible_actions
   *  @param[out] score         The packed score
   *
   */
  int get_state_packed_score(const StateItem & item,
      const action_collection_t & possible_actions,
      packed_score_t & score);

  /**
   *
   *
   *
   */
  int update_state_score(const StateItem & item,
      const action::action_t & act, int now, int scale);

  /**
   * Insert an item into the beam.
   *
   *  @param[out] beam_wrapper  The wrapper for the beam.
   *  @param[in]  item          The beam-item
   *  @param[in]  current_beam_size The current size of beam
   *  @param[in]  max_beam_size   The max number of items in beam.
   *  @return     int           If an element is inserted, return 1.
   *                            Otherwise return 0.
   */
  int insert_into_beam(StateItem ** lattice_wrapper,
      const StateItem * item,
      const int current_beam_size,
      const int max_beam_size);

  /**
   *
   *
   *
   *
   */
  int extend_candidate_transition(const scored_transition_t & trans,
      const int current_beam_size,
      const int max_beam_size);

  /**
   *
   *
   *
   *
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
   *
   *
   */
   bool collect_state_chain_and_update_score(const StateItem * predicted_state,
      const StateItem * correct_state, int now, floatval_t add, floatval_t sub);

private:
  int max_beam_size;

  StateItem * lattice;
  StateItem * lattice_index[kMaxSteps];

  action_sequence_t possible_actions;

  packed_score_t packed_score;

  weight_t weight;

  scored_transition_t candidate_transitions[kMaxBeamSize];
};

}

#endif  //  end for __SR_PIPE_H__
