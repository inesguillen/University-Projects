add_library(common
        progargs.hpp progargs.cpp
        otherfile.cpp
        #../utest-img/test-compress.cpp
)
target_link_libraries(common PRIVATE Microsoft.GSL::GSL)