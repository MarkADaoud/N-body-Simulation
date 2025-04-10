// Copyright 2025 <Mark Daoud>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Universe.hpp"


int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <T> <dt>" << std::endl;
    return -1;
  }

  double T = std::stod(argv[1]);
  double dt = std::stod(argv[2]);

  NB::Universe universe;
  std::cin >> universe;

  sf::RenderWindow window(sf::VideoMode(800, 800), "N-Body Simulation");

  sf::Texture backgroundTexture;
  if (!backgroundTexture.loadFromFile("starfield.jpg")) {
    std::cerr << "Error loading background texture" << std::endl;
    return -1;
  }

  sf::Sprite backgroundSprite(backgroundTexture);
  backgroundSprite.setScale(
      static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
      static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

  sf::Music music;
  if (!music.openFromFile("audio.wav")) {
    std::cerr << "Error loading music" << std::endl;
    return -1;
  }
  music.setLoop(true);
  music.play();

  sf::Clock clock;
  double currentTime = 0.0;

  while (window.isOpen() && currentTime < T) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    double elapsedTime = clock.restart().asSeconds();
    currentTime += elapsedTime;
    universe.step(dt);

    window.clear();

    universe.transformCoordinates(window.getSize().x);

    window.draw(backgroundSprite);
    window.draw(universe);

    window.display();
  }

  std::cout << universe;

  return 0;
}
