cmake_minimum_required(VERSION 3.21)

project(SpoonSaveEditor
    VERSION 1.0.0
    DESCRIPTION "Splatoon Save File Editor"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(spoon_save_editor
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/SaveManager.h
    src/SaveManager.cpp
    src/ItemData.h
    src/ItemData.cpp
    src/EditorTabUtils.h
    src/EditorTabUtils.cpp
    src/EditorTabs.h
    src/WeaponTab.cpp
    src/GearTab.cpp
    src/PlayerTab.cpp
    src/HeroTab.cpp
    src/PlazaTab.cpp
    src/AmiiboTab.cpp
    src/MinigameTab.cpp
    src/SpykeTab.cpp
    src/BitFlagTab.cpp
)

qt_add_resources(spoon_save_editor appicons
    PREFIX "/"
    FILES images/icon.png
)

target_include_directories(spoon_save_editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(spoon_save_editor PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_compile_definitions(spoon_save_editor PRIVATE APP_VERSION=\"${PROJECT_VERSION}\")

install(TARGETS spoon_save_editor RUNTIME DESTINATION bin)
