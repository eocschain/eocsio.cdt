file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(lemon
            eosiolib.cpp
            crypto.cpp
            ${HEADERS})

add_native_library(native_lemon
                   eosiolib.cpp
                   crypto.cpp
                   ${HEADERS})

target_include_directories(lemon PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 {CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_include_directories(native_lemon PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 ${CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( lemon c c++ )
target_link_libraries( native_lemon native_c native_c++ )
add_dependencies( native_lemon lemon )


add_custom_command( TARGET lemon POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lemon> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_lemon POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_lemon> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../eosiolib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
