cmake_minimum_required(VERSION 3.14)
set(target nrfProxy)
project(${target})

set(CMAKE_CXX_STANDARD 14)

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(libconfig++ REQUIRED)
find_package(Boost 1.40 COMPONENTS filesystem program_options thread REQUIRED)
find_library(LIB_CONFIG++ config++)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

if (libconfig++_FOUND)
    message(STATUS "libconfig++ found, version: ${LIBCONFIG++_VERSION}")
endif ()

link_directories(${libconfig++_LIBDIR})
include_directories(${libconfig++_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#set(PROTO_MODELS ${CMAKE_CURRENT_SOURCE_DIR}/src/Models/Proto/)
#file(GLOB DATAMODEL_PROTOS "${PROTO_MODELS}/*.proto")
#
#find_program(GRPC_PLUGIN_EXECUTABLE grpc_cpp_plugin)
#
#if (GRPC_PLUGIN_EXECUTABLE)
#    message(STATUS "Success found grpc plugin: ${GRPC_PLUGIN_EXECUTABLE}")
#endif ()
#
#foreach (proto ${DATAMODEL_PROTOS})
#    FILE(TO_NATIVE_PATH ${proto} proto_native)
#    EXECUTE_PROCESS(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} -I ${PROTO_MODELS} --grpc_out=${PROTO_MODELS} --plugin=protoc-gen-grpc=${GRPC_PLUGIN_EXECUTABLE} ${proto_native}
#            RESULT_VARIABLE rv)
#    EXECUTE_PROCESS(COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} -I ${PROTO_MODELS} --cpp_out=${PROTO_MODELS} ${proto_native})
#    # Optional, but that can show the user if something have gone wrong with the proto generation
#    if (${rv})
#        MESSAGE("Generation of data model returned ${rv} for proto ${proto_native}")
#    endif ()
#endforeach (proto)
#
#file(GLOB PROTO_SRCS "${PROTO_MODELS}/*.cc")
#file(GLOB PROTO_HDRS "${PROTO_MODELS}/*.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_executable(${target} src/main.cpp src/Server.cpp src/Server.h src/Session.cpp src/Session.h)
target_link_libraries(${target} ${Boost_LIBRARIES})
target_link_libraries(${target} ${LIB_CONFIG++})
