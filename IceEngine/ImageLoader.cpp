#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"
namespace IceEngine
{
	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		GLTexture texture = { 0 };

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;
		unsigned long width, height;

		if (!IOManager::readFileToBuffer(filePath, in))
		{
			FATAL_ERROR("Could not load " + filePath + " Image");
		}

		int errorCode = decodePNG(out, width, height, &in[0], in.size());
		if (errorCode != 0)
		{
			FATAL_ERROR("Decode PNG failed with error: " + std::to_string(errorCode) + " On PNG at " + filePath);
		}

		glGenTextures(1, &(texture.ID));

		glBindTexture(GL_TEXTURE_2D, texture.ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//We want to use linear filtering for magnification and minification.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//Use mipmapping so the textured don't look bad at differing distances.
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;
		return texture;
	}
}