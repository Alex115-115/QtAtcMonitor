set(TEST_SOURCES
    test_conflict_detector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../conflictdetector.cpp
)

add_executable(ConflictDetectorTests ${TEST_SOURCES})

target_link_libraries(ConflictDetectorTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
)

target_include_directories(ConflictDetectorTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_test(NAME ConflictDetectorTests COMMAND ConflictDetectorTests)
