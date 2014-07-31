#ifndef __SR_SCORE_MAP_H__
#define __SR_SCORE_MAP_H__

#include <boost/unordered_map.hpp>

#include "score.h"
#include "param.h"

namespace ZGen {

namespace ShiftReduce {

typedef boost::unordered_map<UnigramScore, param_t> UnigramScoreMap;
typedef boost::unordered_map<BigramScore, param_t> BigramScoreMap;
typedef boost::unordered_map<TrigramScore, param_t> TrigramScoreMap;

typedef UnigramScoreMap us_map_t;
typedef BigramScoreMap  bs_map_t;
typedef TrigramScoreMap ts_map_t;

}

}
#endif  //  end for __SR_SCORE_MAP_H__
