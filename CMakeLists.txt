cmake_minimum_required(VERSION 3.20)

project(Network VERSION 0.1 LANGUAGES CXX)

file(GLOB SRC_FILES CONFIGURE_DEPENDS *.cpp)

add_compile_definitions(NEAPU_NETWORK_EXPORTS)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_NAME}_static SHARED ${SRC_FILES})

if(WIN32)
target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
target_link_libraries(${PROJECT_NAME}_static PUBLIC ws2_32)
endif()