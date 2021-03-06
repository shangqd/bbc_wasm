add_executable( ${NODE_EXECUTABLE_NAME} main.cpp )

if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling ${NODE_EXECUTABLE_NAME} with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../.git)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../.."
      OUTPUT_VARIABLE "nodeos_BUILD_VERSION"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Git commit revision: ${nodeos_BUILD_VERSION}")
  else()
    set(nodeos_BUILD_VERSION 0)
  endif()
else()
  set(nodeos_BUILD_VERSION 0)
endif()

configure_file(config.hpp.in config.hpp ESCAPE_QUOTES)

target_include_directories(${NODE_EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if(UNIX)
  if(APPLE)
    set(whole_archive_flag "-force_load")
    set(no_whole_archive_flag "")
    set(build_id_flag "")
  else()
    set(whole_archive_flag "--whole-archive")
    set(no_whole_archive_flag "--no-whole-archive")
    set(build_id_flag "--build-id")
  endif()
else()
  set(whole_archive_flag "--whole-archive")
  set(no_whole_archive_flag "--no-whole-archive")
  set(build_id_flag "")
endif()

target_link_libraries( ${NODE_EXECUTABLE_NAME}
        PRIVATE appbase
        PRIVATE -Wl,${build_id_flag}
        PRIVATE eosio_chain 
        ${CMAKE_DL_LIBS} 
        ${PLATFORM_SPECIFIC_LIBS} )

if(BUILD_MONGO_DB_PLUGIN)
  target_link_libraries( ${NODE_EXECUTABLE_NAME} PRIVATE -Wl,${whole_archive_flag} mongo_db_plugin -Wl,${no_whole_archive_flag} )
endif()

include(additionalPlugins)

copy_bin( ${NODE_EXECUTABLE_NAME} )
install( TARGETS
   ${NODE_EXECUTABLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR} COMPONENT base
)
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/log/eosio
        DIRECTORY_PERMISSIONS OWNER_READ
                              OWNER_WRITE
                              OWNER_EXECUTE
                              GROUP_READ
                              GROUP_WRITE
                              GROUP_EXECUTE
                              WORLD_READ
                              WORLD_EXECUTE
)
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/lib/eosio
        DIRECTORY_PERMISSIONS OWNER_READ
                              OWNER_WRITE
                              OWNER_EXECUTE
                              GROUP_READ
                              GROUP_WRITE
                              GROUP_EXECUTE
                              WORLD_READ
                              WORLD_EXECUTE
			)
