cmake_minimum_required(VERSION 3.13)
project(pi-home-lafvin-ui C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99) # LVGL officially supports C99 and above
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# =========================
# LVGL CONFIG (external)
# =========================

set(LV_CONF_PATH "${CMAKE_SOURCE_DIR}/lv_conf.h")

if(NOT EXISTS "${LV_CONF_PATH}")
    message(FATAL_ERROR
        "lv_conf.h not found!\n"
        "Run ./generate_lv_conf.sh before building."
    )
endif()

message(STATUS "Using lv_conf.h: ${LV_CONF_PATH}")

# ========== LVGL ==========
add_subdirectory(lvgl)

# ========== App Sources ==========
file(GLOB_RECURSE SQUARELINE_SOURCES
    src/gui/squareline/*.c
)

set(APP_SOURCES
    src/gui/gui.c
    src/main.c
    ${SQUARELINE_SOURCES}
    src/drivers/display/fbdev/fbdev.c
    src/drivers/display/display.c
    src/drivers/evdev/evdev.c
    src/drivers/lvgl_driver.c
    pi-home-utils/model/model.c
)

# ========== Executable ==========
add_executable(pi-home-lafvin-ui ${APP_SOURCES})

# ========== Includes ==========
target_include_directories(pi-home-lafvin-ui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/display/fbdev
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/display
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/evdev
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/squareline
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/squareline/components
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/squareline/screens
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/squareline/images
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/squareline/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/pi-home-utils/model
)

# Ensure lv_conf.h is found
target_compile_definitions(pi-home-lafvin-ui PRIVATE LV_CONF_INCLUDE_SIMPLE)

target_link_libraries(pi-home-lafvin-ui
    lvgl
    pthread
    m
    stdc++
)

# ========== Install ==========
install(TARGETS pi-home-lafvin-ui
    RUNTIME DESTINATION bin
)
