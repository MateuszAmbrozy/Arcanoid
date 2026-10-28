cmake_minimum_required(VERSION 3.20)
project(Arcanoid VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
find_package(box2d 2.4 CONFIG REQUIRED)

set(ARCANOID_SOURCES
    Arcanoid/Ball.cpp Arcanoid/Block.cpp Arcanoid/BlockSet.cpp
    Arcanoid/ContactListener.cpp Arcanoid/EditorState.cpp Arcanoid/EndGame.cpp
    Arcanoid/Game.cpp Arcanoid/GameState.cpp Arcanoid/GraphicsSettings.cpp
    Arcanoid/Gui.cpp Arcanoid/LevelState.cpp Arcanoid/MainMenuState.cpp
    Arcanoid/ObjectData.cpp Arcanoid/Paddle.cpp Arcanoid/PausedMenu.cpp
    Arcanoid/Powers.cpp Arcanoid/SettingsState.cpp Arcanoid/State.cpp Arcanoid/Timer.cpp)

add_library(arcanoid_core STATIC ${ARCANOID_SOURCES})
target_include_directories(arcanoid_core PUBLIC Arcanoid)
target_link_libraries(arcanoid_core PUBLIC sfml-graphics sfml-window sfml-system sfml-audio box2d::box2d)
if(MSVC)
    target_compile_options(arcanoid_core PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(arcanoid_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(ARCANOID_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
if(ARCANOID_SANITIZERS AND NOT MSVC)
    target_compile_options(arcanoid_core PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(arcanoid_core PUBLIC -fsanitize=address,undefined)
endif()

add_executable(arcanoid Arcanoid/main.cpp)
target_link_libraries(arcanoid PRIVATE arcanoid_core)
set_target_properties(arcanoid PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:arcanoid>")
add_custom_command(TARGET arcanoid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Arcanoid/Resources" "$<TARGET_FILE_DIR:arcanoid>/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Arcanoid/Fonts" "$<TARGET_FILE_DIR:arcanoid>/Fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Arcanoid/Config" "$<TARGET_FILE_DIR:arcanoid>/Config")
file(GLOB ARCANOID_MAPS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Arcanoid/*.slmp")
foreach(map IN LISTS ARCANOID_MAPS)
    add_custom_command(TARGET arcanoid POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${map}" "$<TARGET_FILE_DIR:arcanoid>")
endforeach()
if(WIN32)
    add_custom_command(TARGET arcanoid POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sfml-graphics> $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system> $<TARGET_FILE:sfml-audio> "$<TARGET_FILE_DIR:arcanoid>")
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(arcanoid_tests tests/RegressionTests.cpp)
    target_link_libraries(arcanoid_tests PRIVATE arcanoid_core)
    add_test(NAME arcanoid_regressions COMMAND arcanoid_tests)
    set_tests_properties(arcanoid_regressions PROPERTIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Arcanoid"
        ENVIRONMENT "ALSOFT_DRIVERS=null")
endif()
