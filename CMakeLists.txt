cmake_minimum_required (VERSION 2.8.0)
project ("shift-reduce")

find_package (Boost REQUIRED)

add_definitions (-DBOOST_LOG_DYN_LINK)

set (CMAKE_CXX_FLAGS "-g")
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (SOURCE_DIR             ${PROJECT_SOURCE_DIR}/src)

include_directories (${SOURCE_DIR})

link_directories (${Boost_LIBRARY_DIRS})

add_executable (srg
    ${SOURCE_DIR}/types/engine.cc
    ${SOURCE_DIR}/types/action.cc
    ${SOURCE_DIR}/types/constraint.cc
    ${SOURCE_DIR}/types/tree.cc
    ${SOURCE_DIR}/types/instance.cc
    ${SOURCE_DIR}/types/state.cc
    ${SOURCE_DIR}/model/weight.cc
    ${SOURCE_DIR}/util/utils.cc
    ${SOURCE_DIR}/extract/extractor.cc
    ${SOURCE_DIR}/extract/full_with_guidance_extractor.cc
    ${SOURCE_DIR}/io/io.cc
    ${SOURCE_DIR}/pipe/pipe.cc
    ${SOURCE_DIR}/pipe/none.cc
    ${SOURCE_DIR}/pipe/postag.cc
    ${SOURCE_DIR}/pipe/full.cc
    ${SOURCE_DIR}/pipe/full_with_guidance.cc
    ${SOURCE_DIR}/zgen/srg.cc)

add_executable (unittest
    ${SOURCE_DIR}/types/instance.cc
    ${SOURCE_DIR}/types/engine.cc
    ${SOURCE_DIR}/util/utils.cc
    ${SOURCE_DIR}/unittest/test.cc)

target_link_libraries (srg
    boost_program_options
    boost_log_setup
    boost_log
    boost_regex
    boost_serialization
    boost_thread
    boost_system
    pthread)

target_link_libraries (unittest
    boost_serialization
    pthread)
