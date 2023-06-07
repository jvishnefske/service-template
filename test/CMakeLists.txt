include(GoogleTest)
enable_testing()
add_executable(test test.cpp)
target_link_libraries(test)

add_executable(benchmark benchmark.cpp)
find_package(benchmark CONFIG REQUIRED)
target_link_libraries(benchmark PRIVATE benchmark::benchmark benchmark::benchmark_main)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(test PRIVATE GTest::gtest_main GTest::gmock backend)
gtest_discover_tests(test)
