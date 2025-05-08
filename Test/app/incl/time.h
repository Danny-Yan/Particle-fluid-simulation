#include <chrono>

//Timer wrapper class
typedef std::chrono::high_resolution_clock Clock;
class LTimer
{
    public:
        //Initializes variables
        LTimer();

        void start();
        std::chrono::milliseconds getTicks();
        bool timerHasTicked(float interval);
		void stop();
        void reset();
        bool isRunning() const;

    private:
		Clock::time_point startTime;
        Clock::time_point curTime;
		bool bIsRunning = false;
};