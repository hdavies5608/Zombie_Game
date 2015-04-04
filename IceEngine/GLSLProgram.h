#pragma once
#include <string>
#include <GL\glew.h>

namespace IceEngine
{
	//This class will load all of our shaders and help to 
	//abstract the loading, compiling and linking of them.
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath,const std::string& fragmentShaderFilePath);
		void linkShaders();
		void addAttribute(const std::string attributeName);
		void use();
		void unuse();
		GLint getUniformLocation(const std::string& uniformName);
	private:
		void compileShader(const std::string& filePath, GLuint ID);

		int _numAttributes;

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}