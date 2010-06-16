
set (UF_BINARY_DIR ${PROJECT_BINARY_DIR})
#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The install-only section is empty for the build tree.
set(UF_CONFIG_INSTALL_ONLY)

# The "use" file.
set(UF_USE_FILE ${UF_BINARY_DIR}/UseUF.cmake)

# The build settings file.
set(UF_BUILD_SETTINGS_FILE ${UF_BINARY_DIR}/ufBuildSettings.cmake)


#-----------------------------------------------------------------------------
# Configure ufConfig.cmake for the build tree.
configure_file(${UF_SOURCE_DIR}/ufConfig.cmake.in
               ${UF_BINARY_DIR}/ufConfig.cmake @ONLY IMMEDIATE)

