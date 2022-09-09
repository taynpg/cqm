cmake_minimum_required(VERSION 3.5)

project(cqm LANGUAGES C)

set(CMAKE_C_STANDARD 99)

MESSAGE(STATUS "system name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "compiler id: ${CMAKE_C_COMPILER_ID}")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    MESSAGE(STATUS "add windows compiler flag")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8 /EHsc")
    # set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/source-charset:utf-8")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    MESSAGE(STATUS "Compiler C_FLAGS:${CMAKE_C_FLAGS}")
endif()

include_directories(include)
aux_source_directory(src SRC_DIR)

add_executable(cqm main.c ${SRC_DIR})

install(TARGETS cqm LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
