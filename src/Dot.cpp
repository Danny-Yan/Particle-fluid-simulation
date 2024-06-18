#include "Dot.h"

Dot::Dot( int x, int y, int velX, int velY, int radius)
{
    //Initialize the offsets
    PosX = x;
    PosY = y;
    mPosX = x;
    mPosY = y;

    //Set collision circle size
    mCollider.r = radius * HITBOX_SCALE;

    //Initialize the velocity
    Velx = velX;
    Vely = velY;
    mVelX = velX;
    mVelY = velY;

    //Move collider relative to the circle
    shiftColliders();
}

void Dot::moveVector( float deltaTime )
{
    mPosX += mVelX * deltaTime;

    mVelY += accel * deltaTime;
    mPosY += mVelY * deltaTime;

    Velx = mVelX;
    Vely = mVelY;
    // Useless conversion
    PosX = mPosX;
    PosY = mPosY;
    shiftColliders();
}

void Dot::check_vector_collision( Mouse mouse, float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index)
{
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = this->compute_full_spacial_area();

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        if ( key == INT_MAX){
            continue;
        }
        for (int i = key; i < circles.size(); i++)
        {
            if (particleHashEntries[i].hash != hash){
                break;
            }

            Dot &dot = circles[particleHashEntries[i].index];
            if (&dot == this)
            {
                continue;
            }

            //COLLISION CHECK
            cLVector = this->checkDotCollision(dot);
            if (cLVector.didCollide)
            {
                //Move back
                mPosX += cLVector.sa[0];
                mPosY += cLVector.sa[1];

                //Momentum transfer
                setmVelX( -cLVector.v[0] );
                setmVelY( -cLVector.v[1] );

                dot.setmVelX( cLVector.v[0] );
                dot.setmVelY( cLVector.v[1] );
            }
        }
    }

    //SCREEN CHECKS 
    if( ( mPosX - mCollider.r < 0 ) )
    {
        //Move back
        mPosX = mCollider.r;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
    } 

    if ( ( mPosX + mCollider.r > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX = SCREEN_WIDTH - mCollider.r;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
    }

    //Check for ceiling collision
    if( ( mPosY - mCollider.r < 0 ) )
    {
        //Move back
        mPosY = mCollider.r;

        // Reverse velocity
        mVelY = -(mVelY - vfriction);
    } 

    if( ( mPosY + mCollider.r > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY = SCREEN_HEIGHT - mCollider.r;

        // Reverse velocity
        mVelY = -(mVelY - vfriction);
    }
}

//Dot/Dot collision detector (DONT NEED)
Collision Dot::checkDotCollision( Dot & dotB )
{
    Dot dotAp = *this;
    Collision collision;

    // // Dont check collision if both dots are stationary
    // if (dotAp.mVelX == 0 && dotB.mVelX == 0 && dotAp.mVelY == 0 && dotB.mVelY == 0){
    //     return collision;    
    // }

    Circle a = dotAp.getColliders();
    Circle b = dotB.getColliders();
    //Calculate total radius squared
    int totalRadius = a.r + b.r;
    float totalRadiusSquared = totalRadius * totalRadius;
    float distance_Squared = distanceSquared( a.x, a.y, b.x, b.y );

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distance_Squared <= ( totalRadiusSquared ) )
    {
        //Calculate the normal
        float magnitude = sqrt(distance_Squared);
        float normalX = (a.x - b.x) / magnitude;
        float normalY = (a.y - b.y) / magnitude;

        float relVecX = dotAp.getVelX() - dotB.getVelX();
        float relVecY = dotAp.getVelY() - dotB.getVelY();

        float dotProd = normalX * relVecX + normalY * relVecY;

        float impulseX = normalX * dotProd / 2;
        float impulseY = normalY * dotProd / 2;

        float radius_ratio = (float)totalRadius / magnitude;

        //The circles have collided
        collision.v.push_back(impulseX);
        collision.v.push_back(impulseY);
        collision.sa.push_back(radius_ratio * (a.x - b.x) - (a.x - b.x));
        collision.sa.push_back(radius_ratio * (a.y - b.y) - (a.y - b.y));

        collision.didCollide = true;
        return collision;
    }

    //If not
    return collision;
}

//SPACIAL HASHING
std::vector<int> Dot::compute_full_spacial_area(){
    std::vector<int> spacial_hashes_full;
    this->compute_spacial_coords();
    spacial_hash = compute_spacial_hash(spacialX, spacialY);


    // for (auto const& [index_x, index_y] : spacial_map){
    //     spacial_hashes_full.push_back(compute_spacial_hash(spacialX + index_x, spacialY + index_y));
    // }

    spacial_hashes_full.push_back(spacial_hash);
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX + 1, spacialY));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX - 1, spacialY));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX, spacialY + 1));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX, spacialY - 1));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX + 1, spacialY + 1));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX + 1, spacialY - 1));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX - 1, spacialY + 1));
    spacial_hashes_full.push_back(compute_spacial_hash(spacialX - 1, spacialY - 1));

    return spacial_hashes_full;
}

void Dot::compute_spacial_coords()
{
    spacialX = (int)mPosX / GRID_WIDTH;
    spacialY = (int)mPosY / GRID_HEIGHT;
}

int Dot::compute_spacial_hash(int spacialX, int spacialY)
{
    int p1 = 65537;
    int p2 = 181081;
    int cell_hash = (spacialX * p1 + spacialY * p2) % PARTICLE_NUM;
    return cell_hash;
}


//SHIFT COLLIDERS
void Dot::shiftColliders()
{
    //Align collider to center of dot
    mCollider.x = mPosX;
    mCollider.y = mPosY;
}

Circle& Dot::getColliders()
{
    return mCollider;
}


//GETS/SETS
float Dot::getVelX()
{
    return Velx;
}

float Dot::getVelY()
{
    return Vely;
}

void Dot::setmVelX( float velX )
{
    mVelX += velX;
}

void Dot::setmVelY( float velY )
{
    mVelY += velY;
}

int Dot::getSpacialX()
{
    return spacialX;
}

int Dot::getSpacialY()
{
    return spacialY;
}


//RENDER
void Dot::render( SDL_Renderer* gRenderer, LTexture& gDotTexture)
{
    //Show the dot
    gDotTexture.render( gRenderer, PosX - mCollider.r, PosY - mCollider.r, SCALE);
}
