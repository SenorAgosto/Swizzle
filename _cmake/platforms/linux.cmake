message("Configuring Linux")
add_definitions(-DPOSIX=1 -DLINUX=1)

# determine Linux distribution
find_program(LSB_RELEASE lsb_release)
execute_process(COMMAND ${LSB_RELEASE} -is OUTPUT_VARIABLE LINUX_DISTRO OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${LSB_RELEASE} -rs OUTPUT_VARIABLE LINUX_DISTRO_VER OUTPUT_STRIP_TRAILING_WHITESPACE)

message("Linux Distro: ${LINUX_DISTRO}")
message("Distro Ver:   ${LINUX_DISTRO_VER}")

if(${LINUX_DISTRO} MATCHES "Ubuntu")
    add_definitions(-DUBUNTU=${LINUX_DISTRO_VER})
endif()

if(${LINUX_DISTRO} MATCHES "Debian")
    add_definitions(-DDEBIAN=${LINUX_DISTRO_VER})
endif()