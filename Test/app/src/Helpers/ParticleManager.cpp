#include "../../incl/ParticleManager.h"

ParticleManager::ParticleManager(int particleNumber)
{
	this->particleCount = particleNumber;
    dots.reserve(particleCount);
}

ParticleManager::~ParticleManager()
{
	dots.clear();
	particleHashEntries.clear();
	spacialKeys.clear();
	spatial_map.clear();
}

void ParticleManager::createParticles(){
    float spacing_scale = RADIUS * SCALE + SPACING;
    int particlesPerRow = (int)sqrt(PARTICLE_NUM);
    int particlesPerCol = (PARTICLE_NUM - 1) / particlesPerRow + 1;
    float x_cord;
    float y_cord;

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        // radius = std::rand() % 10 + 5;
        float x = (i % particlesPerRow - particlesPerRow / 2.0f + 0.5f) * (spacing_scale);
        float y = (i / particlesPerRow - particlesPerCol / 2.0f + 0.5f) * (spacing_scale);

        x_cord = x + PARTICLE_START_X;
        y_cord = y + PARTICLE_START_Y;
        dots.emplace_back(x_cord, y_cord, 0, 0, RADIUS);
    }
}

void ParticleManager::createParticles(const std::function<void()>& func) {
    func();
}

void ParticleManager::updateDensities()
{
    float particle_density = 0;
    for (Dot& dot : dots) {

        this->forParticles(dot, [&](Dot& dotB) {
            particle_density += calcDensity(dotB, dot.getsPosX(), dot.getsPosY());
            });

        dot.setDensity(particle_density);

        particle_density = 0;
    }
}

float ParticleManager::calcDensity(Dot& dotB, float x, float y)
{
    Circle b = dotB.getColliders();
    float influence = 0;
    float distance_squared = Helper::distanceSquared(x, y, b.x, b.y);
    if (distance_squared < FORCE_RADIUS_SQUARED && distance_squared > 0)
    {
        float distance = sqrt(distance_squared);
        influence = smoothingKernel(distance, FORCE_RADIUS);
    }

    return influence;
}

float ParticleManager::calcDensity(float magnitude)
{
    float influence = 0;
    influence = smoothingKernel(magnitude, FORCE_RADIUS);
    return influence;
}

void ParticleManager::calculatePressureGradient(std::vector<float>& pressureGradient, CircleCollider* dotB, CircleCollider* dotA)
{
    Circle a = dotA->getColliders();
    Circle b = dotB->getColliders();
    float normalX = 0;
    float normalY = 0;

    float magnitude = sqrt(Helper::distanceSquared(a.x, a.y, b.x, b.y));

    // // CHECK FOR COLLISON
    // if(distance_squared < radius_squared){
    //     dotA->applyDotCollison(*dotB);
    // }

    if (magnitude <= 0.0f)
    {
        std::vector<float> randomNormal = getRandomDirection();
        normalX = randomNormal[0];
        normalY = randomNormal[1];
    }
    else
    {
        normalX = (a.x - b.x) / magnitude;
        normalY = (a.y - b.y) / magnitude;
    }
    float slope = smoothingKernelDerivative(magnitude, FORCE_RADIUS);
    float density = Helper::sharedDensity(dotB->getDensity(), dotA->getDensity());
    density = (density != 0.0f) ? density : DENSITY_LOWER;

    pressureGradient[0] += -densityToPressure(density) * normalX * slope / density;
    pressureGradient[1] += -densityToPressure(density) * normalY * slope / density;
}

void ParticleManager::calculatePressureGradient(std::vector<float>& pressureGradient, float magnitude, Dot* dotB, Dot* dotA)
{
    Circle a = dotA->getColliders();
    Circle b = dotB->getColliders();
    float normalX;
    float normalY;

    // // CHECK FOR COLLISON
    // if(distance_squared < radius_squared){
    //     dotA->applyDotCollison(*dotB);
    // }

    if (magnitude == 0.0f)
    {
	    std::vector<float> randomNormal = getRandomDirection();
		normalX = randomNormal[0];
		normalY = randomNormal[1];
    }
    else
    {
        normalX = (a.x - b.x) / magnitude;
        normalY = (a.y - b.y) / magnitude;
    }
    float slope = smoothingKernelDerivative(magnitude, FORCE_RADIUS);
    float density = Helper::sharedDensity(dotB->getDensity(), dotA->getDensity());
    density = (density != 0.0f) ? density : DENSITY_LOWER;

    pressureGradient[0] += -densityToPressure(density) * normalX * slope / density;
    pressureGradient[1] += -densityToPressure(density) * normalY * slope / density;
}

float ParticleManager::smoothingKernel(float distance, float radius)
{
    float volume = M_PI * (pow(radius, 5) / 10);
    float force_factor = radius - distance;
    return force_factor * force_factor * force_factor / volume;
}

float ParticleManager::smoothingKernelDerivative(float distance, float radius)
{
    float volume = M_PI * (pow(radius, 5) / 10);
    float force_slope = -3 * (distance - radius) * (distance - radius) / volume;
    return force_slope;
}

std::vector<float> ParticleManager::getRandomDirection()
{
    while (true) {
        float x = std::rand() % 3 - 1;
        float y = std::rand() % 3 - 1;
		if (x == 0 && y == 0) {
			continue;
		}
        //float magnitude = sqrt(Helper::distanceSquared(0, 0, x, y));
		return Helper::unitVector(x, y);
    }
}

float ParticleManager::densityToPressure(float density)
{
    float density_error = density - DENSITY_DESIRED;
    return density_error * FORCE;
}

