cmake_minimum_required(VERSION 3.16)

project(Blueberry_Wolfz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

set(PROJECT_SOURCES
        src/main.cpp
        src/ui/Main.qml
        src/ui/resources.qrc
        src/control/logic/PlayerController.cpp
        src/control/logic/PlayerController.h
        src/control/service/MetadataManager.cpp
        src/control/service/MetadataManager.h
        src/data/player/MusicPlayer.cpp
        src/data/player/MusicPlayer.h
        src/data/playlist/Queue.cpp
        src/data/playlist/Queue.h
        src/data/playlist/History.cpp
        src/data/playlist/History.h
        src/data/model/Song.h
        src/data/model/Song.cpp
        src/data/model/LoopMode.h
)

if(WIN32)
    list(APPEND PROJECT_SOURCES src/ui/resources.rc)
endif()

qt_add_executable(Blueberry_Wolfz
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(Blueberry_Wolfz PRIVATE src)

target_link_libraries(Blueberry_Wolfz PRIVATE
    Qt6::Quick
    Qt6::Multimedia
)

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