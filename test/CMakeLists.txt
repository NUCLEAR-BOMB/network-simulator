include("${PROJECT_SOURCE_DIR}/cmake/compiler_warnings.cmake")

add_executable(network-simulator-test
    "packet.test.cpp"
    "packet_pull.test.cpp"
    "client.test.cpp"
    "ip_client.test.cpp"
)

target_add_warnings(network-simulator-test)

target_link_libraries(network-simulator-test PRIVATE network-simulator-lib)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
    URL_HASH SHA256=ffa17fbc5953900994e2deec164bb8949879ea09b411e07f215bfbb1f87f4632
    FIND_PACKAGE_ARGS NAMES GTest
)
set(gtest_force_shared_crt TRUE CACHE BOOL "" FORCE)
set(BUILD_GMOCK FALSE)
set(INSTALL_GTEST FALSE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(network-simulator-test PRIVATE GTest::gtest GTest::gtest_main)

include("${PROJECT_SOURCE_DIR}/cmake/sanitizers.cmake")
target_add_sanitizer(network-simulator-test gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(network-simulator-test)