# Try to find PluginFactory 
# Once done this will define
# PluginFactory_FOUND - system has the lib 
# PluginFactory_INCLUDE_DIRS - the path to include directory 
# PluginFactory_LIBRARIES - the libraries needed to use
# PluginFactory_DEFINITIONS - compiler switches required for using PluginFactory 

find_package(PkgConfig)
pkg_check_modules(PC_PluginFactory QUIET libPluginFactory)
set(PluginFactory_DEFINITIONS ${PC_PluginFactory_CFLAGS_OTHER})

# see if there is a brew path (useful when it is not /usr/local/)
set(PC_HOMEBREW_PATH $ENV{HOMEBREW})
if(PC_HOMEBREW_PATH)
    set(PC_HOMEBREW_INCLUDE_DIRS "$ENV{HOMEBREW}/include")
    set(PC_HOMEBREW_LIBRARY_DIRS "$ENV{HOMEBREW}/lib")
endif()

find_path(PluginFactory_INCLUDE_DIR PluginFactory/PluginFactory.hpp
    HINTS ${PC_PluginFactory_INCLUDEDIR} ${PC_PluginFactory_INCLUDE_DIRS} ${PC_HOMEBREW_INCLUDE_DIRS}
    PATH_SUFFIXES PluginFactory )

find_library(PluginFactory_LIBRARY NAMES PluginFactory libPluginFactory
    HINTS ${PC_PluginFactory_LIBDIR} ${PC_PluginFactory_LIBRARY_DIRS} ${PC_HOMEBREW_LIBRARY_DIRS} )

set(PluginFactory_LIBRARIES ${PluginFactory_LIBRARY} )
set(PluginFactory_INCLUDE_DIRS ${PluginFactory_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs) 
find_package_handle_standard_args(PluginFactory DEFAULT_MSG
    PluginFactory_LIBRARY PluginFactory_INCLUDE_DIR)

mark_as_advanced(PluginFactory_INCLUDE_DIR PluginFactory_LIBRARY)
 
