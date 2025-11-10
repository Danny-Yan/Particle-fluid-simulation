#pragma once
#include <cmath>

#include "SDL3/SDL.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <array> 
#include <functional>
#include "Helper.h"
#include "Rectangle.h"
#include "Dot.h"
#include <climits>

class ParticleManager
{
public:
	ParticleManager(int particleCount);
	~ParticleManager();

	void updateDensities();
	void updateSpatialLookup();

	void calculatePressureGradient(std::vector<float>& pressureGradient, CircleCollider* dotB, CircleCollider* dotA);
	void calculatePressureGradient(std::vector<float>& pressureGradient, float magnitude, Dot* dotB, Dot* dotA);

	void forParticles(const Dot& dotA, const std::function<void(Dot&)>& func);
	void forParticlesAroundPoint(int x, int y, const std::function<void(Dot&)>& func);

	std::vector<Dot>& getDots();
	std::vector<Entry>& getHashEntries();
	std::vector<int>& getSpacialKeys();
	int getParticleCount() const;

	void createParticles();
	void createParticles(const std::function<void()>& func);

	void checkIfCollide(Dot& dot, const auto& b);
private:

	int particleCount = PARTICLE_NUM;

	//Computing full 3x3 spacial area
	std::vector<int> compute_full_spatial_area(int mPosX, int mPosY);

	//Computing spacial coordinates for a given position
	std::tuple<int, int> compute_spatial_coords(int mPosX, int mPosY);

	//Computing hash value for a set of spacial coords
	int compute_spatial_hash(int spatialX, int spatialY);

	float smoothingKernel(float distance, float radius);
	float smoothingKernelDerivative(float distance, float radius);
	std::vector<float> getRandomDirection();
	float densityToPressure(float density);

	float calcDensity(Dot& dotB, float x, float y);
	float calcDensity(float magnitude);

	Collision checkCollision(const Circle& a, const Circle& b);
	Collision checkCollision(const Circle& a, const Rectangle& b);

	std::vector<Dot> dots;
	std::vector<Entry> particleHashEntries = std::vector<Entry>(particleCount);
	std::vector<int> spacialKeys = std::vector<int>(particleCount);

	std::vector<std::tuple<int, int>> spatial_map = {
	{0, 0},
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1},
	{1, 1},
	{1, -1},
	{-1, 1},
	{-1, -1}
	};
};
