#include "pipe.h"
#include "state.h"
#include <algorithm>

#include <boost/log/trivial.hpp>
#include <boost/assert.hpp>

namespace ZGen {

namespace ShiftReduce {

static bool TransitionHeapMore (const Pipe::scored_transition_t & x,
    const Pipe::scored_transition_t & y) {
  return x.get<2>() > y.get<2>();
}


Pipe::Pipe(int beam_size)
  : max_beam_size(beam_size) {
  // Allocate a very large lattice.
  if (max_beam_size > kMaxBeamSize) {
    max_beam_size = kMaxBeamSize;
  }
  lattice = new StateItem[kMaxSteps * kMaxBeamSize];
  BOOST_LOG_TRIVIAL(info) << "PIPE: lattice[" << kMaxSteps * kMaxBeamSize << "] allocated";
}


Pipe::~Pipe() {
}


bool Pipe::save_model(const char * prefix) {
  std::string chunk(prefix);

  return (save_word_engine((chunk + ".word").c_str()) &&
      weight.save_weight((chunk + ".weight").c_str()));
}


void Pipe::finish_training(int now) {
  weight.flush_weight(now);
}


bool Pipe::load_model(const char * prefix) {
  std::string chunk(prefix);

  return (load_word_engine((chunk + ".word").c_str()) &&
      weight.load_weight((chunk + ".weight").c_str()));
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


void Pipe::clear_candidate_transition() {
  for (int i = 0; i < max_beam_size; ++ i) {
    candidate_transitions[i].get<2>() = -inf;
  }
}


void Pipe::transit(const StateItem & from,
    const action::action_t & act,
    floatval_t score,
    StateItem & to) {

  if (action::is_shift(act)) {
    from.shift(act.label, act.word, act.index, to);
    to.score = score;

  } else if (action::is_left_arc(act)) {
    from.left_arc(act.label, to);
    to.score = score;

  } else if (action::is_right_arc(act)) {
    from.right_arc(act.label, to);
    to.score = score;

  } else {
    BOOST_LOG_TRIVIAL(warning) << "UNKNOWN action : " << act.prefix;
  }
}


bool Pipe::collect_state_chain_and_update_score(
    const StateItem * predicted_final_state,
    const StateItem * correct_final_state,
    int now, floatval_t add, floatval_t sub) {
  const StateItem * predicted_state_chain[kMaxSteps];
  const StateItem * correct_state_chain[kMaxSteps];

  int nr_predicated_states = 0;
  int nr_correct_states = 0;

  for (const StateItem * p = predicted_final_state; p; p = p->previous) {
    predicted_state_chain[nr_predicated_states] = p;
    nr_predicated_states ++;
  }

  for (const StateItem * p = correct_final_state; p; p = p->previous) {
    correct_state_chain[nr_correct_states] = p;
    nr_correct_states ++;
  }

  if (nr_predicated_states != nr_correct_states) {
    BOOST_LOG_TRIVIAL(warning) << "Number of predicated states("
      << nr_predicated_states << ")"
      << " is different from correct states(" << nr_correct_states << ")";
    return false;
  }

  // The stored action chain is in reversed order. Take a action chain with
  // 4 states, the order is [4], [3], [2], [1]

  BOOST_LOG_TRIVIAL(trace) << "PREDICATED STATE CHAIN";
  for (int i = nr_predicated_states - 1; i > 0; -- i) {
    BOOST_LOG_TRIVIAL(trace) << " - [" << i << "]: "
      << (void *)predicted_state_chain[i]
      << " to "
      << (void *)predicted_state_chain[i - 1]
      << " "
      << predicted_state_chain[i - 1]->last_action;
  }

  BOOST_LOG_TRIVIAL(trace) << "CORRECT STATE CHAIN";
  for (int i = nr_correct_states - 1; i > 0; -- i) {
    BOOST_LOG_TRIVIAL(trace) << " - [" << i << "]: "
      << (void *)correct_state_chain[i]
      << " to "
      << (void *)correct_state_chain[i - 1]
      << " "
      << correct_state_chain[i - 1]->last_action;
  }

  int i;
  for (i = nr_correct_states - 1; i >= 0; -- i) {
    if (predicted_state_chain[i]->last_action != correct_state_chain[i]->last_action) {
      break;
    }
  }

  BOOST_LOG_TRIVIAL(trace) << "START updating at POSITION [" << i << "]";

  for (i = i + 1; i > 0; -- i) {
    action::action_t predict_action = predicted_state_chain[i- 1]->last_action;
    action::action_t correct_action = correct_state_chain[i- 1]->last_action;

    update_state_score((*predicted_state_chain[i]), predict_action, now, sub);
    update_state_score((*correct_state_chain[i]), correct_action, now, add);
  }

  return true;
}


StateItem * Pipe::search_correct_state(
    const action::action_t & act,
    const StateItem * previous_correct_state,
    StateItem * lattice_begin,
    StateItem * lattice_end) {

  StateItem * correct_state = NULL;

  if (!action::is_shift(act)) {
    for (StateItem * p = lattice_begin; p != lattice_end; ++ p) {
      if (p->previous == previous_correct_state && p->last_action == act) {
        correct_state = p;
        break;
      }
    }
  } else {
    // SHIFT-FIRST PASS
    for (StateItem * p = lattice_begin; p != lattice_end; ++ p) {
      if (p->previous == previous_correct_state &&
          p->last_action == act &&
          p->last_action.index == act.index) {
        correct_state = p;
        break;
      }
    }

    if (NULL == correct_state) {
      // Perform the second pass
      for (StateItem * p = lattice_begin; p!= lattice_end; ++ p) {
        if (p->previous == correct_state &&
            p->last_action == act) {
          int shifted_index = p->last_action.index;
          int expected_shifted_index = act.index;

          if (!p->buffer.test(expected_shifted_index)) {
            BOOST_LOG_TRIVIAL(warning) << "#" << timestamp
              << ": The expected shifted index should be empty";
            break;
          }

          p->buffer.set(expected_shifted_index, 0);
          // Remove the gold indexed word from the buffer
          p->buffer.set(shifted_index, 1);
          // Recover the predicated shifted word from the buffer

          p->postags[expected_shifted_index] = p->postags[shifted_index];
          p->postags[shifted_index] = 0;

          p->stack.back() = expected_shifted_index;
          p->last_action = act;

          correct_state = p;
          break;
        }
      }
    }
  }

  return correct_state;
}


const StateItem * Pipe::search_best_state(const StateItem * begin,
    const StateItem * end) {
  const StateItem * best = begin;
  for (const StateItem * p = begin+ 1; p != end; ++ p) {
    if (p->score > best->score) {
      best = p;
    }
  }
  return best;
}


void Pipe::work(const dependency_t & input,
    const std::vector<action::action_t> & gold_actions,
    dependency_t & output,
    int now) {
  config_sentence(input, now);

  int N = input.forms.size();

  // GET whether perform training.
  // If gold action is provide, perform training on the current instance.
  bool train_mode = (gold_actions.size() == N * 2 - 1);
  if (train_mode) {
    BOOST_LOG_TRIVIAL(trace) << "TRAIN MODE activated.";
  } else {
    BOOST_LOG_TRIVIAL(trace) << "TEST MODE activated.";
  }

  // BOOST_LOG_TRIVIAL(debug) << N;
  int steps = (N << 1) - 1;

  // Perpare the lattic
  BOOST_LOG_TRIVIAL(trace) << "INIT the starting state.";

  lattice[0].clear();
  lattice[0].set_sentence_reference(&(input.forms));

  lattice_index[0] = lattice;
  lattice_index[1] = lattice + 1;

  // Set the correct state link.
  StateItem* correct_state = lattice;

  int step;
  for (step = 1; step <= steps; ++ step) {
    // generate state from states in step(i-1) to step(i)
    BOOST_LOG_TRIVIAL(trace) << "|||||||||||| ROUND : " << step << " |||||||||||||";

    int current_beam_size = 0;

    clear_candidate_transition();

    for (StateItem * from = lattice_index[step- 1]; from != lattice_index[step]; ++ from) {
      BOOST_LOG_TRIVIAL(trace) << "EXTEND from state [" << (void *)from << "]";

      get_possible_actions((*from), possible_actions);

      // Pack all the score
      get_state_packed_score((*from), possible_actions, packed_score);

      for (int i = 0; i < possible_actions.size(); ++ i) {
        const action::action_t & act = possible_actions[i];
        BOOST_LOG_TRIVIAL(trace) << " - Possible #" << i << ": "
          << act << ", " << from->score + packed_score[act];

        current_beam_size += extend_candidate_transition(
            scored_transition_t(from, act, from->score + packed_score[act]),
            current_beam_size, max_beam_size);
      }
    }

    // Apply transition.
    BOOST_LOG_TRIVIAL(trace) << "||||| APPLYING TRANSITION";
    for (int i = 0; i < current_beam_size; ++ i) {
      const scored_transition_t & trans = candidate_transitions[i];
      BOOST_LOG_TRIVIAL(trace) << (void *)trans.get<0>()
        << " to " << (void *)(lattice_index[step] + i)
        << ": " << trans.get<1>()
        << ", " << trans.get<2>();

      transit((*trans.get<0>()), trans.get<1>(), trans.get<2>(), (*(lattice_index[step]+i)));
    }

    lattice_index[step + 1] = lattice_index[step] + current_beam_size;

    BOOST_LOG_TRIVIAL(trace) << "[" << current_beam_size << "] is inserted into beam";

    if (current_beam_size == 0) {
      BOOST_LOG_TRIVIAL(warning) << "#" << now << ": CURRENT Beam size is ZERO!";
    }

    if (train_mode) {
      //
      StateItem * next_correct_state = search_correct_state(
          gold_actions[step- 1], correct_state,
          lattice_index[step], lattice_index[step+ 1]);

      bool correct_state_in_beam = (next_correct_state != NULL);

      if (!correct_state_in_beam) {
        BOOST_LOG_TRIVIAL(trace) << "Correct state fallout beam at from #"
          << step - 1 << " to #" << step;

        const StateItem * best_to = search_best_state(lattice_index[step],
            lattice_index[step+ 1]);

        transit((*correct_state), gold_actions[step-1], 0, (*lattice_index[step+1]));

        // [Update state
        if (!collect_state_chain_and_update_score(best_to,
              lattice_index[step+1], now, 1, -1)) {
          BOOST_LOG_TRIVIAL(warning) << "#" << now
            << ": Failed to update score (cond i)";
        }
        return;
      } else {
        correct_state = next_correct_state;
      }
    }
  }

  const StateItem * best_to = search_best_state(lattice_index[step- 1], lattice_index[step]);

  if (train_mode) {
    if (best_to != correct_state) {
      if (!collect_state_chain_and_update_score(best_to, correct_state, now, 1, -1)) {
        BOOST_LOG_TRIVIAL(warning) << "#" << now<< "Failed to update score (cond ii)";
      }
    }
    return;
  }

  std::vector<int> order;
  for (const StateItem * p = best_to; p->previous; p = p->previous) {
    if (action::is_shift(p->last_action)) {
      order.push_back(p->last_action.index);
    }
  }

  output.clear();
  if (order.size() != N) {
    BOOST_LOG_TRIVIAL(warning) << "#" << now << ": ORDER is not equal to the sent size";
    BOOST_LOG_TRIVIAL(warning) << "order size : " << order.size() << " N : " << N;

    output.push_back(0, 0, 0, 0);
    return;
  }

  const sentence_t & sentence = input.forms;
  std::reverse(order.begin(), order.end());
  for (int i = 0; i < N; ++ i) {
    int j = order[i];
    output.push_back(sentence[j],
        best_to->postags[j],
        best_to->heads[j],
        best_to->deprels[j]);
  }
}


// The NonePipe
NonePipe::NonePipe(const char * postag_dict_path,
    int beam_size)
  : constraint(postag_dict_path),
  Pipe(beam_size) {
}


int NonePipe::get_possible_actions(const StateItem & item,
    std::vector<action::action_t> & actions) {
  actions.clear();

  // Then loop over the words in the buffer.
  for (int j = 0; j < item.N; ++ j) {
    if (item.buffer.test(j)) {

      // Generate all possible SHIFT actions, first loop over possible PoSTags.
      const char * name = WordEngine::get_const_instance().decode(item.sentence_ref->at(j));
      std::vector< postag_t > possible_tags;
      constraint.get_possible_tags(name, possible_tags);

      for (int i = 0; i < possible_tags.size(); ++ i) {
        postag_t tag = possible_tags[i];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag,
              (*item.sentence_ref)[j], j));
      }
    }
  }

