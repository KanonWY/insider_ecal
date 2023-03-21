cmake_minimum_required(VERSION 3.16)


project(TEST)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")

add_executable(main main.cc)


add_executable(testshared testsharedMemory.cpp)
target_link_libraries(testshared rt)

add_executable(testVector testVector.cpp)
target_link_libraries(testVector pthread)


# find_package(PkgConfig REQUIRED)
# if(PkgConfig_FOUND)
#     message(STATUS "--------------------------------------------------------------------------------->")
#     message(STATUS "======> USE [[ pkg_check_modules ]] <======")
#     pkg_check_modules(UNWIND REQUIRED IMPORTED_TARGET libunwind)
#     if(UNWIND_FOUND)
#         message(STATUS "======> FOUND UNWIND")
#         message(STATUS "======> UNWIND VERSION = ${UNWIND_VERSION}")
#         message(STATUS "======> UNWIND INCLUDE_DIRS = ${UNWIND_INCLUDE_DIRS}")
#         message(STATUS "======> UNWIND LIBRARIES = ${UNWIND_LIBRARIES}")
#         message(STATUS "======> UNWIND LIBRARY_DIRS = ${UNWIND_LIBRARY_DIRS}")
#     endif()

# endif()

add_executable(testtrace testtrace.cpp)
target_link_libraries(testtrace unwind)


add_executable(udp_send UDP_send.cpp)

add_executable(udp_receive UDP_receive.cpp)