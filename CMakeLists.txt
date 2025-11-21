cmake_minimum_required(VERSION 3.15)
project(FastDDS_Middleware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(fastcdr REQUIRED)
    find_package(fastdds 3 REQUIRED)
    include_directories(
    ${CMAKE_SOURCE_DIR}/src 
    ${CMAKE_SOURCE_DIR}/data
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    include_directories(
    ${CMAKE_SOURCE_DIR}/src 
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_SOURCE_DIR}/FastDDS/include 
    )
    link_directories(${CMAKE_SOURCE_DIR}/FastDDS/lib)
endif()

add_library(FastDDS_Middleware SHARED
    ${CMAKE_SOURCE_DIR}/data/sampleDataTypeObjectSupport.cxx
    ${CMAKE_SOURCE_DIR}/data/sampleDataPubSubTypes.cxx
    ${CMAKE_SOURCE_DIR}/src/DataDistributionAPI.cpp
    ${CMAKE_SOURCE_DIR}/src/DataDistributionImplFactoryExt.cpp
     ${CMAKE_SOURCE_DIR}/src/DataReceiveListenerImplExt.cpp
     ${CMAKE_SOURCE_DIR}/src/ParticipantDev.cpp
     ${CMAKE_SOURCE_DIR}/src/ParticipantAPI.cpp
     ${CMAKE_SOURCE_DIR}/src/ParticipantAPIImpl.cpp

)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   target_link_libraries(FastDDS_Middleware fastcdr fastdds)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(FastDDS_Middleware 
    foonathan_memory-0.7.3.lib libcrypto.lib libfastcdr-2.3.lib
    libfastdds-3.2.lib libssl.lib Shlwapi.lib)
endif()

target_compile_definitions(FastDDS_Middleware PRIVATE DATADISTRIBUTIONCONFIG_EXPORT=value)
set_target_properties(FastDDS_Middleware PROPERTIES DEBUG_OUTPUT_NAME "FastDDS_Middleware_d")
set_target_properties(FastDDS_Middleware PROPERTIES RELEASE_OUTPUT_NAME "FastDDS_Middleware")


#TEST.EXE
add_executable(testexe test.cpp)
target_link_directories(testexe PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(testexe debug FastDDS_Middleware_d)
target_link_libraries(testexe optimized FastDDS_Middleware)
set_target_properties(testexe PROPERTIES RELEASE_OUTPUT_NAME "testexe")
set_target_properties(testexe PROPERTIES DEBUG_OUTPUT_NAME "testexe_d")




