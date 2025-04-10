#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>

//Timer wrapper class
class LTimer
{
    public:
        //Initializes variables
        LTimer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();
        void setTicked();
        void resetTicked();

        //Gets the timer's time
        Uint64 getTicks();

        //Checks the status of the timer
        bool isStarted();
        bool isPaused();
        bool getTicked();

    private:
        //The clock time when the timer started
        Uint64 mStartTicks;

        //The ticks stored when the timer was paused
        Uint64 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
        bool hasTicked;
};