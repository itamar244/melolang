cmake_minimum_required(VERSION 3.5)

if(NOT TARGET MeloLang)
	set(CMAKE_CXX_COMPILE clang++-6.0)
	set(CMAKE_CXX_STANDARD 17)

	file(GLOB_RECURSE MeloLang_SRC PRIVATE src/*.cc)

	add_subdirectory(lib/atic)

	include_directories(include)
	add_library(MeloLang SHARED ${MeloLang_SRC})

	target_include_directories(MeloLang
		INTERFACE include
		PRIVATE src
	)

	target_link_libraries(MeloLang PRIVATE Atic)
endif()
