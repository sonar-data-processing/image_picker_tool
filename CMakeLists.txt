cmake_minimum_required (VERSION 2.8)
project (image_picker_tool)

include ( "${WORKSPACE_DIR}/base/cmake/Dependencies.cmake"  )

add_project_dependency (base ${WORKSPACE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

setup_headers ( ${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src" )

include ( FindPkgConfig  )
find_package ( Qt4 REQUIRED )
find_package ( OpenCV REQUIRED )

include( ${QT_USE_FILE} )
add_definitions(${QT_DEFINITIONS})

set(
    image_picker_tool_HEADERS_MOC
    src/ImagePickerTool.hpp
    src/ImageViewer.hpp
)

qt4_wrap_cpp( image_picker_tool_MOC_CPP ${image_picker_tool_HEADERS_MOC} )

set (
    ${PROJECT_NAME}_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/include
    CACHE PATH "Image Picker Tool include directory"
    FORCE
)

file ( GLOB SRCS RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/*.cpp )

set ( SRCS ${SRCS} ${sonar_target_tracking_SOURCES} )

include_directories (
    ${image_picker_tool_INCLUDE_DIR}
)

add_library (
    image_picker_tool STATIC
    ${SRCS}
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
    ${SRCS}
    ${image_picker_tool_MOC_CPP}
)

target_link_libraries (
    image_picker
    image_picker_tool
)
