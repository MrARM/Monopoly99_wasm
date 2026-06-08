cmake_minimum_required(VERSION 3.22)

project(monopoly_web_port C CXX)

set(MONOPOLY_PLATFORM "web" CACHE STRING "Target platform: web")
set_property(CACHE MONOPOLY_PLATFORM PROPERTY STRINGS web)

if(NOT MONOPOLY_PLATFORM STREQUAL "web")
    message(FATAL_ERROR "Only MONOPOLY_PLATFORM=web is defined for this port scaffold")
endif()

if(NOT EMSCRIPTEN)
    message(WARNING "MONOPOLY_PLATFORM=web is intended to be configured with emcmake/emcc")
else()
    set(MONOPOLY_EMSCRIPTEN_CACHE "${CMAKE_CURRENT_BINARY_DIR}/.emcache" CACHE PATH "Repo-local Emscripten cache")
    file(MAKE_DIRECTORY "${MONOPOLY_EMSCRIPTEN_CACHE}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E env EM_CACHE=${MONOPOLY_EMSCRIPTEN_CACHE}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E env EM_CACHE=${MONOPOLY_EMSCRIPTEN_CACHE}")
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(MONOPOLY_WEB_UPLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/upload")
file(MAKE_DIRECTORY "${MONOPOLY_WEB_UPLOAD_DIR}")

set(MONOPOLY_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(MONOPOLY_GENERATED_BLTINC "${MONOPOLY_GENERATED_DIR}/L_BLTINC.i")
set(MONOPOLY_CASEFOLD_INCLUDE_DIR "${MONOPOLY_GENERATED_DIR}/casefold")
set(MONOPOLY_CASEFOLD_STAMP "${MONOPOLY_GENERATED_DIR}/casefold.stamp")
set(MONOPOLY_CASEFOLD_INCLUDE_DIRS
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/monopoly"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/monopoly/Dat_Mon"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/monopoly/dat_mon"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/artlib"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/ArtLib"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/PC3D"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/Pc3D"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/pc3D"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/pc3d"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/ZLib"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/zlib"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/web"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/web/compat"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/PC3D/vrml2"
    "${MONOPOLY_CASEFOLD_INCLUDE_DIR}/PC3D/xvrml"
)
file(MAKE_DIRECTORY ${MONOPOLY_CASEFOLD_INCLUDE_DIRS})

set(MONOPOLY_CASEFOLD_INCLUDE_ARGS)
foreach(MONOPOLY_CASEFOLD_INCLUDE_DIR_ENTRY IN LISTS MONOPOLY_CASEFOLD_INCLUDE_DIRS)
    list(APPEND MONOPOLY_CASEFOLD_INCLUDE_ARGS -I "${MONOPOLY_CASEFOLD_INCLUDE_DIR_ENTRY}")
endforeach()

set(MONOPOLY_PREPROCESS_COMMAND ${CMAKE_CXX_COMPILER})
if(EMSCRIPTEN)
    set(MONOPOLY_PREPROCESS_COMMAND
        ${CMAKE_COMMAND} -E env "EM_CACHE=${MONOPOLY_EMSCRIPTEN_CACHE}" ${CMAKE_CXX_COMPILER}
    )
endif()

add_custom_target(monopoly_casefold_include_overlay
    COMMAND ${Python3_EXECUTABLE}
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_casefold_include_overlay.py"
        --source-root "${CMAKE_CURRENT_SOURCE_DIR}"
        --output-root "${MONOPOLY_CASEFOLD_INCLUDE_DIR}"
        --stamp "${MONOPOLY_CASEFOLD_STAMP}"
    BYPRODUCTS "${MONOPOLY_CASEFOLD_STAMP}"
    COMMENT "Generating case-insensitive include overlay"
    VERBATIM
)

add_custom_command(
    OUTPUT "${MONOPOLY_GENERATED_BLTINC}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_GENERATED_DIR}"
    COMMAND ${MONOPOLY_PREPROCESS_COMMAND}
        -E
        -C
        -Wno-nonportable-include-path
        -Wno-invalid-source-encoding
        ${MONOPOLY_CASEFOLD_INCLUDE_ARGS}
        -I "${CMAKE_CURRENT_SOURCE_DIR}"
        -I "${CMAKE_CURRENT_SOURCE_DIR}/monopoly"
        -I "${CMAKE_CURRENT_SOURCE_DIR}/artlib"
        -D MONOPOLY_WEB=1
        -D WIN32=1
        -D _WIN32=1
        -D _WINDOWS=1
        -D DIRECTDRAW_VERSION=0x0600
        -D DIRECTDRAW_VERSION0=0x0600
        "${CMAKE_CURRENT_SOURCE_DIR}/artlib/L_BltInc.cpp"
        -o "${MONOPOLY_GENERATED_BLTINC}"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/artlib/L_BltInc.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/artlib/L_BltCpy.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/artlib/L_BltFil.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/C_ArtLib.h"
    COMMENT "Generating ArtLib blitter include"
    VERBATIM
)

add_custom_target(monopoly_generate_bltinc
    DEPENDS "${MONOPOLY_GENERATED_BLTINC}"
)
add_dependencies(monopoly_generate_bltinc monopoly_casefold_include_overlay)

set(MONOPOLY_AI_STRATEGY_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Barrow.AI"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Boot.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Cannon.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Dog.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Horse.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Iron.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Moneybag.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/RaceCar.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Ship.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/Thimble.ai"
    "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/TopHat.ai"
)

set(MONOPOLY_SOURCES
    monopoly/Ai.cpp
    monopoly/Ai_load.cpp
    monopoly/Ai_trade.cpp
    monopoly/Ai_util.cpp
    monopoly/Debugart.cpp
    monopoly/display.cpp
    monopoly/GameInc.cpp
    monopoly/Lang.cpp
    monopoly/Main.cpp
    monopoly/Mdef.cpp
    monopoly/Mess.cpp
    monopoly/Rule.cpp
    monopoly/TexInfo.cpp
    monopoly/Tickler.cpp
    monopoly/trade.cpp
    monopoly/UDAuct.cpp
    monopoly/UDBoard.cpp
    monopoly/UDChat.cpp
    monopoly/UDIBar.cpp
    monopoly/UDOpts.cpp
    monopoly/Udpenny.cpp
    monopoly/UDPieces.cpp
    monopoly/UDPsel.cpp
    monopoly/UDSound.cpp
    monopoly/UDStats.cpp
    monopoly/UDTrade.cpp
    monopoly/UDUtils.cpp
    monopoly/Unility.cpp
    monopoly/Userifce.cpp
)

set(ARTLIB_SOURCES
    artlib/candataf.cpp
    artlib/L_Blt.cpp
    artlib/L_Blt16.cpp
    artlib/L_Blt24.cpp
    artlib/L_Blt32.cpp
    artlib/L_Button.cpp
    artlib/l_can.cpp
    artlib/l_canf.cpp
    artlib/L_Chunk.cpp
    artlib/L_Data.cpp
    artlib/L_DirIni.cpp
    artlib/L_Error.cpp
    artlib/L_Fonts.cpp
    artlib/L_Grafix.cpp
    artlib/L_Header.cpp
    artlib/L_Init.cpp
    artlib/L_JoyPad.cpp
    artlib/L_Keybrd.cpp
    artlib/L_Matrix.cpp
    artlib/L_Memory.cpp
    artlib/L_Midi.cpp
    artlib/L_MMX.cpp
    artlib/L_Mouse.cpp
    artlib/L_Print.cpp
    artlib/L_Queue.cpp
    artlib/L_Rend0D.cpp
    artlib/L_Rend2D.cpp
    artlib/L_Rend3D.cpp
    artlib/L_Seqncr.cpp
    artlib/L_Sound.cpp
    artlib/L_Speed.cpp
    artlib/L_Sprite.cpp
    artlib/L_Timers.cpp
    artlib/L_UIMsg.cpp
    artlib/L_Video.cpp
)

set(PC3D_SOURCES
    PC3D/camera.cpp
    PC3D/D3DDevice.cpp
    PC3D/d3dutils.cpp
    PC3D/DDDrvMgr.cpp
    PC3D/DDrawDrv.cpp
    PC3D/DDrawWnd.cpp
    PC3D/debug.cpp
    PC3D/FRAME.CPP
    PC3D/GlobalData.cpp
    PC3D/hmdload.cpp
    PC3D/IPOINT.CPP
    PC3D/IRECT.CPP
    PC3D/LIGHT.CPP
    PC3D/MATERIAL.CPP
    PC3D/Matrix.cpp
    PC3D/MemManager.cpp
    PC3D/mesh.cpp
    PC3D/mesh1.cpp
    PC3D/MeshGroup.cpp
    PC3D/NewFrame.cpp
    PC3D/NewMesh.cpp
    PC3D/pc3d.cpp
    PC3D/PC3DHdr.cpp
    PC3D/rndmesh1.cpp
    PC3D/SCENE.CPP
    PC3D/surface.cpp
    PC3D/TAG.CPP
    PC3D/ToMesh.cpp
    PC3D/VIEW.CPP
)

set(ZLIB_SOURCES
    ZLib/Adler32.c
    ZLib/CRC32.c
    ZLib/Deflate.c
    ZLib/InfBlock.c
    ZLib/InfCodes.c
    ZLib/InfFast.c
    ZLib/Inflate.c
    ZLib/InfTrees.c
    ZLib/InfUtil.c
    ZLib/Trees.c
    ZLib/ZImplode.c
    ZLib/ZUtil.c
)

add_executable(monopoly
    ${MONOPOLY_SOURCES}
    ${ARTLIB_SOURCES}
    ${PC3D_SOURCES}
    ${ZLIB_SOURCES}
    web/monopoly_web_entry.cpp
    web/monopoly_web_platform.cpp
    web/monopoly_web_dxini.cpp
    web/monopoly_web_getdxver.cpp
    web/monopoly_web_bltwarp.cpp
    web/monopoly_web_browser.cpp
    web/monopoly_webgl_bridge.cpp
)

target_include_directories(monopoly PRIVATE
    "${MONOPOLY_GENERATED_DIR}"
    ${MONOPOLY_CASEFOLD_INCLUDE_DIRS}
    web/compat
    web
    .
    monopoly
    artlib
    PC3D
    ZLib
)
add_dependencies(monopoly monopoly_generate_bltinc)
add_dependencies(monopoly monopoly_casefold_include_overlay)

target_compile_definitions(monopoly PRIVATE
    MONOPOLY_WEB=1
    CE_ARTLIB_DO_NOT_OVERRIDE_NEW_AND_DELETE=1
    WIN32=1
    _WIN32=1
    _WINDOWS=1
    DIRECTDRAW_VERSION=0x0600
    DIRECTDRAW_VERSION0=0x0600
)

target_compile_features(monopoly PRIVATE cxx_std_14)

target_compile_options(monopoly PRIVATE
    -fms-extensions
    -fdelayed-template-parsing
    -fshort-wchar
    -Wno-address-of-temporary
    -Wno-c++11-narrowing
    -Wno-deprecated-writable-strings
    -Wno-register
    -Wno-invalid-source-encoding
    -Wno-macro-redefined
    -Wno-microsoft-enum-forward-reference
    -Wno-microsoft-extra-qualification
    -Wno-nonportable-include-path
    -Wno-pragma-pack
    -Wno-reorder
    -Wno-return-type
    -Wno-switch
    -Wno-writable-strings
)

target_link_options(monopoly PRIVATE
    "SHELL:-s MIN_WEBGL_VERSION=2"
    "SHELL:-s MAX_WEBGL_VERSION=2"
    "SHELL:-s FULL_ES3=1"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s INITIAL_MEMORY=268435456"
    "SHELL:-s ABORTING_MALLOC=0"
    "SHELL:-s STACK_SIZE=8388608"
    "SHELL:-s ASSERTIONS=2"
    "SHELL:-s STACK_OVERFLOW_CHECK=2"
    "SHELL:-s FORCE_FILESYSTEM=1"
    "SHELL:-s EXIT_RUNTIME=0"
    "SHELL:-s INVOKE_RUN=0"
    "SHELL:-s MODULARIZE=0"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','FS','IDBFS']"
    "SHELL:-s EXPORTED_FUNCTIONS=['_main','_monopoly_web_start','_monopoly_web_stop','_monopoly_web_key_down','_monopoly_web_key_up','_monopoly_web_char','_monopoly_web_mouse_event','_monopoly_web_debug_state']"
    "-lidbfs.js"
)

set_target_properties(monopoly PROPERTIES
    OUTPUT_NAME monopoly
    SUFFIX ".js"
)

add_custom_target(monopoly_web_static_assets
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_WEB_UPLOAD_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_WEB_UPLOAD_DIR}/ai"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_WEB_UPLOAD_DIR}/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/web/monopoly_shell.html"
        "${MONOPOLY_WEB_UPLOAD_DIR}/index.html"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/web/monopoly_service_worker.js"
        "${MONOPOLY_WEB_UPLOAD_DIR}/monopoly_service_worker.js"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/BackGround.bmp"
        "${MONOPOLY_WEB_UPLOAD_DIR}/resources/BackGround.bmp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/BackGroundEuro.bmp"
        "${MONOPOLY_WEB_UPLOAD_DIR}/resources/BackGroundEuro.bmp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MONOPOLY_AI_STRATEGY_FILES}
        "${MONOPOLY_WEB_UPLOAD_DIR}/ai"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/web/monopoly_shell.html"
        "${CMAKE_CURRENT_SOURCE_DIR}/web/monopoly_service_worker.js"
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/BackGround.bmp"
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/BackGroundEuro.bmp"
        ${MONOPOLY_AI_STRATEGY_FILES}
)
add_dependencies(monopoly monopoly_web_static_assets)

add_custom_command(TARGET monopoly POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_WEB_UPLOAD_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_WEB_UPLOAD_DIR}/ai"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONOPOLY_WEB_UPLOAD_DIR}/resources"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/web/monopoly_shell.html"
        "${MONOPOLY_WEB_UPLOAD_DIR}/index.html"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/web/monopoly_service_worker.js"
        "${MONOPOLY_WEB_UPLOAD_DIR}/monopoly_service_worker.js"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/BackGround.bmp"
        "${MONOPOLY_WEB_UPLOAD_DIR}/resources/BackGround.bmp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/monopoly/BackGroundEuro.bmp"
        "${MONOPOLY_WEB_UPLOAD_DIR}/resources/BackGroundEuro.bmp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MONOPOLY_AI_STRATEGY_FILES}
        "${MONOPOLY_WEB_UPLOAD_DIR}/ai"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:monopoly>"
        "${MONOPOLY_WEB_UPLOAD_DIR}/monopoly.js"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:monopoly>/monopoly.wasm"
        "${MONOPOLY_WEB_UPLOAD_DIR}/monopoly.wasm"
)
