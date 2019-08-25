# Try to find UsingIntrusivePtrIn
# Once done this will define
# UsingIntrusivePtrIn_FOUND - system has the lib 
# UsingIntrusivePtrIn_INCLUDE_DIRS - the path to include directory 
# UsingIntrusivePtrIn_LIBRARIES - the libraries needed to use
# UsingIntrusivePtrIn_DEFINITIONS - compiler switches required for using UsingIntrusivePtrIn

find_package(PkgConfig)
pkg_check_modules(PC_UsingIntrusivePtrIn QUIET libUsingIntrusivePtrIn)
set(UsingIntrusivePtrIn_DEFINITIONS ${PC_UsingIntrusivePtrIn_CFLAGS_OTHER})

# see if there is a brew path (useful when it is not /usr/local/)
set(PC_HOMEBREW_PATH $ENV{HOMEBREW})
if(PC_HOMEBREW_PATH)
    set(PC_HOMEBREW_INCLUDE_DIRS "$ENV{HOMEBREW}/include")
    set(PC_HOMEBREW_LIBRARY_DIRS "$ENV{HOMEBREW}/lib")
endif()

find_path(UsingIntrusivePtrIn_INCLUDE_DIR UsingIntrusivePtrIn/UsingIntrusivePtrIn.hpp
    HINTS ${PC_UsingIntrusivePtrIn_INCLUDEDIR} ${PC_UsingIntrusivePtrIn_INCLUDE_DIRS} ${PC_HOMEBREW_INCLUDE_DIRS}
    PATH_SUFFIXES UsingIntrusivePtrIn )

find_library(UsingIntrusivePtrIn_LIBRARY NAMES UsingIntrusivePtrIn libUsingIntrusivePtrIn
    HINTS ${PC_UsingIntrusivePtrIn_LIBDIR} ${PC_UsingIntrusivePtrIn_LIBRARY_DIRS} ${PC_HOMEBREW_LIBRARY_DIRS} )

set(UsingIntrusivePtrIn_LIBRARIES ${UsingIntrusivePtrIn_LIBRARY} )
set(UsingIntrusivePtrIn_INCLUDE_DIRS ${UsingIntrusivePtrIn_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs) 
find_package_handle_standard_args(UsingIntrusivePtrIn DEFAULT_MSG
    UsingIntrusivePtrIn_LIBRARY UsingIntrusivePtrIn_INCLUDE_DIR)

mark_as_advanced(UsingIntrusivePtrIn_INCLUDE_DIR UsingIntrusivePtrIn_LIBRARY)
 
