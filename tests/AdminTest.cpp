/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#include <YD/Admin.hpp>
#include <YD/Provider.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

static const std::string resource_type = "dummy";
static constexpr const char* resource_config = "{ \"path\" : \"mydb\" }";

TEST_CASE("Admin tests", "[admin]") {

    auto engine = thallium::engine("na+sm", THALLIUM_SERVER_MODE);
    // Initialize the provider
    YD::Provider provider(engine);

    SECTION("Create an admin") {
        YD::Admin admin(engine);
        std::string addr = engine.self();

        SECTION("Create and destroy resources") {
            YD::UUID resource_id = admin.createResource(addr, 0, resource_type, resource_config);

            REQUIRE_THROWS_AS(admin.createResource(addr, 0, "blabla", resource_config),
                              YD::Exception);

            admin.destroyResource(addr, 0, resource_id);

            YD::UUID bad_id;
            REQUIRE_THROWS_AS(admin.destroyResource(addr, 0, bad_id), YD::Exception);
        }
    }
    // Finalize the engine
    engine.finalize();
}
