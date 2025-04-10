# PS3: N-Body Simulation

## Contact
Name: Mark Daoud
Section: 201
Time to Complete: 5 hours


## Description
The projects reads from a file containing a number of planets along with a radius and positions, velocity, mass, and an image source file for each planet. It then intializes a Universe class using that data which
reads it into a Celestial body object for each planet. It then uses the overriden draw functions to draw the Universe with all the planets. The program then calls universe's step function which using physics computes 
new positions for planets based on force, velocity and acceleration and updates the positions of the sprites. 

### Features
I used a vector to store the positions and velocity for each planet to make it easily accesible as well as a float for the mass. Used smart pointers for texture and sprite for better memory handling. Created a transform 
coordinates function to make sure Universe starts at center of window. Stored bodies for universe as a vector of unique_ptrs of class Celestial bodies for better handaling of planets. Created a calculate force function to make 
it easier to calculate the force between two planets. Also created an apply force function in celestialbody class to compute acceleration based on force and update the velocity. 

### Memory
I used smart pointers to manage the memory with my textures and sprites. I made them std::shared_ptr which allowed me to load textures and sprites to ensure proper memory handaling using std::make_shared when loading. 

### Issues
When using audio file towards the end when starting to loop it starts buffering and sometimes stops playing. 

### Extra Credit
I drew a background image using the starfield.jpg file and made sure it fit in the window by setting its scale based on the window size / sprite size. 
I also utilized sf::Music to play an audio using a downloaded wav file. 

## Acknowledgements
audio.wav - used convertor to convert from mp3
https://www.sfml-dev.org/tutorials/2.5/audio-sounds.php
Dr.Daly