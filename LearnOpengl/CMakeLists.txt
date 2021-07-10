  
# cmake_minimum_required( VERSION 3.1 )

# project( LearnOpenGL )
# # project( GLFW-CMake-starter )


# find_package( OpenGL REQUIRED )

# include_directories( ${OPENGL_INCLUDE_DIRS} )

# set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
# set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

# add_subdirectory( dependencies/glfw-3.3.4 )

# if( MSVC )
#     SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
# endif()

# set( GLFW-CMAKE-STARTER-SRC
#      main.cpp
#      )
     
# add_executable( LearnOpenGL WIN32 ${GLFW-CMAKE-STARTER-SRC} )
# target_link_libraries( LearnOpenGL ${OPENGL_LIBRARIES} glfw )
# if( MSVC )
#     if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
#         message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
#     else()
#         set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter )
#     endif()
# endif()
cmake_minimum_required( VERSION 3.1 )
project( LearnOpenGL )
find_package(glfw3 3.3 REQUIRED)
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( GLFW-CMAKE-STARTER-SRC
     main.cpp
     )
add_executable( LearnOpenGL WIN32 ${GLFW-CMAKE-STARTER-SRC} )
target_link_libraries( LearnOpenGL ${OPENGL_LIBRARIES} glfw )
if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter )
    endif()
endif()