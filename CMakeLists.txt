cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE MeloLang_SRC INTERFACE src/*.cc)

add_library(MeloLang SHARED ${MeloLang_SRC})

target_include_directories(MeloLang INTERFACE include)
