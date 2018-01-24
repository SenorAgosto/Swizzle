# we use std::thread, we need to link pthread.
set(platform_dependencies pthread)
add_definitions(-DPOSIX=1)
