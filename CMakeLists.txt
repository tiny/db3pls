cmake_minimum_required(VERSION 3.10)
project(test_db3pls)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX OR APPLE)
    add_definitions(-DLINUX -DUNIX)
endif() 

add_executable(${PROJECT_NAME} 
    src/main_testdb3.cpp
    src/db3pls.cpp
)

target_link_libraries(${PROJECT_NAME} )

