#include "incl/spatial_hashers.h"


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

//SPATIAL HASHING
std::vector<int> compute_full_spatial_area(int mPosX, int mPosY){
    std::vector<int> spatial_hashes_full;
    int spatialX, spatialY;
    std::tie(spatialX, spatialY) = compute_spatial_coords(mPosX, mPosY);

     for (auto& [index_x, index_y] : spatial_map){
		 if (spatialX + index_x < 0 || spatialX + index_x > SPATIAL_WIDTH || spatialY + index_y < 0 || spatialY + index_y > SPATIAL_HEIGHT) {
			 continue;
		 }
         spatial_hashes_full.push_back(compute_spatial_hash(spatialX + index_x, spatialY + index_y));
     }

    return spatial_hashes_full;
}

std::tuple<int, int> compute_spatial_coords(int mPosX, int mPosY)
{
    int spatialX = mPosX / GRID_WIDTH;
    int spatialY = mPosY / GRID_HEIGHT;
    return { spatialX, spatialY };
}

int compute_spatial_hash(int spatialX, int spatialY)
{
    int p1 = 71993;
    int p2 = 199933;
    int cell_hash = (spatialX * p1 + spatialY * p2) % PARTICLE_NUM;
    return cell_hash;
}
