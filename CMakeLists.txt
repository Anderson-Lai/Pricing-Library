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
project(pricinglibrary)
set(PROJECT_NAME pricinglibrary)

# dependencies
find_package(CURL REQUIRED)

# set c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include all .cpp files
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# include all .h, .hpp, and template files
target_include_directories(${PROJECT_NAME} 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public/"
	PRIVATE 
		"${CMAKE_CURRENT_SOURCE_DIR}/include/private/project/"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/private/json/"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/private/templates/"
)

target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl)

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		$<$<CONFIG:Debug>:/MDd>
		$<$<CONFIG:Release>:/MDd>
	)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -pthread)
endif()