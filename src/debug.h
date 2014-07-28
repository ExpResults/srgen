#ifndef __SR_DEBUG_H__
#define __SR_DEBUG_H__

#include "engine.h"
#include <boost/log/trivial.hpp>

static void
debug_word_engine() {
  // Dump the word engine.
  BOOST_LOG_TRIVIAL(trace) << "DEBUGING SR::WordEngine";

  for (int i = 0; i < SR::WordEngine::get_mutable_instance().id2name.size(); ++ i) {
    BOOST_LOG_TRIVIAL(trace) << SR::WordEngine::get_const_instance().id2name[i];
  }

  BOOST_LOG_TRIVIAL(trace) << "Number of entries: "
    << SR::WordEngine::get_const_instance().grand_id;

  BOOST_LOG_TRIVIAL(trace) << "Number of entries: "
    << SR::WordEngine::get_const_instance().id2name.size();

}

#endif  //  end for __SR_DEBUG_H__
