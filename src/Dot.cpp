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


//MOVEMENT UPDATE
void Dot::updateVelocity()
{
    // mVelX = Velx;
    // mVelY = Vely;

    Velx = mVelX;
    Vely = mVelY;
}

void Dot::move()
{
    PosX = mPosX;
    PosY = mPosY;
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


//CHECK COLLISION
void Dot::check_collision( float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, Mouse mouse, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index)
{
    //3x3 Grid of spacial hashes
    std::vector<int> spacial_hashesX;
    std::vector<int> spacial_hashesY;
    this->compute_spacial_coords();
    spacial_hash = compute_spacial_hash(spacialX, spacialY);

    spacial_hashesX.push_back(spacial_hash);
    spacial_hashesX.push_back(compute_spacial_hash(spacialX + 1, spacialY));
    spacial_hashesX.push_back(compute_spacial_hash(spacialX - 1, spacialY));

    spacial_hashesY.push_back(spacial_hash);
    spacial_hashesY.push_back(compute_spacial_hash(spacialX, spacialY + 1));
    spacial_hashesY.push_back(compute_spacial_hash(spacialX, spacialY - 1));
    
    //Move the dot left or right FRAMES
    mPosX += mVelX * deltaTime;
    shiftColliders();

    // // Rolling friction
    // if ( mPosY + mCollider.r >= SCREEN_HEIGHT){
    //     mVelX /= hFriction;
    //     // if ( mVelX > 0 ){
    //     //     mVelX -= hFriction;
    //     // }
    //     // else if ( mVelX < 0 ){
    //     //     mVelX += hFriction;
    //     // }
    // }


    for (int hash : spacial_hashesX)
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
            cLVector = this->checkDotCollision(dot);
            if (cLVector.didCollide)
            {
                //Move back FRAMES
                mPosX -= mVelX * deltaTime;

                //Reverse velocity
                mVelX = -( mVelX  / (hFriction + 1) );
                // dot.mVelX = -( mVelX );
                // mVelX = -(cLVector.sa[0]);
                // dot.mVelX = -(cLVector.sb[0]);
                shiftColliders();
                // dot.shiftColliders();
            }
        }
    }

    //If the dot collided or went too far to the left or right
    if( ( mPosX - mCollider.r < 0 ) || ( mPosX + mCollider.r > SCREEN_WIDTH ) || checkCollision( mCollider, square ).didCollide || checkCollision( mCollider, mouse).didCollide )
    {
        //Move back FRAMES
        mPosX -= mVelX * deltaTime;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
        shiftColliders();
    } 

    // else if (checkCollision( mCollider, mouse).didCollide)
    // {
    //     mCollider.x
    // }
    

    mVelY += accel * deltaTime;

    //Move the dot up or down
    mPosY += (mVelY) * deltaTime;
    shiftColliders();

    for (int hash : spacial_hashesY)
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
            cLVector = this->checkDotCollision(dot);
            if (cLVector.didCollide)
            {
                //FRAMES
                mPosY -= (mVelY) * deltaTime;

                // Reverse velocity
                mVelY = -(mVelY - vfriction);
                // dot.mVelY = -( mVelY );

                // mVelY = -(cLVector.sa[1] + grav_speed - vfriction);
                // dot.mVelY = -(cLVector.sb[1] + grav_speed - vfriction);
                shiftColliders();
                // dot.shiftColliders();
            }
        }
    }
    //Check for ceiling collision
    if( ( mPosY - mCollider.r < 0 ) || ( mPosY + mCollider.r > SCREEN_HEIGHT ) || checkCollision( mCollider, square ).didCollide || checkCollision( mCollider, mouse).didCollide )
    {
        //FRAMES
        mPosY -= (mVelY) * deltaTime;

        // Reverse velocity
        mVelY = -(mVelY - vfriction);
        shiftColliders();
    } 

    //Check for collision with circle (CHANGE TO CHECK COLLISION WITH ANOTHER DOT) checkCollision( DOTA, DOTB )
    //Use velocities to calculate the collision vector and carried over velocity
    // cLVector = checkCollision( mCollider, circles );

    // for (Dot &dot: circles){
    //     if ( &dot == this )
    //     {
    //         continue;
    //     }
    //     cLVector = this->checkDotCollision(dot);
    //     if (cLVector.didCollide){
    //         // Reverse velocity
    //         mVelX = -(mVelX);
    //         mVelY = -(mVelY + grav_speed - vfriction);

    //         // mVelX = -(cLVector.sa[0]);
    //         // mVelY = -(cLVector.sa[1] + grav_speed - vfriction);

    //         // dot.mVelX = -(cLVector.sb[0]);
    //         // dot.mVelY = -(cLVector.sb[1] + grav_speed - vfriction);

    //         mPosX -= cLVector.v[0];
    //         mPosY -= cLVector.v[1];

    //         grav_speed = 0;
    //         shiftColliders();
    //         break;
    //     }
    // }
}

