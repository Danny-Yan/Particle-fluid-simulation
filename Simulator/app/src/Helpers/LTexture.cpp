#include "../../incl/LTexture.h"

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( SDL_Renderer* gRenderer, std::string path )
{
    //Get rid of preexisting texture
    free();

     //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image (Cyan screen)
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromXPM(SDL_Renderer* gRenderer, char** path) {
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_ReadXPMFromArrayToRGB888(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    }
    else
    {
        //Color key image (Cyan screen)
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

// ---------------------------------------------------------------------------------------------
// RGB implementation (OLD)
// Set blending function
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

//Set blending function
void LTexture::setColorForSpeed(int speed) {
    const std::array<Uint8, 3>& colors = colourProcessorRGBtoRGB(speed);
    setColor(colors[0], colors[1], colors[2]);
}

std::array<Uint8, 3> LTexture::colourProcessorRGBtoRGB(int speed) {
    std::array<Uint8, 3> rgbArr;
    rgbArr[0] = (Uint8)colorSpeedLinearisation(speed, COLOR_MAX_SPEED, COLOR_MIN[0], COLOR_MAX[0]); // red
    rgbArr[1] = (Uint8)colorSpeedLinearisation(speed, COLOR_MAX_SPEED, COLOR_MIN[1], COLOR_MAX[1]); // green
    rgbArr[2] = (Uint8)colorSpeedLinearisation(speed, COLOR_MAX_SPEED, COLOR_MIN[2], COLOR_MAX[2]); // blue
    //printf("Speed: %d, R: %d, G: %d, B: %d\n", speed, rgbArr[0], rgbArr[1], rgbArr[2]);
    return rgbArr;
}

// Linear interpolation of colour against speed
float LTexture::colorSpeedLinearisation(float speed, float color_max_speed, float color_min, float color_max) {
    return (color_max - color_min) * (float)(speed / color_max_speed) + color_min;
}

// ----------------------------------------------------------------------------------------------
// HSL implementation
//Set blending function
void LTexture::setColorForSpeedHSL(float speed) {
    const std::array<Uint8, 3>& colors = colourLinearisationHSL(speed);
    setColor(colors[0], colors[1], colors[2]);
}

// Linear interpolation of colour against speed using HSL
std::array<Uint8, 3> LTexture::colourLinearisationHSL(float speed) {
	float h = colorSpeedLinearisation(speed, COLOR_MAX_SPEED, COLOR_H_MIN[0], COLOR_H_MAX[0]) / 360.0f;
	float s = colorSpeedLinearisation(speed, COLOR_MAX_SPEED, COLOR_H_MIN[1], COLOR_H_MAX[1]);
	float l = colorSpeedLinearisation(speed, COLOR_MAX_SPEED, COLOR_H_MIN[2], COLOR_H_MAX[2]);
	// Convert HSL to RGB
	std::array<Uint8, 3> rgb = hslToRgb(h, s, l);
	//printf("Speed: %f, H: %f, S: %f, L: %f, R: %d, G: %d, B: %d\n", speed, h, s, l, rgb[0], rgb[1], rgb[2]);
	return rgb;  
}

//Set color modulation HSL
void LTexture::setColorHSL(float h, float s, float l) {
    // Convert HSL to RGB
    std::array<Uint8, 3> rgb = hslToRgb(h, s, l);
    // Set the color modulation
    setColor(rgb[0], rgb[1], rgb[2]);
}

// HSL to RGB conversion
std::array<Uint8, 3> LTexture::hslToRgb(float h, float s, float l) {
    float r, g, b;

    if (s == 0) {
        r = g = b = l; // achromatic
    }
    else {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        r = hueToRgb(p, q, h + 1.0 / 3.0);
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h - 1.0 / 3.0);
    }
    
    return {
        static_cast<Uint8>(round(r * 255)),
        static_cast<Uint8>(round(g * 255)),
        static_cast<Uint8>(round(b * 255))
    };
}

// Helper function for HSL to RGB conversion
float LTexture::hueToRgb(float p, float q, float t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.0 / 6.0) return p + (q - p) * 6 * t;
    if (t < 1.0 / 2.0) return q;
    if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6;
    return p;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( SDL_Renderer* gRenderer, int x, int y, float scale)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, (int)(mWidth * scale), (int)(mHeight * scale)};
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}