cmake_minimum_required(VERSION 3.23)

project(Geode)

add_executable(geode)

target_compile_features(geode
    PRIVATE
        cxx_std_14
)

target_sources(geode
    PRIVATE
        ./src/main.cpp
)
