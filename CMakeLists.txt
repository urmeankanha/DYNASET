cmake_minimum_required(VERSION 3.16)
project(DevToolsCalc VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME devtools_calc)

# ── Sources ───────────────────────────────────────────────────────────────────
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ── Find SDL2 ─────────────────────────────────────────────────────────────────
find_package(SDL2 REQUIRED)

# ── Build target ─────────────────────────────────────────────────────────────
add_executable(${TARGET_NAME} src/main.cpp ${IMGUI_SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_INCLUDE_DIRS}
)

# ── Platform linking ──────────────────────────────────────────────────────────
if(WIN32)
    # -lmingw32 and SDL2main must come BEFORE SDL2 for the MinGW linker
    target_link_libraries(${TARGET_NAME} PRIVATE
        -lmingw32
        ${SDL2MAIN_LIBRARY}
        ${SDL2_LIBRARIES}
        opengl32
        -mwindows
    )
    target_compile_definitions(${TARGET_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    find_library(COCOA_LIB Cocoa REQUIRED)
    find_library(IOKIT_LIB IOKit REQUIRED)
    find_library(COREVIDEO_LIB CoreVideo REQUIRED)
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${SDL2_LIBRARIES}
        "-framework OpenGL"
        ${COCOA_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB}
    )
else()
    # Linux
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${SDL2_LIBRARIES}
        GL dl pthread
    )
    target_compile_options(${TARGET_NAME} PRIVATE -O2 -Wall)
endif()

message(STATUS "")
message(STATUS "  DevTools Calculator Suite  v${PROJECT_VERSION}")
message(STATUS "  Platform   : ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Target     : ${TARGET_NAME}")
message(STATUS "")
