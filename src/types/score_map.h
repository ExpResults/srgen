#ifndef __SR_SCORE_MAP_H__
#define __SR_SCORE_MAP_H__

#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "score.h"
#include "param.h"

namespace ZGen {

namespace ShiftReduce {

typedef std::unordered_map<UnigramScore, param_t, boost::hash<UnigramScore> > UnigramScoreMap;
typedef std::unordered_map<BigramScore,  param_t, boost::hash<BigramScore>  > BigramScoreMap;
typedef std::unordered_map<TrigramScore, param_t, boost::hash<TrigramScore> > TrigramScoreMap;

typedef UnigramScoreMap us_map_t;
typedef BigramScoreMap  bs_map_t;
typedef TrigramScoreMap ts_map_t;

}

}
#endif  //  end for __SR_SCORE_MAP_H__
