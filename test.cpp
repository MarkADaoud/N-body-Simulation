// Copyright 2025 <Mark Daoud>
#define BOOST_TEST_MODULE NBodyTests
#include <sstream>
#include <boost/test/included/unit_test.hpp>
#include <SFML/Graphics.hpp>
#include "CelestialBody.hpp"
#include "Universe.hpp"

const float EPSILON = 1e-6f;

// Test CelestialBody input and output stream operators
BOOST_AUTO_TEST_CASE(test_CelestialBody_stream_operators) {
    std::stringstream ss("1.496e+11 0 0 29800 5.974e+24 earth.gif");
    NB::CelestialBody body;
    ss >> body;

    BOOST_CHECK_CLOSE_FRACTION(body.position().x, 1.496e+11, EPSILON);
    BOOST_CHECK_SMALL(body.position().y, EPSILON);
    BOOST_CHECK_SMALL(body.velocity().x, EPSILON);
    BOOST_CHECK_CLOSE(body.velocity().y, 29800.0f, EPSILON);
    BOOST_CHECK_CLOSE(body.mass(), 5.974e+24f, EPSILON);

    std::stringstream output;
    output << body;
    BOOST_CHECK_EQUAL(output.str(), "1.496e+11 0 0 29800 5.974e+24 earth.gif");
}

// Test Universe constructor
BOOST_AUTO_TEST_CASE(test_Universe_constructor) {
    NB::Universe universe;

    BOOST_CHECK_EQUAL(universe.size(), 0);
    BOOST_CHECK_SMALL(static_cast<float>(universe.radius()), EPSILON);
}

// Test Universe input and output stream operators
BOOST_AUTO_TEST_CASE(test_Universe_stream_operators) {
    std::stringstream ss("1\n2.50e+11\n1.496e+11 0 0 29800 5.974e+24 mars.gif\n");

    NB::Universe universe;
    ss >> universe;

    std::stringstream output;
    output << universe;

    BOOST_CHECK_EQUAL(output.str(), "1\n2.5e+11\n1.496e+11 0 0 29800 5.974e+24 mars.gif\n");
}

// Test Universe step function
BOOST_AUTO_TEST_CASE(test_Universe_step) {
    std::stringstream ss("1\n2.50e+11\n1.496e+11 0 0 29800 5.974e+24 earth.gif\n");
    NB::Universe universe;
    ss >> universe;

    BOOST_CHECK_CLOSE_FRACTION(universe[0].position().x, 1.496e+11, EPSILON);
    BOOST_CHECK_SMALL(universe[0].position().y, EPSILON);

    universe.step(1.0);

    BOOST_CHECK_CLOSE_FRACTION(universe[0].position().x, 1.496e+11f, EPSILON);
    BOOST_CHECK_CLOSE(universe[0].position().y, 29800.0f, EPSILON);
}

// Test multiple bodies in Universe
BOOST_AUTO_TEST_CASE(test_Universe_multiple_bodies) {
    std::stringstream ss("3\n2.50e+11\n "
                         "1.496e+11 0 0 29800 5.974e+24 earth.gif\n"
                         "2.279e+11 0 0 24100 6.419e+23 mars.gif\n"
                         "5.790e+10 0 0 47900 3.302e+23 mercury.gif\n");

    NB::Universe universe;
    ss >> universe;

    BOOST_CHECK_EQUAL(universe.size(), 3);
    BOOST_CHECK_CLOSE(static_cast<float>(universe.radius()), 2.50e+11f, EPSILON);

    // Check the positions and velocities of the bodies
    BOOST_CHECK_CLOSE_FRACTION(universe[0].position().x, 1.496e+11f, EPSILON);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 29800.0f, EPSILON);
    BOOST_CHECK_CLOSE(universe[0].mass(), 5.974e+24f, EPSILON);

    BOOST_CHECK_CLOSE(universe[1].position().x, 2.279e+11f, EPSILON);
    BOOST_CHECK_CLOSE(universe[1].velocity().y, 24100.0f, EPSILON);
    BOOST_CHECK_CLOSE_FRACTION(universe[1].mass(), 6.419e+23f, EPSILON);

    BOOST_CHECK_CLOSE_FRACTION(universe[2].position().x, 5.790e+10f, EPSILON);
    BOOST_CHECK_CLOSE(universe[2].velocity().y, 47900.0f, EPSILON);
    BOOST_CHECK_CLOSE(universe[2].mass(), 3.302e+23f, EPSILON);

    universe.step(1.0);

    // Check the positions after stepping
    BOOST_CHECK_CLOSE(universe[0].position().y, 29800.0f, EPSILON);
    BOOST_CHECK_CLOSE(universe[1].position().y, 24100.0f, EPSILON);
    BOOST_CHECK_CLOSE(universe[2].position().y, 47900.0f, EPSILON);
}

