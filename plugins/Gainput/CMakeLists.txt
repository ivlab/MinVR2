# Template for creating a CMakeLists.txt for new plugins -- copy this, replace XXX and YYY, 
# and follow the example of how OpenGL/GLEW are handled below if you need your plugin to 
# link with an external library.
# -----------------------------------------------------------------------------------------

# This file is part of the MinVR cmake build system.  
# See the main MinVR/CMakeLists.txt file for authors, copyright, and license info.
#
# Builds the MinVR_XXX plugin as a shared library.

# Note: This CMakeLists.txt is not intended to work on its own.  It inherits many settings 
# from the root CMakeLists.txt (build and install directories, compiling flags, #defines, etc.)


# Plugin names must begin with "MinVR_"
project(MinVR_Gainput)


# Source:
set (SOURCEFILES 
  src/VRGainputPlugin.cpp
  src/VRGainputDevice.cpp
  src/VRGainputDeviceButtonListener.cpp
)
set (HEADERFILES
  src/VRGainputDevice.h
  src/VRGainputDeviceButtonListener.h
)


# Define the target
add_library(${PROJECT_NAME} SHARED ${HEADERFILES} ${SOURCEFILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>                  # for headers when building
    $<INSTALL_INTERFACE:${INSTALL_PLUGININCLUDE_DEST}/${PROJECT_NAME}>  # for client in install mode
)


# Setup dependency on libMinVR:
target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../src>  # for headers when building
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DEST}>              # for client in install mode
)
target_link_libraries(${PROJECT_NAME} PUBLIC MinVR)


# Setup dependency:
include(AutoBuildGainput)
AutoBuild_use_package_Gainput(${PROJECT_NAME} PUBLIC)


add_definitions(-DUSE_MIVR_GAINPUT)

# Installation:
install(TARGETS ${PROJECT_NAME} EXPORT MinVRTargets COMPONENT ${PROJECT_NAME}
    LIBRARY DESTINATION ${INSTALL_PLUGINLIB_DEST}
    ARCHIVE DESTINATION ${INSTALL_PLUGINLIB_DEST}
    RUNTIME DESTINATION ${INSTALL_PLUGINDLL_DEST}
)

message(status "Gainput install")
message(status "INSTALL_PLUGININCLUDE_DEST ${INSTALL_PLUGININCLUDE_DEST}/${PROJECT_NAME}")
install(FILES ${HEADERFILES} DESTINATION ${INSTALL_PLUGININCLUDE_DEST}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})


# For better organization when using an IDE with folder structures:
source_group("Header Files" FILES ${HEADERFILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "plugins")
