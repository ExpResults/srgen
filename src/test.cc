#include <iostream>
#include <fstream>
#include <bitset>

#include "engine.h"
#include "score.h"
#include "param.h"
#include "weight.h"
#include "settings.h"
#include "utils.h"
#include "serialization/unordered_map.h"

#include <boost/unordered_map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace SR = ZGen::ShiftReduce;

// Test 1' The serialization test.
void serialize_unordered_map() {
  boost::unordered_map<std::string, int> m;

  m["a"] = 1;
  m["b"] = 2;

  std::ofstream ofs("pickle");
  boost::archive::text_oarchive oa(ofs);
  oa << m;
  ofs.close();

  boost::unordered_map<std::string, int> m2;
  std::ifstream ifs("pickle");
  boost::archive::text_iarchive ia(ifs);
  ia >> m2;
  ifs.close();

  std::cout << m2["b"] << std::endl;
}

// Test 2' test dumping the word engine.
void word_engine_dump_test() {
  SR::WordEngine::get_mutable_instance().insert("  ");
  SR::WordEngine::get_mutable_instance().insert("==2==");

  SR::save_word_engine("pickle");
  SR::load_word_engine("pickle");

  std::cout << SR::WordEngine::get_const_instance().encode("  ") << std::endl;
  std::cout << SR::WordEngine::get_const_instance().decode(4) << std::endl;
}


// Test 3' test the word engine's function.
void word_engine_test() {
#define REF(name) SR::WordEngine::get_mutable_instance().insert(name)
  int a[] = {
    REF("In"),
    REF("__an_Oct._19_review__"),
    REF("of"),
    REF("``"),
    REF("__The_Misanthrope__"),
    REF("''"),
    REF("at"),
    REF("__Chicago_'s__"),
    REF("Goodman"),
  };

  for (int i = 0; i < 8; ++ i) {
    SR::action::action_t act(
        SR::ActionEncoderAndDecoder::SH,
        SR::PoSTagEncoderAndDecoder::NP,
        a[i],
        0);
    std::cout << act << std::endl;
  }
}


// Test 4' test the scored map
void scored_map_test() {
  SR::us_map_t m1;
  SR::action::action_t act1(0, 0, 0);
  SR::action::action_t act2(1, 1, 1);

  SR::us_t key1(1, act1);
  SR::us_t key2(2, act2);

  m1[key1] = SR::param_t(0, 1, 2);
  m1[key2] = SR::param_t(3, 4, 5);

  std::cout << m1[key2].w_sum << std::endl;

  std::ofstream ofs("pickle");
  boost::archive::text_oarchive oa(ofs);
  oa << m1;
  ofs.close();

  //
  SR::us_map_t m2;
  std::ifstream ifs("pickle");
  boost::archive::text_iarchive ia(ifs);
  ia >> m2;
  ifs.close();

  std::cout << m2[key2].w_sum << std::endl;
}

// Test 5' 
void postags_engine_test() {
  for (int i = SR::kStartIndexOfValidPoSTag; i < SR::kNumberOfPoSTags; ++ i) {
    std::cout << 
      SR::PoSTagEngine::get_const_instance().decode(i) << std::endl;
  }
}

// Test 6' Shuffle test
void shuffle_test() {
  namespace SR = ZGen::ShiftReduce;
  SR::dependency_t input;

  input.push_back(
      SR::WordEngine::get_mutable_instance().insert("a"),
      SR::PoSTagEngine::get_const_instance().encode("NN"),
      1,
      SR::DeprelEngine::get_const_instance().encode("NMOD"));

  input.push_back(
      SR::WordEngine::get_mutable_instance().insert("b"),
      SR::PoSTagEngine::get_const_instance().encode("VB"),
      -1,
      SR::DeprelEngine::get_const_instance().encode("HEAD"));

  input.push_back(
      SR::WordEngine::get_mutable_instance().insert("c"),
      SR::PoSTagEngine::get_const_instance().encode("NN"),
      1,
      SR::DeprelEngine::get_const_instance().encode("VMOD"));


  std::cout << input;

  SR::dependency_t output;
  std::vector<int> order;

  shuffle_instance(input, output, order);

  for (int i = 0; i < order.size(); ++ i) {
    std::cout << order[i] << ", ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << output;
}


int main() {
  shuffle_test();
  return 0;
}

