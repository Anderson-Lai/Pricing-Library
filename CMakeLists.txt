cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# target vcpkg
if (DEFINED ENV{VCPKG_ROOT_DIRECTORY})
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT_DIRECTORY}/scripts/buildsystems/vcpkg.cmake")
else ()
	message(FATAL_ERROR "Environment variable VCPKG_ROOT_DIRECTORY is not defined. Point it to where vcpkg is installed")
endif()

# project name
project(add_project_name)
set(PROJECT_NAME add_project_name)

# dependencies
# find_package(CURL REQUIRED)

# set c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include all .cpp files
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

# include all .h and .hpp files
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/project/")

#find_library(CURL_LIB NAMES curl REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/curl/")
#target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIB} CURL::libcurl)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different
	#"${CMAKE_CURRENT_SOURCE_DIR}/lib/curl/libcurl.dll"
	#<:TARGET_FILE_DIR:${PROJECT_NAME}>
#)