cmake_minimum_required (VERSION 2.8.0)
project ("shift-reduce")

find_package (Boost REQUIRED)

add_definitions (-DBOOST_LOG_DYN_LINK)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (SOURCE_DIR             ${PROJECT_SOURCE_DIR}/src)

link_directories (${Boost_LIBRARY_DIRS})

add_executable (srg
    ${SOURCE_DIR}/engine.cc
    ${SOURCE_DIR}/action.cc
    ${SOURCE_DIR}/extractor.cc
    ${SOURCE_DIR}/io.cc
    ${SOURCE_DIR}/pipe.cc
    ${SOURCE_DIR}/state.cc
    ${SOURCE_DIR}/srg.cc)

#target_link_libraries (srg
#    ${Boost_LIBRARY_DIRS}/libboost_program_options.a
#    ${Boost_LIBRARY_DIRS}/libboost_log_setup.a
#    ${Boost_LIBRARY_DIRS}/libboost_log.a
#    ${Boost_LIBRARY_DIRS}/libboost_regex.a
#    ${Boost_LIBRARY_DIRS}/libboost_thread.a
#    ${Boost_LIBRARY_DIRS}/libboost_system.a
#    pthread)

target_link_libraries (srg
    boost_program_options
    boost_log_setup
    boost_log
    boost_regex
    boost_thread
    boost_system
    pthread)
