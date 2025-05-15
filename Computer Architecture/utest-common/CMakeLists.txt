add_executable(test-progargs
        test-progargs.cpp
)

target_link_libraries(test-progargs PRIVATE common GTest::gtest_main Microsoft.GSL::GSL)

add_test(NAME test-progargs COMMAND test-progargs)