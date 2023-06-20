file(GLOB CPP_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
message("Module located in directory: ${CPP_FILES}")

# Add the CPP files to your target
add_library(imnotgui STATIC ${CPP_FILES})
target_include_directories(imnotgui PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(imnotgui PUBLIC raylib raylib_cpp)