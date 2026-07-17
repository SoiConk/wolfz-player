cmake_minimum_required(VERSION 3.25)

project(Blueberry_Wolfz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    QuickControls2
    Multimedia
    Sql
    Concurrent
)

qt_standard_project_setup(REQUIRES 6.11.1)

find_package(TagLib REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp

        src/control/logic/PlayerController.h
        src/control/logic/PlayerController.cpp
        src/control/logic/MusicLoader.h
        src/control/logic/MusicLoader.cpp

        src/control/service/MetadataManager.cpp
        src/control/service/MetadataManager.h
        src/control/service/ShowInfo.h
        src/control/service/ShowInfo.cpp
        src/control/service/DatabaseManager.h
        src/control/service/DatabaseManager.cpp
        src/control/service/ExtractMetadata.h
        src/control/service/ExtractMetadata.cpp

        src/data/player/MusicPlayer.cpp
        src/data/player/MusicPlayer.h

        src/data/playlist/Queue.cpp
        src/data/playlist/Queue.h
        src/data/playlist/History.cpp
        src/data/playlist/History.h
        src/data/playlist/Playlist.h
        src/data/playlist/Playlist.cpp

        src/data/model/Song.h
        src/data/model/Song.cpp
)

if(WIN32)
    list(APPEND PROJECT_SOURCES src/ui/resources.rc)
endif()

qt_add_executable(Blueberry_Wolfz
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(Blueberry_Wolfz PUBLIC
    src
    src/control/logic
    src/control/service
)

file(GLOB_RECURSE ALL_QML_FILES CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/ui/*.qml"
)

file(GLOB IMAGE_RESOURCES CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/ui/assets/images/*.png"
    "src/ui/assets/icons/*.svg"
)

qt_add_qml_module(Blueberry_Wolfz
    URI Blueberry_Wolfz
    VERSION 1.0

    QML_FILES
        ${ALL_QML_FILES}

    RESOURCES
        ${IMAGE_RESOURCES}
)

target_link_libraries(Blueberry_Wolfz PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia
    Qt6::Sql
    Qt6::Concurrent
    TagLib::TagLib
)

target_compile_definitions(Blueberry_Wolfz PRIVATE QT_NO_DEBUG_OUTPUT)

if(WIN32)
    set_target_properties(Blueberry_Wolfz PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

if(APPLE)
    set_target_properties(Blueberry_Wolfz PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

qt_finalize_executable(Blueberry_Wolfz)

