#pragma once
#include <string>
#include "TextureCache.h"

namespace IceEngine
{
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);
	
	private:
		static TextureCache _textureCache;
	};
}
