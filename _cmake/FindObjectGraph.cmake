# Try to find ObjectGraph
# Once done this will define
# ObjectGraph_FOUND - system has the lib 
# ObjectGraph_INCLUDE_DIRS - the path to include directory 
# ObjectGraph_LIBRARIES - the libraries needed to use
# ObjectGraph_DEFINITIONS - compiler switches required for using ObjectGraph

find_package(PkgConfig)
pkg_check_modules(PC_ObjectGraph QUIET libObjectGraph)
set(ObjectGraph_DEFINITIONS ${PC_ObjectGraph_CFLAGS_OTHER})

# see if there is a brew path (useful when it is not /usr/local/)
set(PC_HOMEBREW_PATH $ENV{HOMEBREW})
if(PC_HOMEBREW_PATH)
    set(PC_HOMEBREW_INCLUDE_DIRS "$ENV{HOMEBREW}/include")
    set(PC_HOMEBREW_LIBRARY_DIRS "$ENV{HOMEBREW}/lib")
endif()

find_path(ObjectGraph_INCLUDE_DIR ObjectGraph/DirectedGraph.hpp
    HINTS ${PC_ObjectGraph_INCLUDEDIR} ${PC_ObjectGraph_INCLUDE_DIRS} ${PC_HOMEBREW_INCLUDE_DIRS}
    PATH_SUFFIXES ObjectGraph )

find_library(ObjectGraph_LIBRARY NAMES ObjectGraph libObjectGraph
    HINTS ${PC_ObjectGraph_LIBDIR} ${PC_ObjectGraph_LIBRARY_DIRS} ${PC_HOMEBREW_LIBRARY_DIRS} )

set(ObjectGraph_LIBRARIES ${ObjectGraph_LIBRARY} )
set(ObjectGraph_INCLUDE_DIRS ${ObjectGraph_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs) 
find_package_handle_standard_args(ObjectGraph DEFAULT_MSG
    ObjectGraph_LIBRARY ObjectGraph_INCLUDE_DIR)

mark_as_advanced(ObjectGraph_INCLUDE_DIR ObjectGraph_LIBRARY)
 
