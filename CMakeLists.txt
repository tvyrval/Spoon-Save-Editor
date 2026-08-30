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
    src/core/SaveManager.h
    src/core/SaveManager.cpp
    src/core/ItemData.h
    src/core/ItemData.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
    src/ui/utils/EditorTabUtils.h
    src/ui/utils/EditorTabUtils.cpp
    src/ui/tabs/EditorTabs.h
    src/ui/tabs/WeaponTab.cpp
    src/ui/tabs/GearTab.cpp
    src/ui/tabs/PlayerTab.cpp
    src/ui/tabs/HeroTab.cpp
    src/ui/tabs/PlazaTab.cpp
    src/ui/tabs/AmiiboTab.cpp
    src/ui/tabs/MinigameTab.cpp
    src/ui/tabs/SpykeTab.cpp
    src/ui/tabs/BitFlagTab.cpp
)

qt_add_resources(spoon_save_editor appicons
    PREFIX "/"
    FILES images/icon.png
)

target_include_directories(spoon_save_editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/tabs
)

target_link_libraries(spoon_save_editor PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_compile_definitions(spoon_save_editor PRIVATE APP_VERSION=\"${PROJECT_VERSION}\")

if(WIN32)
    target_sources(spoon_save_editor PRIVATE resources.rc)
    set_target_properties(spoon_save_editor PROPERTIES OUTPUT_NAME "Spoon Save Editor")
    target_link_options(spoon_save_editor PRIVATE "$<$<CONFIG:Release>:-mwindows>")
endif()

if(APPLE)
    set(MACOSX_ICON "images/icon.icns")
    target_sources(spoon_save_editor PRIVATE ${MACOSX_ICON})
    set_source_files_properties(${MACOSX_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    
    set_target_properties(spoon_save_editor PROPERTIES
        MACOSX_BUNDLE TRUE
        OUTPUT_NAME "Spoon Save Editor"
        MACOSX_BUNDLE_BUNDLE_NAME "Spoon Save Editor"
        MACOSX_BUNDLE_ICON_FILE "icon.icns"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.github.tvyrval.spoonsaveeditor"
    )
endif()

install(TARGETS spoon_save_editor
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
