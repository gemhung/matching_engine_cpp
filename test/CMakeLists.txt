include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

set(MATCHING_ENGINE_TEST_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(MATCHING_ENGINE_TEST_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

include_directories(${MATCHING_ENGINE_TEST_INSTALL_INCLUDE_DIR})
set(SOURCE_FILES ./test.cpp)

enable_testing()

add_executable(
    matching_engine_test
    test.cpp
)

target_link_libraries(
    matching_engine_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(matching_engine_test)

install(TARGETS matching_engine_test DESTINATION ${MATCHING_ENGINE_TEST_INSTALL_BIN_DIR})
