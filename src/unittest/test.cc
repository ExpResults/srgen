#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

#include "io/io.h"
#include "types/tree.h"
#include "types/engine.h"
#include "types/score.h"
#include "types/param.h"
#include "types/instance.h"
#include "types/settings.h"
#include "util/utils.h"
#include "model/weight.h"
#include "serialization/unordered_map.h"

#include <boost/unordered_map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ZGenSR
#include <boost/test/unit_test.hpp>

using namespace ZGen::ShiftReduce;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(suite1)

// Test 1' The serialization test.
BOOST_AUTO_TEST_CASE( test_serialize_unordered_map ) {
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

  BOOST_CHECK_EQUAL( 2, m2["b"] );
}


// Test 2' test dumping the word engine.
BOOST_AUTO_TEST_CASE( word_engine_dump_test ) {
  WordEngine::get_mutable_instance().insert("  ");
  WordEngine::get_mutable_instance().insert("==2==");

  save_word_engine("pickle");
  load_word_engine("pickle");

  BOOST_CHECK_EQUAL( 3, WordEngine::get_const_instance().encode("  ") );
  BOOST_CHECK_EQUAL( 4, WordEngine::get_const_instance().encode("==2==") );
  BOOST_CHECK_EQUAL( -1, WordEngine::get_const_instance().encode("!!") );
}

// Test 3'
BOOST_AUTO_TEST_CASE( scored_map_test ) {
  us_map_t m1;
  action::action_t act1(0, 0, 0);
  action::action_t act2(1, 1, 1);

  us_t key1(1, act1);
  us_t key2(2, act2);

  m1[key1] = param_t(0, 1, 2);
  m1[key2] = param_t(3, 4, 5);

  std::ofstream ofs("pickle");
  boost::archive::text_oarchive oa(ofs);
  oa << m1;
  ofs.close();

  //
  us_map_t m2;
  std::ifstream ifs("pickle");
  boost::archive::text_iarchive ia(ifs);
  ia >> m2;
  ifs.close();

  BOOST_CHECK_EQUAL( 4, m2[key2].w_sum );
  BOOST_CHECK_EQUAL( 3, m2[key2].w );
}


BOOST_AUTO_TEST_CASE( instance_test ) {
  std::stringstream S;
  S << "a NP 1 NMOD" << std::endl
    << "b VBZ -1 ROOT" << std::endl
    << "c NP 1 NMOD" << std::endl;

  std::vector< dependency_t > data;
  read_from_dep(S, data);

  const dependency_t& inst = data[0];

  BOOST_CHECK_EQUAL( inst.size(), 3);

  DependencyTree tree;
  tree.set_ref(&inst);

  BOOST_CHECK_EQUAL(2,  tree.descendants(1).size());
}


BOOST_AUTO_TEST_CASE( dependency_tree_with_guidance_test ) {
  std::stringstream S;
  S << "0 NP  -1 ROOT" << std::endl  // 0
    << "1 VBZ 0  P"    << std::endl  // 1
    << "2 NP  1  NMOD" << std::endl  // 2
    << "3 NP  2  VMOD" << std::endl  // 3
    << "4 NP  3  PMOD" << std::endl  // 4
    << "5 NP  6  OBJ"  << std::endl  // 5
    << "6 NP  2  SBAR" << std::endl  // 6
    << "7 NP  6  PRD"  << std::endl;

  std::vector< dependency_t > data;
  read_from_dep(S, data);

  const dependency_t& inst = data[0];

  DependencyTreeWithGuidance tree;
  tree.set_ref(&inst);

  BOOST_CHECK_EQUAL( tree.lvl2(5), tree.lvl2(7) );
  BOOST_CHECK_EQUAL( tree.lvl1(4), tree.lvl1(7) );
}

BOOST_AUTO_TEST_CASE( dependency_forest_test ) {
  std::stringstream S;
  S << "0 NP  1  P" << std::endl  // 0
    << "1 VBZ -1 NONE"    << std::endl  // 1
    << "2 NP  3  NMOD" << std::endl  // 2
    << "3 NP  -1 NONE" << std::endl;

  std::vector< dependency_t > data;
  read_from_dep(S, data);

  const dependency_t& inst = data[0];

  DependencyForest forest;
  forest.set_ref(&inst);

  BOOST_CHECK_EQUAL( forest.root(0), 1 );
  BOOST_CHECK_EQUAL( forest.root(1), 1 );
  BOOST_CHECK_EQUAL( forest.root(2), 3 );
  BOOST_CHECK_EQUAL( forest.root(3), 3 );
}


BOOST_AUTO_TEST_SUITE_END()

