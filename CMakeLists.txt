project(deberg)

cmake_minimum_required(VERSION 2.8.8)

add_definitions(-std=c++11)

include_directories(${CMAKE_SOURCE_DIR}/third_party/)

set(BOOST_COMPONENTS unit_test_framework)

#Check Boost
set(BOOST_MIN_VERSION "1.49.0")
find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Fatal error: Boost (version >= 1.49.0) required.\n")
endif()
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_TEST_DYN_LINK)

add_subdirectory(./third_party/tinyxml2)
add_subdirectory(./third_party/glm)

SET(TESTS_SOURCE tests/tests.cpp tests/reader_test.cpp tests/geometry_tests.cpp tests/tangent_splitter_tests.cpp  tests/sweepline_tests.cpp tests/point_distributor_tests.cpp tests/shortcut_acceptor_tests.cpp tests/static_permutation_queue_tests.cpp)
SET(LIBRARY_SOURCE tangent_splitter.cpp point_distributor.cpp sweepline_state.cpp shortcut_acceptor.cpp)
add_executable(tests ${TESTS_SOURCE} ${LIBRARY_SOURCE})
target_link_libraries(tests ${Boost_LIBRARIES} tinyxml2)


