# Basic CMake that uses All.c
cmake_minimum_required(VERSION 3.7)

project(SonicMania)
set(GAME_NAME SonicMania CACHE STRING "The game directory to look into")

set(OUTPUT_NAME "Game" CACHE STRING "The name of the built library")

if(EMSCRIPTEN)
    # --- FIX START ---
    
    # 1. Point to Jdaslepre's Engine folder (Adjust ".." if your folders are different)
    set(RSDK_ENGINE_DIR "${CMAKE_SOURCE_DIR}/../RSDKv5-Decompilation") 

    # 2. Include the WASM setup (creates the target ${GAME_NAME})
    include(RSDK-Library-GameAPI.cmake)

    # 3. CRITICAL: Tell the compiler to use the Mod Loader system
    # This fixes "Object Class Unimplemented" errors
    target_compile_definitions(${GAME_NAME} PRIVATE 
        -DRETRO_REV02=1
        -DRETRO_REV0U=1 
        -DMANIA_PREPLUS=0
        -DRETRO_USE_MOD_LOADER=1
        -DRETRO_REV02=1
        -DRETRO_REV0U=1
    )

    # 4. Link the Engine Headers so the game knows what 'RSDK::Mod' is
    target_include_directories(${GAME_NAME} PRIVATE
        "${RSDK_ENGINE_DIR}/RSDKv5"
        "${RSDK_ENGINE_DIR}/dependencies/all"
    )
    
    # --- FIX END ---
else()
    add_library(SonicMania SHARED
        ${GAME_NAME}/Game.c
        ${GAME_NAME}/Objects/All.c
    )

    target_include_directories(SonicMania PRIVATE
        ${GAME_NAME}/
        ${GAME_NAME}/Objects/
    )

    # Note: Native build also needs Mod Loader if you want consistency
    target_compile_options(SonicMania PRIVATE 
        -DRETRO_REV02=1
        -DRETRO_REV0U=1 
        -DMANIA_PREPLUS=0
        -DRETRO_USE_MOD_LOADER=1
        -DRETRO_REV02=1
        -DRETRO_REV0U=1
        -DGAME_VERSION=7
        -DMANIA_USE_PLUS=1
    )

    set_target_properties(SonicMania PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
endif()

unset(GAME_NAME CACHE)
unset(OUTPUT_NAME CACHE)