// Test physics of the Universe
BOOST_AUTO_TEST_CASE(test_Universe_physics) {
    std::stringstream ss("2\n2.50e+11\n"
                         "0 0 0 0 1.989e+30 sun.gif\n"
                         "1.496e+11 0 0 29800 5.974e+23 earth.gif\n");

    NB::Universe universe;
    ss >> universe;

    // Check the initial positions
    BOOST_CHECK_SMALL(universe[0].position().x, EPSILON);
    BOOST_CHECK_SMALL(universe[0].position().y, EPSILON);
    BOOST_CHECK_CLOSE_FRACTION(universe[1].position().x, 1.496e+11f, EPSILON);
    BOOST_CHECK_SMALL(universe[1].position().y, EPSILON);

    float initialY = universe[0].position().y;

    // Step the simulation
    double dt = 86400;
    universe.step(dt);

    BOOST_CHECK_SMALL(universe[0].position().x, 1e5f);
    BOOST_CHECK_SMALL(universe[0].position().y, 1e5f);

    BOOST_CHECK_CLOSE_FRACTION(universe[1].position().x, 1.496e+11f, 1.0f);
    BOOST_CHECK(std::abs(universe[1].position().y) > 2.5e+9f);

    // Check the velocity of the Earth
    BOOST_CHECK(universe[1].velocity().x < 0);
    BOOST_CHECK_CLOSE(universe[1].velocity().y, 29800.0f, 1.0f);
    BOOST_CHECK_GT(universe[1].position().y, initialY);
}

BOOST_AUTO_TEST_CASE(test_Universe_extreme_force_case) {
    const double G = 6.67e-11;
    const double dt = 31557600;

    std::stringstream ss("2\n1.0\n"
                         "-1.0 0 0 0 1.0 earth.gif\n"
                         "1.0 0 0 0 1.0 mars.gif\n");

    NB::Universe universe;
    ss >> universe;

    auto &body1 = universe[0];
    auto &body2 = universe[1];

    // Compute initial force magnitude
    sf::Vector2f direction = body2.position() - body1.position();
    double distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    double forceMagnitude = (G * body1.mass() * body2.mass()) / (distance * distance);
    sf::Vector2f forceOnBody1 = (direction / static_cast<float>(distance)) *
                                static_cast<float>(forceMagnitude);
    sf::Vector2f forceOnBody2 = -forceOnBody1;

    // Compute acceleration
    sf::Vector2f acceleration1 = forceOnBody1 / static_cast<float>(body1.mass());
    sf::Vector2f acceleration2 = forceOnBody2 / static_cast<float>(body2.mass());

    // Compute expected velocity update
    sf::Vector2f expectedVelocity1 = acceleration1 * static_cast<float>(dt);
    sf::Vector2f expectedVelocity2 = acceleration2 * static_cast<float>(dt);

    // Compute expected position update
    sf::Vector2f expectedPosition1 = body1.position() + expectedVelocity1 * static_cast<float>(dt);
    sf::Vector2f expectedPosition2 = body2.position() + expectedVelocity2 * static_cast<float>(dt);

    universe.step(dt);

    BOOST_CHECK_CLOSE(body1.velocity().x, expectedVelocity1.x, EPSILON);
    BOOST_CHECK_CLOSE(body1.velocity().y, expectedVelocity1.y, EPSILON);
    BOOST_CHECK_CLOSE(body2.velocity().x, expectedVelocity2.x, EPSILON);
    BOOST_CHECK_CLOSE(body2.velocity().y, expectedVelocity2.y, EPSILON);

    BOOST_CHECK_CLOSE(body1.position().x, expectedPosition1.x, EPSILON);
    BOOST_CHECK_CLOSE(body1.position().y, expectedPosition1.y, EPSILON);
    BOOST_CHECK_CLOSE(body2.position().x, expectedPosition2.x, EPSILON);
    BOOST_CHECK_CLOSE(body2.position().y, expectedPosition2.y, EPSILON);
}

BOOST_AUTO_TEST_CASE(test_inverse_axis) {
    const double dt = 31557600;

    std::stringstream ss("2\n1.0\n"
                             "0 -1.0 0 0 2.0 earth.gif\n"
                             "0 1.0 0 0 0.5 mars.gif\n");
    NB::Universe universe;
    ss >> universe;

    auto &earth = universe[0];
    auto &mars = universe[1];

    universe.step(dt);

    BOOST_CHECK_GT(earth.velocity().y, 0.0f);
    BOOST_CHECK_LT(mars.velocity().y, 0.0f);
    BOOST_CHECK_CLOSE(earth.velocity().y * earth.mass(),
                      -mars.velocity().y * mars.mass(), EPSILON);
}

