cmake_minimum_required(VERSION 3.16)

project(AtcMonitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    flight.h
    flightsmodel.h
    flightsmodel.cpp

    radarview.h
    radarview.cpp

    datagenerator.h
    datagenerator.cpp

    conflictdetector.h
    conflictdetector.cpp

    flightsproxymodel.h
    flightsproxymodel.cpp

    appconfig.h
    appconfig.cpp

    settingsdialog.h
    settingsdialog.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AtcMonitor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(AtcMonitor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(AtcMonitor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(AtcMonitor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AtcMonitor
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AtcMonitor)
endif()

set_target_properties(AtcMonitor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AtcMonitor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

enable_testing()
add_subdirectory(tests)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AtcMonitor)
endif()