// For each loop around particles
void ParticleManager::forParticles(const Dot& dotA, const std::function<void(Dot&)>& func)
{
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dotA.getsPosX(), (int)dotA.getsPosY());

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        for (int i = key; i < dots.size(); i++)
        {
            if (particleHashEntries[i].hash != hash) {
                break;
            }

            Dot& dot = dots[particleHashEntries[i].index];

            if (&dot == &dotA)
            {
                continue;
            }

            // Do something
            func(dot);
        }
    }
}

// For each loop around particles
void ParticleManager::forParticlesAroundPoint(int x, int y, const std::function<void(Dot&)>& func)
{
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area(x, y);

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        for (int i = key; i < dots.size(); i++)
        {
            if (particleHashEntries[i].hash != hash) {
                break;
            }

            Dot& dot = dots[particleHashEntries[i].index];

            // Do something
            func(dot);
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
//SPATIAL HASHING
std::vector<int> ParticleManager::compute_full_spatial_area(int mPosX, int mPosY) {
    std::vector<int> spatial_hashes_full;
    int spatialX, spatialY;
    std::tie(spatialX, spatialY) = compute_spatial_coords(mPosX, mPosY);

    for (auto& [index_x, index_y] : this->spatial_map) {
        if (spatialX + index_x < 0 || spatialX + index_x > SPATIAL_WIDTH || spatialY + index_y < 0 || spatialY + index_y > SPATIAL_HEIGHT) {
            continue;
        }
        spatial_hashes_full.push_back(compute_spatial_hash(spatialX + index_x, spatialY + index_y));
    }

    return spatial_hashes_full;
}

std::tuple<int, int> ParticleManager::compute_spatial_coords(int mPosX, int mPosY)
{
    int spatialX = mPosX / GRID_WIDTH;
    int spatialY = mPosY / GRID_HEIGHT;
    return { spatialX, spatialY };
}

int ParticleManager::compute_spatial_hash(int spatialX, int spatialY)
{
    int p1 = 71993;
    int p2 = 199933;
    int cell_hash = (spatialX * p1 + spatialY * p2) % PARTICLE_NUM;
    return cell_hash;
}

// Adapted from sebastain lague's tutorial
void ParticleManager::updateSpatialLookup()
{
    // Compute spatial hash for each dot
    for (int i = 0; i < dots.size(); i++)
    {
        const Dot& dot = dots[i];
        int spatialX, spatialY;
        std::tie(spatialX, spatialY) = compute_spatial_coords((int)dot.getsPosX(), (int)dot.getsPosY());
        int spatial_hash = compute_spatial_hash(spatialX, spatialY);
        if (spatial_hash < 0) {
            printf("ID: %d\n", dots[i].getID());
			printf("Density: %f\n", dots[i].getDensity());  
            printf("POSITIONS: % d % d \n", (int)dot.getsPosX(), (int)dot.getsPosY());
            printf("VELOCITY: % d % d \n", (int)dot.getmVelX(), (int)dot.getmVelY());
			printf("SPATIAL CORD: %d %d\n", spatialX, spatialY);
            printf("NEGATIVE SPATIAL HASH: %d \n", spatial_hash);
            spatial_hash = abs(spatial_hash);
        }
        particleHashEntries[i].hash = spatial_hash;
        particleHashEntries[i].index = i;
        spacialKeys[i] = INT_MAX;
    }

    std::sort(particleHashEntries.begin(), particleHashEntries.end(), [](Entry& a, Entry& b) { return a.hash < b.hash; }); // Sort by hash

    // Compute spatial lookup
    for (int i = 0; i < dots.size(); i++)
    {
        int Key = particleHashEntries[i].hash;
        int keyPrev = (i == 0) ? INT_MAX : particleHashEntries[i - 1].hash;
        if (Key != keyPrev)
        {
            spacialKeys[Key] = i;
        }
    }
}

std::vector<Dot>& ParticleManager::getDots() {
	return dots;
}
std::vector<Entry>& ParticleManager::getHashEntries() {
	return particleHashEntries;
}
std::vector<int>& ParticleManager::getSpacialKeys() {
	return spacialKeys;
}

int ParticleManager::getParticleCount() const
{
    return particleCount;
}

// idk how to fix so that overloads are shared with parent here
void ParticleManager::checkIfCollide(Dot& dot, const auto& b){
    try
    {
        const Circle& a = dot.getColliders();
        Collision collider = this->checkCollision(a, b);
        if (collider.didCollide)
        {
        }
	}
    // Catch overload exception
	catch (const std::exception& e)
	{
        std::cerr << "Error: " << e.what() << "\n";
		return;
	}
}

//Check collision between two circles
Collision ParticleManager::checkCollision(const Circle& a, const Circle& b)
{
    Collision collision;
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if (Helper::distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
    {
        collision.didCollide = true;
        return collision;
    }

    //If not
    return collision;
}

//Check collision between circle and rectangle
Collision ParticleManager::checkCollision(const Circle& a, const Rectangle& b)
{
    //Closest point on collision box
    int cX, cY;

    Collision collision;

    //Find closest x offset
    if (a.x < b.x)
    {
        cX = b.x;
    }
    else if (a.x > b.x + b.w)
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest y offset
    if (a.y < b.y)
    {
        cY = b.y;
    }
    else if (a.y > b.y + b.h)
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if (Helper::distanceSquared(a.x, a.y, cX, cY) < a.r * a.r)
    {
        //This box and the circle have collided
        collision.didCollide = true;
        return collision;
    }

    //If the shapes have not collided
    return collision;
}
