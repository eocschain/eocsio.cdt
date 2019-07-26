configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lemon-cc.cpp.in ${CMAKE_BINARY_DIR}/lemon-cc.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lemon-cpp.cpp.in ${CMAKE_BINARY_DIR}/lemon-cpp.cpp)

add_tool(lemon-cc)
add_tool(lemon-cpp)
