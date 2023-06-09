/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include <YD/Client.hpp>
#include <YD/Provider.hpp>
#include <YD/ResourceHandle.hpp>
#include <YD/Admin.hpp>

static constexpr const char* resource_config = "{ \"path\" : \"mydb\" }";
static const std::string resource_type = "dummy";

TEST_CASE("Client test", "[client]") {

    auto engine = thallium::engine("na+sm", THALLIUM_SERVER_MODE);
    // Initialize the provider
    YD::Provider provider(engine);
    YD::Admin admin(engine);
    std::string addr = engine.self();
    auto resource_id = admin.createResource(addr, 0, resource_type, resource_config);

    SECTION("Open resource") {
        YD::Client client(engine);
        std::string addr = engine.self();

        YD::ResourceHandle my_resource = client.makeResourceHandle(addr, 0, resource_id);
        REQUIRE(static_cast<bool>(my_resource));

        auto bad_id = YD::UUID::generate();
        REQUIRE_THROWS_AS(client.makeResourceHandle(addr, 0, bad_id), YD::Exception);
    }

    admin.destroyResource(addr, 0, resource_id);
    engine.finalize();
}
