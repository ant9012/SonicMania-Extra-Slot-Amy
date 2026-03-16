set(WITH_RSDK OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")

add_executable(${GAME_NAME} 
    ${GAME_NAME}/Game.c
    ${GAME_NAME}/Objects/All.c
)

target_include_directories(${GAME_NAME} PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
)

target_compile_options(${GAME_NAME} PRIVATE -DRETRO_REVISION=3 -DMANIA_PREPLUS=0 -DRETRO_REV02=1 -DRETRO_REV0U=1 -DRETRO_VER_EGS=1 -DRETRO_USE_MOD_LOADER=1)

set_target_properties(${GAME_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set(emsc_link_options
    -sTOTAL_MEMORY=32MB
    -sALLOW_MEMORY_GROWTH=1
    -sWASM=1
    -sLINKABLE=1
    -sEXPORT_ALL=1
    -sSIDE_MODULE=2
    -sSINGLE_FILE=1
    -sBINARYEN_ASYNC_COMPILATION=0
    -sUSE_PTHREADS=1
    -sPTHREAD_POOL_SIZE=4
    -pthread
)

target_link_options(${GAME_NAME} PRIVATE ${emsc_link_options})

set_target_properties(${GAME_NAME} PROPERTIES
    OUTPUT_NAME "Game"
    SUFFIX ".wasm"
)
