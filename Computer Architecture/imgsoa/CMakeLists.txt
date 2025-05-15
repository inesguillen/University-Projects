add_library(imgsoa
        files.cpp.hpp
        imagesoa.cpp)

target_link_libraries(imgsoa PUBLIC common PRIVATE Microsoft.GSL::GSL)