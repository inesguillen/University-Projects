add_library(imgaos
        files.cpp.hpp
        imageaos.cpp
)

target_link_libraries(imgaos PUBLIC common PRIVATE Microsoft.GSL::GSL)