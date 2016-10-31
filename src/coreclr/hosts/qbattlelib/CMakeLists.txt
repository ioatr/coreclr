project(qbattlelib)


find_package(PythonLibs REQUIRED)

include_directories(../qbattlecommon ${PYTHON_INCLUDE_DIRS})
add_compile_options(-fPIC)

set(QBATTLELIB_SOURCES 
    qbattlelib.cpp 
)

#_add_executable(qbattle
#    ${QBATTLE_SOURCES}
#)

add_library(qbattlelib SHARED ${QBATTLELIB_SOURCES})

# FreeBSD and NetBSD implement dlopen(3) in libc
if(NOT CMAKE_SYSTEM_NAME STREQUAL FreeBSD AND NOT CMAKE_SYSTEM_NAME STREQUAL NetBSD)
    target_link_libraries(qbattlelib
        dl
    )
endif(NOT CMAKE_SYSTEM_NAME STREQUAL FreeBSD AND NOT CMAKE_SYSTEM_NAME STREQUAL NetBSD)

# Libc turns locks into no-ops if pthread was not loaded into process yet. Loading
# pthread by the process executable ensures that all locks are initialized properly.
target_link_libraries(qbattlelib
    qbattlecommon
    pthread
    ${PYTHON_LIBRARY}
)

add_dependencies(qbattlelib
    coreclr
)

install(FILES __init__.py qbattle.py DESTINATION .)

set_target_properties(qbattlelib PROPERTIES PREFIX "" SUFFIX ".so")
install_clr(qbattlelib)
