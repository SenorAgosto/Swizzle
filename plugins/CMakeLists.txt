# suppress warnings about not setting rpath
if(APPLE)
  if(UNIX)
    cmake_policy(SET CMP0042 OLD)
  endif()
endif() 

include_directories(".")

### print plugin
file(GLOB print_src ${CMAKE_CURRENT_SOURCE_DIR}/print/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/print/*.hpp)
add_library(plugin_print SHARED ${print_src})
target_link_libraries(plugin_print swizzle ${Boost_LIBRARIES})

# target_link_libraries(load_plugins PluginFactory ${Boost_LIBRARIES} ${DL_LIBRARIES})

if(POSIX)
    set_property(TARGET plugin_print PROPERTY COMPILE_FLAGS --export-dynamic)
endif()

