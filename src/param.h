#ifndef __SR_PARAM_H__
#define __SR_PARAM_H__

#include "settings.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace SR {

struct Parameter {
  Parameter()  : w(0), w_sum(0), w_time(0) {}

  Parameter(floatval_t _w, floatval_t _w_sum, floatval_t _w_time) 
    : w(_w), w_sum(_w_sum), w_time(_w_time) {}

  floatval_t  w;
  floatval_t  w_sum;
  int         w_time;

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & w & w_sum & w_time;
  }
};

typedef Parameter param_t;

}

#endif  //  end for __SR_PARAM_H__
