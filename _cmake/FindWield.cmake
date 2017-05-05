# Try to find Wield
# Once done this will define
# Wield_FOUND - system has the lib 
# Wield_INCLUDE_DIRS - the path to include directory 
# Wield_LIBRARIES - the libraries needed to use
# Wield_DEFINITIONS - compiler switches required for using Wield

find_package(PkgConfig)
pkg_check_modules(PC_Wield QUIET libWield)
set(Wield_DEFINITIONS ${PC_Wield_CFLAGS_OTHER})

# see if there is a brew path (useful when it is not /usr/local/)
set(PC_HOMEBREW_PATH $ENV{HOMEBREW})
if(PC_HOMEBREW_PATH)
    set(PC_HOMEBREW_INCLUDE_DIRS "$ENV{HOMEBREW}/include")
    set(PC_HOMEBREW_LIBRARY_DIRS "$ENV{HOMEBREW}/lib")
endif()

find_path(Wield_INCLUDE_DIR Wield/SchedulerBase.hpp
    HINTS ${PC_Wield_INCLUDEDIR} ${PC_Wield_INCLUDE_DIRS} ${PC_HOMEBREW_INCLUDE_DIRS}
    PATH_SUFFIXES Wield )

find_library(Wield_LIBRARY NAMES Wield libWield
    HINTS ${PC_Wield_LIBDIR} ${PC_Wield_LIBRARY_DIRS} ${PC_HOMEBREW_LIBRARY_DIRS} )

set(Wield_LIBRARIES ${Wield_LIBRARY} )
set(Wield_INCLUDE_DIRS ${Wield_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs) 
find_package_handle_standard_args(Wield DEFAULT_MSG
    Wield_LIBRARY Wield_INCLUDE_DIR)

mark_as_advanced(Wield_INCLUDE_DIR Wield_LIBRARY)
 
