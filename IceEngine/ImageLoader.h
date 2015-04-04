#pragma once
#include <string>
#include "GLTexture.h"

namespace IceEngine
{
	class ImageLoader
	{
	public:
		//We want to use a static function so we don't have to make multiple `ImageLoader` objects
		static GLTexture loadPNG(std::string filePath);
	};
}