  if (item.stack.size() > 2) {
    // There is one pseudo node in the stack
    // Generate LEFT-ARC and RIGHT-ARC actions
    for (int i = kStartIndexOfValidDependencyRelation;
        i < kNumberOfDependencyRelations; ++ i) {
      actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, i, 0));
      actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, i, 0));
    }
  }

  return (int)actions.size();
}


int NonePipe::config_sentence(const dependency_t & input, int now) {
  timestamp = now;
  input_ref = &input;
}


PoSTagPipe::PoSTagPipe(int beam_size)
  : Pipe(beam_size) {
}


int PoSTagPipe::get_possible_actions(const StateItem & item,
    action_collection_t & actions) {
  actions.clear();

  // Then loop over the words in the buffer.
  for (int j = 0; j < item.N; ++ j) {
    if (item.buffer.test(j)) {
      word_t   word = input_ref->forms[j];
      postag_t  tag = input_ref->postags[j];
      actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
    }
  }

  if (item.stack.size() > 2) {
    // There is one pseudo node in the stack
    // Generate LEFT-ARC and RIGHT-ARC actions
    for (int i = kStartIndexOfValidDependencyRelation;
        i < kNumberOfDependencyRelations; ++ i) {
      actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, i, 0));
      actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, i, 0));
    }
  }

  return (int)actions.size();

}

