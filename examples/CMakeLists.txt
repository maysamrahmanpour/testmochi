add_executable (example-server ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)
target_link_libraries (example-server YD-server)

add_executable (example-admin ${CMAKE_CURRENT_SOURCE_DIR}/admin.cpp)
target_link_libraries (example-admin YD-admin)

add_executable (example-client ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp)
target_link_libraries (example-client YD-client)
