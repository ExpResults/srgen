#include "pipe.h"
#include "state.h"
#include <algorithm>

#include <boost/log/trivial.hpp>
#include <boost/assert.hpp>

namespace SR {

static bool StateHeapMore (const StateItem * x, const StateItem * y) {
  return x->score > y->score;
}


static bool TransitionHeapMore (const Pipe::scored_transition_t & x,
    const Pipe::scored_transition_t & y) {
  return x.get<2>() > y.get<2>();
}


Pipe::Pipe() 
  : max_beam_size(32) {
  // Allocate a very large lattice.
  lattice = new StateItem[kMaxSteps * kMaxBeamSize];
  BOOST_LOG_TRIVIAL(info) << "Pipe.lattice["
    << kMaxSteps * kMaxBeamSize << "] allocated";
}


int Pipe::insert_into_beam(StateItem ** beam_wrapper,
    const StateItem * item,
    const int current_beam_size,
    const int max_beam_size) {

  if (current_beam_size == max_beam_size) {
    // If the beam is full and the smallest element is smaller than the input
    // element, replace this element with the input element.
    if ((*item) > (*(*beam_wrapper))) {
      StateItem * p = beam_wrapper[0];
      std::pop_heap(beam_wrapper, beam_wrapper + max_beam_size, StateHeapMore);
      (*p) = (*item);
      beam_wrapper[max_beam_size - 1] = p;
      std::push_heap(beam_wrapper, beam_wrapper + max_beam_size, StateHeapMore);
    }
    return 0;
  }

  **(beam_wrapper + current_beam_size) = *(item);
  std::push_heap(beam_wrapper, beam_wrapper + current_beam_size + 1, StateHeapMore);
  return 1;
}


int Pipe::extend_candidate_transition(const scored_transition_t & trans,
    const int current_beam_size,
    const int max_beam_size) {
  if (current_beam_size == max_beam_size) {
    if (trans.get<2>() > candidate_transitions[0].get<2>()) {
      std::pop_heap(candidate_transitions,
          candidate_transitions + max_beam_size,
          TransitionHeapMore);
      candidate_transitions[max_beam_size-1] = trans;
      std::push_heap(candidate_transitions,
          candidate_transitions + max_beam_size,
          TransitionHeapMore);
    }
    return 0;
  }

  candidate_transitions[current_beam_size] = trans;
  std::push_heap(candidate_transitions,
      candidate_transitions + current_beam_size + 1,
      TransitionHeapMore);
  return 1;
}


int Pipe::get_possible_actions(const StateItem & item,
    std::vector<action::action_t> & actions) {
  actions.clear();

  // Generate all possible SHIFT actions, first loop over possible
  // PoSTags.
  for (int i = kStartIndexOfValidPoSTag;
      i < kNumberOfPoSTags; ++ i) {
    // Then loop over the words in the buffer.
    for (int j = 0; j < item.N; ++ j) {
      if (item.buffer.test(j)) {
        actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, i,
              (*item.sentence_ref)[j]));
      }
    }
  }

  if (item.stack.size() >= 2) {
    // Generate LEFT-ARC and RIGHT-ARC actions
    for (int i = kStartIndexOfValidDependencyRelation;
        i < kNumberOfDependencyRelations; ++ i) {
      actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, i, 0));
      actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, i, 0));
    }
  }

  return (int)actions.size();
}


void Pipe::transit(const StateItem & from,
    const action::action_t & act,
    floatval_t score,
    StateItem & to) {

  if (act.act() == ActionEncoderAndDecoder::SH) {

    from.shift(act.rel(), act.word(), to);
    to.score = score;
  } else if (act.act() == ActionEncoderAndDecoder::LA) {

    from.left_arc(act.rel(), to);
    to.score = score;
  } else if (act.act() == ActionEncoderAndDecoder::RA) {

    from.right_arc(act.rel(), to);
    to.score = score;
  } else {
    BOOST_LOG_TRIVIAL(warning) << "UNKNOWN action : " << act.act();
  }
}


