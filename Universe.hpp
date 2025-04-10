// Copyright 2025 <Mark Daoud>
#pragma once

#include <vector>
#include <memory>
#include "CelestialBody.hpp"
#include <SFML/Graphics/Drawable.hpp>

namespace NB {
class Universe : public sf::Drawable {
 public:
     Universe();
     explicit Universe(const std::string& filename);

     friend std::istream& operator>>(std::istream& is, Universe& universe);
     friend std::ostream& operator<<(std::ostream& os, const Universe& universe);
     size_t size() const { return bodies.size(); }
     double radius() const { return _radius; }

     const CelestialBody& operator[](size_t index) const;

     void transformCoordinates(int windowSize);

     void step(double dt);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
      std::vector<std::unique_ptr<CelestialBody>> bodies;
      double _radius;
      sf::Vector2f calculateForce(const CelestialBody& body1, const CelestialBody& body2) const;
};
}  // namespace NB
