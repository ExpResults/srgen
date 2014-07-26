#ifndef __SR_SCORE_MAP_H__
#define __SR_SCORE_MAP_H__

#include <boost/unordered_map.hpp>

#include "settings.h"
#include "score.h"

namespace SR {

typedef boost::unordered_map<UnigramScore, param_t> UnigramScoreMap;
typedef boost::unordered_map<BigramScore, param_t> BigramScoreMap;
typedef boost::unordered_map<TrigramScore, param_t> TrigramScoreMap;

}

#endif  //  end for __SR_SCORE_MAP_H__
