#pragma once
#include <GL/glew.h>
#include <string>
#include "GLTexture.h"

namespace IceEngine
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();
	private:
		float _x;
		float _y;
		float _height;
		float _width;
		GLuint _vboID;
		GLTexture _texture;
	};
}