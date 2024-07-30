cmake_minimum_required(VERSION 3.20)

include(FetchContent)

# use vcpkg
if (DEFINED ENV{VCPKG_ROOT_DIRECTORY})
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT_DIRECTORY}/scripts/buildsystems/vcpkg.cmake")
else ()
	message("Environment variable VCPKG_ROOT_DIRECTORY is not defined. Fetching dependencies from source")
endif()

# project name
project(pricinglibrary)

# fetching dependencies
if (DEFINED ENV{VCPKG_ROOT_DIRECTORY})
	find_package(CURL REQUIRED)
else()
	FetchContent_Declare(
		CURL 
		GIT_REPOSITORY https://github.com/curl/curl.git
		GIT_TAG master
	)
	FetchContent_MakeAvailable(CURL)
endif()

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
	target_compile_options(${PROJECT_NAME} PUBLIC 
		$<$<CONFIG:Release>:/MD>
		$<$<CONFIG:Debug>:/MDd>
	)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${PROJECT_NAME} PUBLIC -pthread)
endif()