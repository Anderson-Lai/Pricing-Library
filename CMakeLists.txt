cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project(add_project_name)
set(PROJECT_NAME add_project_name)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/project/")

#find_library(CURL_LIB NAMES curl REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/curl/")
#target_link_libraries(${PROJECT_NAME} ${CURL_LIB})

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different
	#"${CMAKE_CURRENT_SOURCE_DIR}/lib/curl/libcurl.dll"
	#<:TARGET_FILE_DIR:${PROJECT_NAME}>
#)