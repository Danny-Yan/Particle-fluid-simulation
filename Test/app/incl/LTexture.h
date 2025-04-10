#ifndef LTEXTURE_H
#define LTEXTURE_H


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

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

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

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
};

#endif