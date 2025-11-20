cmake_minimum_required(VERSION 3.20)

project(gbemu LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GB_TARGET_DESKTOP "Build desktop raylib+ImGui frontend" ON)
option(GB_TARGET_WASM "Build WebAssembly frontend" OFF)
option(GB_BUILD_TESTS "Build tests" ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    set(EM_FLAGS "-sASYNCIFY")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EM_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EM_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EM_FLAGS}")
endif()

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/raylib)

add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)

add_library(rlImGui STATIC
    external/rlImGui/rlImGui.cpp
)

target_include_directories(rlImGui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)

target_link_libraries(rlImGui PUBLIC
    raylib
    imgui
)

add_library(gb_core
    src/core/gb_core.c
)

target_include_directories(gb_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(GB_TARGET_DESKTOP)
    add_executable(gbemu_desktop
        src/frontend/raylib_desktop/main.cpp
    )

    target_link_libraries(gbemu_desktop
        PRIVATE
            gb_core
            rlImGui
    )

    target_include_directories(gbemu_desktop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()

if(GB_TARGET_WASM)
    add_executable(gbemu_web
        src/frontend/raylib_desktop/main.cpp
    )

    target_link_libraries(gbemu_web
        PRIVATE
            gb_core
            rlImGui
    )

    target_include_directories(gbemu_web PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    set_target_properties(gbemu_web PROPERTIES OUTPUT_NAME "index")
endif()

if(GB_BUILD_TESTS AND NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    enable_testing()

    add_executable(gb_tests
        tests/test_smoke.c
    )

    target_link_libraries(gb_tests
        PRIVATE
            gb_core
    )

    target_include_directories(gb_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    add_test(NAME gb_smoke COMMAND gb_tests)
endif()

