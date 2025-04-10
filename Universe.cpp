// Copyright 2025 <Mark Daoud>
#include "Universe.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

static constexpr double GravitationalConstant = 6.67e-11;

namespace NB {
Universe::Universe() : _radius(0) {}

Universe::Universe(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Error: could not open file " << filename << std::endl;
      return;
    }
    file >> *this;
}

void Universe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& body : bodies) {
      target.draw(*body, states);
    }
}

std::istream& operator>>(std::istream& is, Universe& uni) {
    size_t numBodies;
    is >> numBodies >> uni._radius;

    uni.bodies.clear();
    for (size_t i = 0; i < numBodies; ++i) {
        auto body = std::make_unique<CelestialBody>();
        is >> *body;
        uni.bodies.push_back(std::move(body));
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Universe& uni) {
    os << uni.bodies.size() << "\n" << uni._radius << "\n";
    for (const auto& body : uni.bodies) {
      os << *body << "\n";
    }
    return os;
}

const CelestialBody& Universe::operator[](size_t index) const {
    if (index < bodies.size()) {
        return *bodies[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

void Universe::transformCoordinates(int windowSize) {
    for (auto& body : bodies) {
        body->transformCoordinates(_radius, windowSize);
    }
}

void Universe::step(double dt) {
    for (auto& body : bodies) {
        sf::Vector2f totalForce(0, 0);
        for (const auto& otherBody : bodies) {
            if (body.get() != otherBody.get()) {
                totalForce += calculateForce(*body, *otherBody);
            }
        }
        body->applyForce(totalForce, dt);
    }

    for (auto& body : bodies) {
        body->update(dt);
    }
}

sf::Vector2f Universe::calculateForce(const CelestialBody& body1,
const CelestialBody& body2) const {
    sf::Vector2f direction = body2.position() - body1.position();
    double distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    double force = (GravitationalConstant * body1.mass() * body2.mass()) / (distance * distance);
    return (direction / static_cast<float>(distance)) * static_cast<float>(force);
}

}  // namespace NB
