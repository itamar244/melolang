cmake_minimum_required(VERSION 3.5)

if(NOT TARGET MeloLang)
	include_directories(include)

	file(GLOB_RECURSE MeloLang_SRC PRIVATE src/*.cc)

	add_subdirectory(lib/atic)

	add_library(MeloLang SHARED ${MeloLang_SRC})

	target_include_directories(MeloLang INTERFACE include)

	target_link_libraries(MeloLang PRIVATE Atic)
endif()