int PoSTagPipe::config_sentence(const dependency_t & input, int now) {
  timestamp = now;
  input_ref = &input;
}


FullPipe::FullPipe(int beam_size)
  : Pipe(beam_size) {
}

FullPipe::~FullPipe() {
  BOOST_LOG_TRIVIAL(error) << "PIPE: Should not be deallocate";
}

//
int FullPipe::get_possible_actions(const StateItem & item,
    std::vector<action::action_t> & actions) {
  actions.clear();

  if (item.stack.size() == 1) {
    for (int j = 0; j < item.N; ++ j) {
      if (item.buffer.test(j)) {
        word_t   word = input_ref->forms[j];
        postag_t  tag = input_ref->postags[j];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, j));
      }
    }
  } else {
    bool all_descendants_shifted = true;
    const DependencyTree::edgeset_t& descendants = cache.descendants(item.stack.back());

    for (int j = 0; j < descendants.size(); ++ j) {
      int d = descendants[j];
      if (item.buffer.test(d)) {
        all_descendants_shifted = false;
        break;
      }
    }

    if (!all_descendants_shifted) {
      for (int j = 0; j < descendants.size(); ++ j) {
        int d = descendants[j];
        if (item.buffer.test(d)) {
          word_t word = input_ref->forms[d];
          postag_t tag = input_ref->postags[d];
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, d));
        }
      }
    } else {
      int top0 = item.stack.back();
      int top1 = (item.stack.size() > 2 ? item.stack[item.stack.size() - 2]: -1);

      if (top1 >= 0 && cache.arc(top0, top1)) {
        deprel_t deprel = input_ref->deprels[top1];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::LA, deprel, 0));
      } else if (top1 >= 0 && cache.arc(top1, top0)) {
        deprel_t deprel = input_ref->deprels[top0];
        actions.push_back(action::action_t(ActionEncoderAndDecoder::RA, deprel, 0));
      } else {
        const DependencyTree::edgeset_t& siblings = cache.siblings(item.stack.back());

        for (int i = 0; i < siblings.size(); ++ i) {
          int s = siblings[i];
          if (item.buffer.test(s)) {
            word_t word = input_ref->forms[s];
            postag_t tag = input_ref->postags[s];
            actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, s));
          }
        }

        int h = cache.head(item.stack.back());
        if (item.buffer.test(h)) {
          word_t word = input_ref->forms[h];
          postag_t tag = input_ref->postags[h];
          actions.push_back(action::action_t(ActionEncoderAndDecoder::SH, tag, word, h));
        }
      }
    }
  }

  return (int)actions.size();
}


int FullPipe::config_sentence(const dependency_t & input,
    int now) {
  timestamp = now;
  input_ref= &input;

  cache.set_ref(&input);
  return 0;
}


}

}
