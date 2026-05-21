cmake_minimum_required(VERSION 3.20)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── SFML 3 (installed via MSYS2) ──────────────────────────────────────────────
set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# ── Source files ──────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/core/Game.cpp
    src/core/Snake.cpp
    src/core/Food.cpp
    src/ui/Renderer.cpp
)

add_executable(SnakeGame ${SOURCES})

target_include_directories(SnakeGame PRIVATE src)

target_link_libraries(SnakeGame
    SFML::Graphics
    SFML::Window
    SFML::System
)

# ── Copy assets next to the executable after every build ─────────────────────
add_custom_command(TARGET SnakeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:SnakeGame>/assets
    COMMENT "Copying assets..."
)
