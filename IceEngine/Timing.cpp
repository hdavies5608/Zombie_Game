#include "Timing.h"
#include <SDL\SDL.h>

namespace IceEngine
{
	FPSLimiter::FPSLimiter()
	{}

	void FPSLimiter::init(float targetFPS)
	{
		setTargetFPS(targetFPS);
	}

	void FPSLimiter::beginFrame()
	{
		_startTicks = SDL_GetTicks();
	}

	void FPSLimiter::setTargetFPS(float targetFPS)
	{
		_targetFPS = targetFPS;
	}

	void FPSLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;
		static float prevTicks = (float)SDL_GetTicks();

		float currentTicks;
		currentTicks = (float)SDL_GetTicks();

		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		prevTicks = currentTicks;

		int count;
		currentFrame++;
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;

		for (int i = 0; i < count; i++)
		{
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
		{
			_fps = 1000.f / frameTimeAverage;
		}
		else
		{
			_fps = 60;
		}
	}


	float FPSLimiter::endFrame()
	{
		calculateFPS();

		float frameTicks = (float)SDL_GetTicks() - _startTicks;
		//Limit FPS to max FPS
		if (1000.f / _targetFPS > frameTicks)
		{
			SDL_Delay((Uint32)((1000.f / _targetFPS) - frameTicks));
		}
		return _fps;
	}
}