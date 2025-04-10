// Copyright 2025 <Mark Daoud>
#include <iostream>
#include "CelestialBody.hpp"


namespace NB {
CelestialBody::CelestialBody() : _mass(0), texture(std::make_shared<sf::Texture>()),
sprite(std::make_shared<sf::Sprite>()) {}



void CelestialBody::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(*sprite, states);
}

void CelestialBody::transformCoordinates(float universeRadius, int windowSize) {
    // Scale the position based on the universe radius and window size
    float scaleFactor = (windowSize / 2.0f) / universeRadius;
    sprite->setPosition(
        (-_position.x * scaleFactor) + windowSize / 2.0f,
        (_position.y * scaleFactor) + windowSize / 2.0f);
}

void CelestialBody::applyForce(const sf::Vector2f &force, double dt) {
    // Update the velocity based on the force applied
    sf::Vector2f acceleration = force / static_cast<float>(_mass);
    _velocity += acceleration * static_cast<float>(dt);
}
void CelestialBody::update(double dt) {
    // Update the position based on the velocity
    _position += _velocity * static_cast<float>(dt);
}

std::istream &operator>>(std::istream &is, CelestialBody &body) {
    // Read in the celestial body properties
    is >> body._position.x >> body._position.y >> body._velocity.x >>
    body._velocity.y >> body._mass >> body._image;

    // Load the texture and create the sprite
    body.texture = std::make_shared<sf::Texture>();
    if (!body.texture->loadFromFile(body._image)) {
      std::cerr << "Failed to load Texture " << body._image << std::endl;
    }
    body.sprite = std::make_shared<sf::Sprite>();
    body.sprite->setTexture(*body.texture);
    // Origin is set to the center of the sprite
    body.sprite->setOrigin(body.sprite->getLocalBounds().width / 2,
                           body.sprite->getLocalBounds().height / 2);

    return is;
}

std::ostream &operator<<(std::ostream &os, const CelestialBody &body) {
    os << body._position.x << " " << body._position.y << " "
       << body._velocity.x << " " << body._velocity.y << " "
       << body._mass << " " << body._image;
    return os;
}

}  // namespace NB
