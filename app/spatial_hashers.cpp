#include "incl/spatial_hashers.h"

//SPAtIAL HASHING
std::vector<int> compute_full_spatial_area(int mPosX, int mPosY){
    std::vector<int> spatial_hashes_full;
    int spatialX, spatialY;
    std::tie(spatialX, spatialY) = compute_spatial_coords(mPosX, mPosY);

    // for (auto const& [index_x, index_y] : spatial_map){
    //     spatial_hashes_full.push_back(compute_spatial_hash(spatialX + index_x, spatialY + index_y));
    // }

    spatial_hashes_full.push_back(compute_spatial_hash(spatialX, spatialY));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX + 1, spatialY));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX - 1, spatialY));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX, spatialY + 1));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX, spatialY - 1));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX + 1, spatialY + 1));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX + 1, spatialY - 1));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX - 1, spatialY + 1));
    spatial_hashes_full.push_back(compute_spatial_hash(spatialX - 1, spatialY - 1));

    return spatial_hashes_full;
}

std::tuple<int, int> compute_spatial_coords(int mPosX, int mPosY)
{
    int spatialX = mPosX / GRID_WIDTH;
    int spatialY = mPosY / GRID_HEIGHT;
    return std::make_tuple(spatialX, spatialY);
}

int compute_spatial_hash(int spatialX, int spatialY)
{
    int p1 = 65537;
    int p2 = 181081;
    int cell_hash = (spatialX * p1 + spatialY * p2) % PARTICLE_NUM;
    return cell_hash;
}
