#include "../../../incl/Dot.h"

int Dot::globalID = 0;

Dot::Dot( float x, float y , float velX, float velY, int radius) 
    : CircleCollider(x, y, radius, (int)FORCE_RADIUS),
	mVelX(velX), mVelY(velY),
	Velx(velX), Vely(velY),

    PosX(x), PosY(y),
	sPosX(x), sPosY(y)
{
    id = globalID++;
    //Move collider relative to the circle
    shiftColliders();
}

void Dot::movePrediction(float deltaTime, float constantDelta = 0)
{
    //  Predicted position
    sPosX = mPosX + constantDelta;
    sPosY = mPosY + constantDelta;

    // Initial position after velocity applied
    mPosX += mVelX * deltaTime;
    mVelY += accel * deltaTime;
    mPosY += mVelY * deltaTime;

    Velx = mVelX;
    Vely = mVelY;
    shiftColliders();
}

void Dot::move(float deltaTime)
{
    PosX = mPosX;
    PosY = mPosY;
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
    float distance_Squared = Helper::distanceSquared( a.x, a.y, b.x, b.y );

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

// //Dot/Dot collision detector
void Dot::applyDotCollison( Dot &dotB )
{
    cLVector = this->checkDotCollision( dotB );
    if (cLVector.didCollide)
    {
        //Momentum transfer
        addmVelX( -cLVector.v[0] );
        addmVelY( -cLVector.v[1] );

        dotB.addmVelX( cLVector.v[0] );
        dotB.addmVelY( cLVector.v[1] );
    }
}
void Dot::applyDotCollisons(std::vector<Dot> &dots){
    for (Dot dotB: dots){
        cLVector = this->checkDotCollision( dotB );
        if (cLVector.didCollide)
        {
            //Momentum transfer
            addmVelX( -cLVector.v[0] );
            addmVelY( -cLVector.v[1] );

            dotB.addmVelX( cLVector.v[0] );
            dotB.addmVelY( cLVector.v[1] );

            shiftColliders();
        }
    }
}

void Dot::applyDotCollisons(std::vector<Dot*> &dots){
    for (Dot *dotB: dots){
        Dot &dotBp = *dotB;
        cLVector = this->checkDotCollision( dotBp );
        if (cLVector.didCollide)
        {
            //Momentum transfer
            addmVelX( -cLVector.v[0] );
            addmVelY( -cLVector.v[1] );

            dotBp.addmVelX( cLVector.v[0] );
            dotBp.addmVelY( cLVector.v[1] );

            shiftColliders();
        }
    }
}

//FORCE
void Dot::check_vector_force( Dot &dot )
{
    //COLLISION CHECK
    cLVector = this->checkCircleForce( dot );
    if (cLVector.didCollide)
    {
        //Momentum transfer
        addmVelX( -cLVector.v[0] * dot.forceMultiplier );
        addmVelY( -cLVector.v[1] * dot.forceMultiplier );

        dot.addmVelX( cLVector.v[0] * this->forceMultiplier );
        dot.addmVelY( cLVector.v[1] * this->forceMultiplier );
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
        mVelX = -( mVelX );

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;

        shiftColliders();
    } 

    if ( ( mPosX + mCollider.r > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX = SCREEN_WIDTH - mCollider.r;
        mPosY -= (SCREEN_WIDTH - mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX );

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;

        shiftColliders();
    }

    //Check for ceiling/floor collision
    if( ( mPosY - mCollider.r < 0 ) )
    {
        //Move back
        mPosY = mCollider.r;
        mPosX -= (mPosY - mCollider.r) * mVelX / mVelY;

        // Reverse velocity
        mVelY = -(mVelY);

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;

        shiftColliders();
    } 

    if( ( mPosY + mCollider.r > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY = SCREEN_HEIGHT - mCollider.r;
        mPosX -= (SCREEN_HEIGHT - mPosY - mCollider.r) * mVelX / mVelY;
        
        // Reverse velocity
        mVelY = -(mVelY);

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;

        shiftColliders();
    }
}

void Dot::check_wall_no_shift(){
    //Check for screen wall collision
    if( ( mPosX - mCollider.r < 0 ) && mVelX != 0)
    {
        //Move back
        mPosX = mCollider.r;
        mPosY -= (mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX );

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;
    } 

    if ( ( mPosX + mCollider.r > SCREEN_WIDTH ) && mVelX != 0)
    {
        //Move back
        mPosX = SCREEN_WIDTH - mCollider.r;
        mPosY -= (SCREEN_WIDTH - mPosX - mCollider.r) * mVelY / mVelX;

        //Reverse velocity
        mVelX = -( mVelX );

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;
    }

    //Check for ceiling/floor collision
    if( ( mPosY - mCollider.r < 0 ) && mVelY != 0)
    {
        //Move back
        mPosY = mCollider.r;
        mPosX -= (mPosY - mCollider.r) * mVelX / mVelY;

        // Reverse velocity
        mVelY = -(mVelY);

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;
    } 

    if( ( mPosY + mCollider.r > SCREEN_HEIGHT )  && mVelY != 0)
    {
        //Move back
        mPosY = SCREEN_HEIGHT - mCollider.r;
        mPosX -= (SCREEN_HEIGHT - mPosY - mCollider.r) * mVelX / mVelY;
        
        // Reverse velocity
        mVelY = -(mVelY);

        // Friction
        mVelX *= RESISTANCE;
        mVelY *= RESISTANCE;
    }
}

void Dot::check_mouse_force( Mouse *mP )
{
    Mouse &mouse = *mP;

    // Dont check collision if mouse is not clicked
    if (mouse.getForceMultiplier() == 0.0f){return;}

    //COLLISION CHECK
    cLVector = this->checkCircleForce( mouse );
    if (cLVector.didCollide && this->density > DENSITY_UPPER)
    {
        //Momentum transfer
        addmVelX( -cLVector.v[0] * mouse.getForceMultiplier() );
        addmVelY( -cLVector.v[1] * mouse.getForceMultiplier() );
    }
}

//Dot/Dot collision detector
Collision Dot::checkCircleForce( CircleCollider &dotB )
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
    float totalRadius = dotAp.getfR() + dotB.getfR();
    float totalRadiusSquared = totalRadius * totalRadius;
    float distance_Squared = Helper::distanceSquared( a.x, a.y, b.x, b.y );

    if (distance_Squared > totalRadiusSquared || distance_Squared == 0){return collision;}

    //If the distance between the centers of the circles is less than the sum of their radii


    // printf("AX %d\n", a.x);
    // printf("AY %d\n", a.y);
    // printf("BX %d\n", b.x);
    // printf("BY %d\n", b.y);

    // printf("TOTAL RADIUS SQUARED %f\n", totalRadiusSquared);
    // printf("DISTANCE SQUARED %f\n", distance_Squared);
    //Calculate the impulse vector
    float magnitude = sqrt(distance_Squared);
    float normalX = (a.x - b.x) / magnitude;
    float normalY = (a.y - b.y) / magnitude;


    // float force_factor = std::max(0.0f, std::min( BARRIER_HEIGHT / BARRIER_WIDTH * magnitude + BARRIER_HEIGHT, -BARRIER_HEIGHT / BARRIER_WIDTH * magnitude + BARRIER_HEIGHT));

    float force_factor = std::max(0.0f, totalRadius - magnitude) / totalRadius;

    // float relVecX = dotAp.getVelX() - dotB.getVelX();
    // float relVecY = dotAp.getVelY() - dotB.getVelY();

    // float dotProd = normalX * relVecX + normalY * relVecY;
    // float shared_density = sharedDensity( dotAp.getDensity(), dotB.getDensity());

    float impulseX = normalX * force_factor * force_factor * force_factor;
    float impulseY = normalY * force_factor * force_factor * force_factor;


    //The circles have collided
    collision.v.push_back(-impulseX);
    collision.v.push_back(-impulseY);
    collision.didCollide = true;

    return collision;
    
}

//GETS/SETS
float Dot::getVelX() const
{
    return Velx;
}

float Dot::getVelY() const
{
    return Vely;
}

float Dot::getmVelX() const
{
    return mVelX;
}
float Dot::getmVelY() const
{
    return mVelY;
}

// CHANGE TO ADD ACCELERATION
void Dot::addmVelX( float velX )
{
    mVelX += velX;
}

void Dot::addmVelY( float velY )
{
    mVelY += velY;
}

float Dot::getPosX() const
{
    return PosX;
}

float Dot::getPosY() const
{
    return PosY;
}

float Dot::getsPosX() const
{
    return sPosX;
}
float Dot::getsPosY() const
{
    return sPosY;
}

int Dot::getID()  const
{
    return id;
}

//RENDER
void Dot::render( SDL_Renderer* gRenderer, LTexture& gDotTexture)
{
    //Show the dot
    gDotTexture.render( gRenderer, PosX - mCollider.r, PosY - mCollider.r, SCALE);
}
