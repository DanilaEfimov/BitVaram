cmake_minimum_required(VERSION 3.12)
project(BitVaram LANGUAGES CXX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("E:/libraries/boost_1_87_0")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/detail") #for templates

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(BitVaram ${SOURCES})
set_target_properties(BitVaram PROPERTIES OUTPUT_NAME "Derpy")
set_target_properties(BitVaram PROPERTIES PREFIX "")

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
    set_property(TARGET BitVaram PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(BitVaram PRIVATE libboost_program_options-vc143-mt-gd-x64-1_87)
target_link_libraries(BitVaram PRIVATE libboost_regex-vc143-mt-gd-x64-1_87)

# TODO: Добавьте тесты и дополнительные цели, если нужно.
