# set source files
set (server-src-files
     Provider.cpp
     Backend.cpp)

set (client-src-files
     Client.cpp
     ResourceHandle.cpp
     AsyncRequest.cpp)

set (admin-src-files
     Admin.cpp)

set (dummy-src-files
     dummy/DummyBackend.cpp)

set (module-src-files
     BedrockModule.cpp)

# load package helper for generating cmake CONFIG packages
include (CMakePackageConfigHelpers)

# where to install files for "find_package"
set (YD-pkg "share/cmake/YD")

# set version fo shared library
set (YD-vers "${YD_VERSION_MAJOR}.${YD_VERSION_MINOR}")

# server library
add_library (YD-server ${server-src-files} ${dummy-src-files})
target_link_libraries (YD-server
    PUBLIC thallium PkgConfig::uuid nlohmann_json::nlohmann_json
    PRIVATE spdlog::spdlog coverage_config)
target_include_directories (YD-server PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories (YD-server BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
target_include_directories (YD-server BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties (YD-server
    PROPERTIES VERSION ${YD_VERSION}
    SOVERSION ${YD_VERSION_MAJOR})

# client library
add_library (YD-client ${client-src-files})
target_link_libraries (YD-client
    PUBLIC thallium PkgConfig::uuid nlohmann_json::nlohmann_json
    PRIVATE spdlog::spdlog coverage_config)
target_include_directories (YD-client PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories (YD-client BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
target_include_directories (YD-client BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties (YD-client
    PROPERTIES VERSION ${YD_VERSION}
    SOVERSION ${YD_VERSION_MAJOR})

# admin library
add_library (YD-admin ${admin-src-files})
target_link_libraries (YD-admin PUBLIC thallium PkgConfig::uuid nlohmann_json::nlohmann_json
    PRIVATE spdlog::spdlog coverage_config)
target_include_directories (YD-admin PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories (YD-admin BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
target_include_directories (YD-admin BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties (YD-admin
    PROPERTIES VERSION ${YD_VERSION}
    SOVERSION ${YD_VERSION_MAJOR})

if (${ENABLE_BEDROCK})
# bedrock module library
add_library (YD-bedrock-module ${module-src-files})
target_link_libraries (YD-bedrock-module YD-server YD-admin YD-client bedrock-server coverage_config)
target_include_directories (YD-bedrock-module PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories (YD-bedrock-module BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
target_include_directories (YD-bedrock-module BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties (YD-bedrock-module
    PROPERTIES VERSION ${YD_VERSION}
    SOVERSION ${YD_VERSION_MAJOR})
endif ()

# installation stuff (packaging and install commands)
write_basic_package_version_file (
    "YD-config-version.cmake"
    VERSION ${YD_VERSION}
    COMPATIBILITY AnyNewerVersion)

# generate our config file for find_package()
set (INCLUDE_INSTALL_DIR include/)
set (LIB_INSTALL_DIR lib/)
configure_package_config_file (YD-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/YD-config.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/YD/cmake
  PATH_VARS INCLUDE_INSTALL_DIR)

# some bits for the pkg-config file
set (DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set (SERVER_PRIVATE_LIBS "-lYD-server")
set (CLIENT_PRIVATE_LIBS "-lYD-client")
set (ADMIN_PRIVATE_LIBS  "-lYD-admin")
configure_file ("YD-server.pc.in" "YD-server.pc" @ONLY)
configure_file ("YD-client.pc.in" "YD-client.pc" @ONLY)
configure_file ("YD-admin.pc.in"  "YD-admin.pc"  @ONLY)

# configure config.h
configure_file ("config.h.in" "config.h" @ONLY)

# "make install" rules
install (TARGETS YD-admin YD-server YD-client
         EXPORT YD-targets
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)
if (${ENABLE_BEDROCK})
    install (TARGETS YD-bedrock-module
             ARCHIVE DESTINATION lib
             LIBRARY DESTINATION lib)
endif ()
install (EXPORT YD-targets
         DESTINATION ${YD-pkg}
         FILE "YD-targets.cmake")
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/YD-config.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/YD-config-version.cmake"
         DESTINATION ${YD-pkg} )
install (DIRECTORY ../include/YD
         DESTINATION include
         FILES_MATCHING PATTERN "*.h")
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/YD-server.pc"
               "${CMAKE_CURRENT_BINARY_DIR}/YD-client.pc"
               "${CMAKE_CURRENT_BINARY_DIR}/YD-admin.pc"
         DESTINATION "lib/pkgconfig/")