void Dot::check_vector_collision(float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index)
{
    std::vector<int> spacial_hashes_full = this->compute_full_spacial_area();

    //3x3 Grid of spacial hashes
    // std::vector<int> spacial_hashes_full;
    // this->compute_spacial_coords();
    // spacial_hash = compute_spacial_hash(spacialX, spacialY);

    // spacial_hashes_full.push_back(spacial_hash);
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX + 1, spacialY));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX - 1, spacialY));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX, spacialY + 1));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX, spacialY - 1));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX + 1, spacialY + 1));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX + 1, spacialY - 1));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX - 1, spacialY + 1));
    // spacial_hashes_full.push_back(compute_spacial_hash(spacialX - 1, spacialY - 1));

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
            cLVector = this->checkDotCollision(dot);
            if (cLVector.didCollide)
            {
                //Move back FRAMES
                mPosX += cLVector.sa[0];
                mPosY += cLVector.sa[1];
                // mPosX -= Velx * deltaTime;
                // mPosY -= Vely * deltaTime;

                setmVelX( -cLVector.v[0] );
                setmVelY( -cLVector.v[1] );

                dot.setmVelX( cLVector.v[0] );
                dot.setmVelY( cLVector.v[1] );

                // //Reverse velocity
                // mVelX = -( mVelX  / (hFriction + 1) );
                // // Reverse velocity
                // mVelY = -(mVelY - vfriction);


                // dot.mVelY = -( mVelY );
                // dot.mVelX = -( mVelX );
                // mVelX = -(cLVector.sa[0]);
                // dot.mVelX = -(cLVector.sb[0]);
                // shiftColliders();
                // dot.shiftColliders();
            }
        }
    }

    //If the dot collided or went too far to the left or right
    if( ( mPosX - mCollider.r < 0 ) )
    {
        //Move back FRAMES
        // mPosX -= Velx * deltaTime;
        mPosX = mCollider.r;

        //Reverse velocity
        mVelX = -( mVelX / (hFriction + 1) );
        // shiftColliders();
    } 

    if ( ( mPosX + mCollider.r > SCREEN_WIDTH ) )
    {
        mPosX = SCREEN_WIDTH - mCollider.r;
        mVelX = -( mVelX / (hFriction + 1) );
    }

    //Check for ceiling collision
    if( ( mPosY - mCollider.r < 0 ) )
    {
        //FRAMES
        // mPosY -= (Vely) * deltaTime;
        mPosY = mCollider.r;

        // Reverse velocity
        mVelY = -(mVelY - vfriction);
        // shiftColliders();
    } 

    if( ( mPosY + mCollider.r > SCREEN_HEIGHT ) )
    {
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
    // float averageVelX = (abs(dotAp.getVelX()) + abs(dotB.getVelX())) / 2;
    // float averageVelY = (abs(dotAp.getVelY()) + abs(dotB.getVelY())) / 2;

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

        // collision.sa.push_back(dotB.getVelX());
        // collision.sa.push_back(dotB.getVelY());

        // collision.sb.push_back(dotAp.getVelX());
        // collision.sb.push_back(dotAp.getVelY());

        // collision.sa.push_back(averageVelX);
        // collision.sa.push_back(averageVelY);

        // collision.sb.push_back(averageVelX);
        // collision.sb.push_back(averageVelY);
        return collision;
    }

    //If not
    return collision;
}


//SPACIAL HASHING
void Dot::compute_spacial_area(){
    //3x3 Grid of spacial hashes
    std::vector<int> spacial_hashesX;
    std::vector<int> spacial_hashesY;
    this->compute_spacial_coords();
    spacial_hash = compute_spacial_hash(spacialX, spacialY);

    spacial_hashesX.push_back(spacial_hash);
    spacial_hashesX.push_back(compute_spacial_hash(spacialX + 1, spacialY));
    spacial_hashesX.push_back(compute_spacial_hash(spacialX - 1, spacialY));

    spacial_hashesY.push_back(spacial_hash);
    spacial_hashesY.push_back(compute_spacial_hash(spacialX, spacialY + 1));
    spacial_hashesY.push_back(compute_spacial_hash(spacialX, spacialY - 1));
}

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
