add_executable (AdminTest AdminTest.cpp)
target_link_libraries (AdminTest PRIVATE Catch2::Catch2WithMain YD-server YD-admin)
add_test (NAME AdminTest COMMAND ./AdminTest)

add_executable (ClientTest ClientTest.cpp)
target_link_libraries (ClientTest PRIVATE Catch2::Catch2WithMain YD-server YD-client YD-admin)
add_test (NAME ClientTest COMMAND ./ClientTest)

add_executable (ResourceTest ResourceTest.cpp)
target_link_libraries (ResourceTest PRIVATE Catch2::Catch2WithMain YD-server YD-client YD-admin)
add_test (NAME ResourceTest COMMAND ./ResourceTest)
