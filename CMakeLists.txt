cmake_minimum_required (VERSION 2.8)
project (image_picker_tool)

include ( FindPkgConfig  )

find_package ( Qt4 REQUIRED )
find_package ( OpenCV REQUIRED )

include( ${QT_USE_FILE} )
add_definitions(${QT_DEFINITIONS})

set(
    ${PROJECT_NAME}_HEADERS_MOC
    src/ImagePickerTool.hpp
    src/ImageDrawer.hpp
)

qt4_wrap_cpp( image_picker_tool_MOC_CPP ${image_picker_tool_HEADERS_MOC} )


file ( GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )
file ( GLOB HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp )

include_directories (
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_INSTALL_PREFIX}/include/sonar_toolkit
)

add_library (
    image_picker_tool SHARED
    ${SOURCES}
    ${image_picker_tool_MOC_CPP}
)

target_link_libraries (
    image_picker_tool
    ${QT_LIBRARIES}
    ${OpenCV_LIBS}
)

add_executable (
    image_picker
    example/image_picker.cpp
    ${SOURCES}
    ${image_picker_tool_MOC_CPP}
)

target_link_libraries (
    image_picker
    image_picker_tool
)

install(
    FILES ${HEADERS}
    DESTINATION include/sonar_toolkit/${PROJECT_NAME}
)

install(
    TARGETS image_picker_tool
    DESTINATION lib/sonar_toolkit
)

install(
    TARGETS image_picker
    DESTINATION bin
)
