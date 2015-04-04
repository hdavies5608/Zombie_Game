#include "IOManager.h"
#include <fstream>
namespace IceEngine
{
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}
		buffer.clear();
		//Place cursor at end of file...
		file.seekg(0, std::ios::end);
		//...Get file size by finding the 'distance' from 0 bytes to EOF
		int fileSize = (int)file.tellg();
		//Place cursor at begining of file for firther operation
		file.seekg(0, std::ios::beg);
		//Remove any bytes that may be in the file header and not important to the file's contents
		fileSize -= (int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&buffer[0], fileSize);

		file.close();

		return true;
	}
}
