cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project("NetworkSimulator" CXX)

file(GLOB_RECURSE PROJECT_FILES CONFIGURE_DEPENDS 
	"${CMAKE_SOURCE_DIR}/header/*.hpp"
	"${CMAKE_SOURCE_DIR}/source/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

target_compile_features	  (${PROJECT_NAME} PUBLIC "cxx_std_14")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/header")

target_compile_options(${PROJECT_NAME} PUBLIC
	$<IF:$<CXX_COMPILER_ID:MSVC>,
		/W4 /WX
	,
		-Wall -Wextra -pedantic -Werror
	>
)