void Pipe::collect_state_chain_and_update_score(
    const StateItem * predicted_final_state,
    const StateItem * correct_final_state,
    int now, floatval_t add, floatval_t sub) {
  const StateItem * predicted_state_chain[kMaxSteps];
  const StateItem * correct_state_chain[kMaxSteps];

  int nr_predicated_states = 0;
  int nr_correct_state = 0;

  for (const StateItem * p = predicted_final_state; p; p = p->previous) {
    predicted_state_chain[nr_predicated_states] = p;
    nr_predicated_states ++;
  }

  for (const StateItem * p = correct_final_state; p; p = p->previous) {
    correct_state_chain[nr_correct_state] = p;
    nr_correct_state ++;
  }

  if (nr_predicated_states != nr_correct_state) {
    BOOST_LOG_TRIVIAL(warning) << "Number of predicated states is different from "
      << "correct states";
    exit(1);
  }

  int i;
  for (i = nr_correct_state - 1; i >= 0; -- i) {
    if (predicted_state_chain[i]->last_action != correct_state_chain[i]->last_action) {
      break;
    }
  }

  for (i = i + 1; i > 0; -- i) {
    action::action_t predict_action = predicted_state_chain[i]->last_action;
    action::action_t correct_action = correct_state_chain[i]->last_action;

    update_state_score((*predicted_state_chain[i]), predict_action, now, sub);
    update_state_score((*correct_state_chain[i]), correct_action, now, add);
  }
}


void Pipe::work(const sentence_t & sentence,
    const std::vector<action::action_t> & gold_actions,
    dependency_t & output,
    int now) {

  // GET whether perform training.
  // If gold action is provide, perform training on the current instance.
  bool train_mode = (gold_actions.size() == sentence.size() * 2 - 1);

  int N = sentence.size();
  int steps = (N << 1) - 1;

  // Perpare the lattic
  BOOST_LOG_TRIVIAL(trace) << "INIT the starting state.";

  lattice[0].clear();
  lattice[0].set_sentence_reference(&sentence);

  lattice_index[0] = lattice;
  lattice_index[1] = lattice + 1;

  // Set the correct state link.
  StateItem * correct_state = lattice;

  for (int step = 1; step < steps; ++ step) {
    // generate state from states in step(i-1) to step(i)
    BOOST_LOG_TRIVIAL(trace) << "ROUND : " << step;

    int current_beam_size = 0;

    for (StateItem * from = lattice_index[step - 1]; from != lattice_index[step]; ++ from) {
      BOOST_LOG_TRIVIAL(trace) << "EXTEND from state [" << (void *)from << "]";

      get_possible_actions((*from), possible_actions);

      // Pack all the score
      get_state_packed_score((*from), possible_actions, packed_score);

      for (int i = 0; i < possible_actions.size(); ++ i) {
        // BOOST_LOG_TRIVIAL(trace) << "possible-actions: " << possible_actions[i];
        const action::action_t & act = possible_actions[i];
        current_beam_size += extend_candidate_transition(
            scored_transition_t(from, act, packed_score[act]),
            current_beam_size, max_beam_size);
      }
    }

    // Apply transition.
    for (int i = 0; i < current_beam_size; ++ i) {
      transit((*candidate_transitions[i].get<0>()),
          candidate_transitions[i].get<1>(),
          candidate_transitions[i].get<2>(),
          (*(lattice_index[step]+i)));
    }

    lattice_index[step + 1] = lattice_index[step] + current_beam_size;

    if (train_mode) {
      //
      bool correct_state_in_beam = false;
      for (StateItem * p = lattice_index[step]; p != lattice_index[step + 1]; ++ p) {
        if (p == correct_state && p->last_action == gold_actions[step]) {
          correct_state = p;
          correct_state_in_beam = true;
          break;
        }
      }

      if (!correct_state_in_beam) {
        BOOST_LOG_TRIVIAL(trace) << "Correct state fallout beam at STEP " << step;
        StateItem * best_to = lattice_index[step];
        for (StateItem * to =lattice_index[step] + 1;
            to != lattice_index[step + 1]; ++ to) {
          if (best_to->score < to->score) {
            best_to = to;
          }
        }

        transit((*correct_state), gold_actions[step], 0, (*lattice_index[step+1]));
        // [Update state
        collect_state_chain_and_update_score(best_to,
            lattice_index[step+1], now, 1, -1);
        return;
      }
    }
  }
}

}
