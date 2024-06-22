#include "Dot.h"

Dot::Dot( int x, int y , int velX, int velY, int radius) : Collider(radius)
{
    //Initialize the offsets
    PosX = x;
    PosY = y;
    mPosX = x;
    mPosY = y;

    //Initialize the velocity
    Velx = velX;
    Vely = velY;
    mVelX = velX;
    mVelY = velY;

    //Move collider relative to the circle
    shiftColliders();
}

void Dot::moveVector(float deltaTime)
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

void Dot::check_vector_collision( float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index)
{
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)mPosX, (int)mPosY);

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

                shiftColliders();
            }
        }
    }

    //Check for screen wall collision
    if( ( mPosX - mCollider.r < 0 ) )
    {
        //Move back
        mPosX = mCollider.r;
        mPosY -= (mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
        shiftColliders();
    } 

    if ( ( mPosX + mCollider.r > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX = SCREEN_WIDTH - mCollider.r;
        mPosY -= (SCREEN_WIDTH - mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
        shiftColliders();
    }

    //Check for ceiling/floor collision
    if( ( mPosY - mCollider.r < 0 ) )
    {
        //Move back
        mPosY = mCollider.r;
        mPosX -= (mPosY - mCollider.r) * mVelX / mVelY;

        // Reverse velocity
        mVelY = -(mVelY - vfriction);
        shiftColliders();
    } 

    if( ( mPosY + mCollider.r > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY = SCREEN_HEIGHT - mCollider.r;
        mPosX -= (SCREEN_HEIGHT - mPosY - mCollider.r) * mVelX / mVelY;
        
        // Reverse velocity
        mVelY = -(mVelY - vfriction);
        shiftColliders();
    }
}

//Dot/Dot collision detector
Collision Dot::checkDotCollision( Dot & dotB )
{
    Dot dotAp = *this;
    Collision collision;

    // Dont check collision if both dots are stationary
    if (dotAp.mVelX == 0 && dotB.mVelX == 0 && dotAp.mVelY == 0 && dotB.mVelY == 0){
        return collision;    
    }

    Circle a = dotAp.getColliders();
    Circle b = dotB.getColliders();
    //Calculate total radius squared
    int totalRadius = a.r + b.r;
    float totalRadiusSquared = totalRadius * totalRadius;
    float distance_Squared = distanceSquared( a.x, a.y, b.x, b.y );

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distance_Squared <= ( totalRadiusSquared ) )
    {
        //Calculate the impulse vector
        float magnitude = sqrt(distance_Squared);
        float normalX = (a.x - b.x) / magnitude;
        float normalY = (a.y - b.y) / magnitude;
        // float force_factor = std::max(0.0f, std::min(BARRIER * magnitude + 1, -BARRIER * magnitude + 1)) + 1;

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

//FORCE
void Dot::check_vector_force( Dot &dot )
{
    //COLLISION CHECK
    cLVector = this->checkCircleForce( dot );
    if (cLVector.didCollide)
    {
        //Momentum transfer
        setmVelX( -cLVector.v[0] );
        setmVelY( -cLVector.v[1] );

        dot.setmVelX( cLVector.v[0] );
        dot.setmVelY( cLVector.v[1] );
    }
}

void Dot::check_wall_collision(){
    //Check for screen wall collision
    if( ( mPosX - mCollider.r < 0 ) )
    {
        //Move back
        mPosX = mCollider.r;
        mPosY -= (mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
        shiftColliders();
    } 

    if ( ( mPosX + mCollider.r > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX = SCREEN_WIDTH - mCollider.r;
        mPosY -= (SCREEN_WIDTH - mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
        shiftColliders();
    }

    //Check for ceiling/floor collision
    if( ( mPosY - mCollider.r < 0 ) )
    {
        //Move back
        mPosY = mCollider.r;
        mPosX -= (mPosY - mCollider.r) * mVelX / mVelY;

        // Reverse velocity
        mVelY = -(mVelY - vfriction);
        shiftColliders();
    } 

    if( ( mPosY + mCollider.r > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY = SCREEN_HEIGHT - mCollider.r;
        mPosX -= (SCREEN_HEIGHT - mPosY - mCollider.r) * mVelX / mVelY;
        
        // Reverse velocity
        mVelY = -(mVelY - vfriction);
        shiftColliders();
    }
}

void Dot::check_mouse_force( Mouse &mouse )
{
    //COLLISION CHECK
    cLVector = this->checkCircleForce( mouse );
    if (cLVector.didCollide)
    {
        //Momentum transfer
        setmVelX( -cLVector.v[0] );
        setmVelY( -cLVector.v[1] );
    }
}

//Dot/Dot collision detector
Collision Dot::checkCircleForce( Collider &dotB ) // USE OOP have children as inputs where each child has a different force radius but same getColliders() method
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
    int totalRadius = dotAp.getfR() + dotB.getfR();
    float totalRadiusSquared = totalRadius * totalRadius;
    float distance_Squared = distanceSquared( a.x, a.y, b.x, b.y );

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distance_Squared <= ( totalRadiusSquared ) )
    {
        //Calculate the impulse vector
        float magnitude = sqrt(distance_Squared);
        float normalX = (a.x - b.x) / magnitude;
        float normalY = (a.y - b.y) / magnitude;
        float force_factor = std::max(0.0f, std::min( BARRIER_HEIGHT / BARRIER_WIDTH * magnitude + BARRIER_HEIGHT, -BARRIER_HEIGHT / BARRIER_WIDTH * magnitude + BARRIER_HEIGHT));

        // float force_factor = std::max(0, totalRadius - magnitude) / totalRadius;

        // float relVecX = dotAp.getVelX() - dotB.getVelX();
        // float relVecY = dotAp.getVelY() - dotB.getVelY();

        // float dotProd = normalX * relVecX + normalY * relVecY;
        // float shared_density = sharedDensity( dotAp.density, dotB.density);

        float impulseX = normalX * force_factor * force_factor * force_factor * FORCE;
        float impulseY = normalY * force_factor * force_factor * force_factor * FORCE;

        float radius_ratio = (float)totalRadius / magnitude;

        //The circles have collided
        collision.v.push_back(-impulseX);
        collision.v.push_back(-impulseY);
        collision.sa.push_back(radius_ratio * (a.x - b.x) - (a.x - b.x));
        collision.sa.push_back(radius_ratio * (a.y - b.y) - (a.y - b.y));

        collision.didCollide = true;
        return collision;
    }

    //If not
    return collision;
}


//SHIFT COLLIDERS
void Dot::shiftColliders()
{
    //Align collider to center of dot
    mCollider.x = mPosX;
    mCollider.y = mPosY;
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


float Dot::getmPosX()
{
    return mPosX;
}

float Dot::getmPosY()
{
    return mPosY;
}

//RENDER
void Dot::render( SDL_Renderer* gRenderer, LTexture& gDotTexture)
{
    //Show the dot
    gDotTexture.render( gRenderer, PosX - mCollider.r, PosY - mCollider.r, SCALE);
}
