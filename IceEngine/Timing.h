#pragma once
namespace IceEngine
{
	class FPSLimiter
	{
	public:
		FPSLimiter();

		void init(float targetFPS);
		void setTargetFPS(float targetFPS);
		void beginFrame();
	
		float endFrame();
	private:
		void calculateFPS();
		float _targetFPS;

		unsigned int _startTicks;
		float _fps;
		float _frameTime;
	};
}