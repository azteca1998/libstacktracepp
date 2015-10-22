set(PROJECT_HEADERS
    "../include/stacktracepp/stack_trace.hpp"
    "../include/stacktracepp/stack_frame.hpp"
)

set(PROJECT_SOURCES
    "stack_trace.cpp"
    "stack_frame.cpp"
)

set(PROJECT_LIBRARIES
    "-ldw"
    "-lunwind"
)

include_directories("${CMAKE_SOURCE_DIR}/include")
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} )
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION "lib")
install(FILES ${PROJECT_HEADERS} DESTINATION "include")


set(CPACK_PACKAGE_NAME "libstacktracepp")
set(CPACK_PACKAGE_VENDOR "Esteve Soler Arderiu")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libstacktracepp - High level stack unwinding library for C++")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "libstacktracepp")
include(CPack)

