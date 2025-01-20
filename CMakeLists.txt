cmake_minimum_required(VERSION 3.22)
project(lux_2021)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(lux_2021 main.cpp)

set_target_properties(lux_2021 PROPERTIES OUTPUT_NAME "lux")
add_custom_command(TARGET lux_2021 POST_BUILD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND lux-ai-2021 lux.exe lux.exe --out=replay.json
)