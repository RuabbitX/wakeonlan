cmake_minimum_required(VERSION 3.28)
project(wakeonlan)

set(CMAKE_CXX_STANDARD 23)

add_executable(wakeonlan main.cpp
)


# C++23已经拥有实现了
#find_package(fmt CONFIG REQUIRED)
#target_link_libraries(wakeonlan PRIVATE fmt::fmt)

message(STATUS "CMAKE Version:${CMAKE_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "COMPILER: ${CMAKE_CXX_COMPILER_ID}")
