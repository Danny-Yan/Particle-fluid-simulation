#ifndef LTEXTURE_H
#define LTEXTURE_H


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <array>

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( SDL_Renderer* gRenderer, std::string path );

        void setColorForSpeed(int speed);
        void setColorForSpeedHSL(float speed);

        //Set color modulation
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        //Set color modulation
        void setColorHSL(float h, float s, float l);

        // //Set blending
        // void setBlendMode( SDL_BlendMode blending );

        // //Set alpha modulation
        // void setAlpha( Uint8 alpha );

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render( SDL_Renderer* gRenderer, int x, int y, float scale);

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;

		// Color Linearisation
        Uint8 colourLinearisationRGB(int speed, int color_max_speed, int color_min, int color_max);
        std::array<Uint8, 3> colourLinearisationHSL(float speed, float color_max_speed, float hMin, float hMax);

		// Color Processors from speed
        std::array<Uint8, 3> colourProcessorRGBtoRGB(int speed);

		// Convert hsl to rgb
        std::array<Uint8, 3> hslToRgb(float h, float s, float l);
        float hueToRgb(float p, float q, float t);
};

#endif