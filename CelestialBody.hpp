// Copyright 2025 <Mark Daoud>
#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

namespace NB {
class CelestialBody : public sf::Drawable {
 public:
      explicit CelestialBody();

      inline sf::Vector2f position() const { return _position; }
      inline sf::Vector2f velocity() const { return _velocity; }
      inline double mass() const { return _mass; }
      inline void setVelocity(const sf::Vector2f &velocity) { _velocity = velocity; }
      void applyForce(const sf::Vector2f &force, double dt);
      void update(double dt);

      friend std::istream &operator>>(std::istream &is, CelestialBody &body);

      friend std::ostream &operator<<(std::ostream &os, const CelestialBody &body);

      void transformCoordinates(float universeRadius, int windowSize);

 protected:
      void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
      sf::Vector2f _position;
      sf::Vector2f _velocity;
      double _mass;
      std::string _image;
      std::shared_ptr<sf::Texture> texture;
      std::shared_ptr<sf::Sprite> sprite;
};
}  // namespace NB
