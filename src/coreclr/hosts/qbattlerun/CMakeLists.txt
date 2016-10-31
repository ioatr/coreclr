project(qbattlerun)


find_package(PythonLibs REQUIRED)

include_directories(../qbattlecommon ${PYTHON_INCLUDE_DIRS})
add_compile_options(-fPIE)

set(QBATTLERUN_SOURCES 
    qbattlerun.cpp 
)

_add_executable(qbattlerun
    ${QBATTLERUN_SOURCES}
)

# FreeBSD and NetBSD implement dlopen(3) in libc
if(NOT CMAKE_SYSTEM_NAME STREQUAL FreeBSD AND NOT CMAKE_SYSTEM_NAME STREQUAL NetBSD)
    target_link_libraries(qbattlerun
        dl
    )
endif(NOT CMAKE_SYSTEM_NAME STREQUAL FreeBSD AND NOT CMAKE_SYSTEM_NAME STREQUAL NetBSD)

# Libc turns locks into no-ops if pthread was not loaded into process yet. Loading
# pthread by the process executable ensures that all locks are initialized properly.
target_link_libraries(qbattlerun
    qbattlecommon
    pthread
)

add_dependencies(qbattlerun
    coreclr
)

install_clr(qbattlerun)
