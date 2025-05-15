add_executable(utest-img
        test-maxlevelSOA.cpp
        tests-info.cpp
        test-compress.cpp)

target_link_libraries(utest-img PRIVATE imgaos imgsoa GTest::gtest_main Microsoft.GSL::GSL)

add_test(NAME utests-img COMMAND utest-img)