# Fluid particle simulator
A simple particle based fluid simulation written in SDL3 and C++  

Inspired by Sebastian Lague's Particle Simulation Coding Adventure  
https://www.youtube.com/watch?v=rSKMYc1CQHE  

![alt text](https://github.com/Danny-Yan/Particle-fluid-simulation/blob/main/images/fluid-sim-screenshot.png "Fluid simulation png")

### To run
```
git clone https://github.com/Danny-Yan/Particle-fluid-simulation.git
cd Particle-fluid-simulation
cmake -S . -B build -G Ninja
cmake --build build
exec ./build/particle-simulation
```