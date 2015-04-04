#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>
//We want to use an initialization list and set all our variables to 0,
//We use the list as its slightly faster than in the body of the constructor
//and set them to 0 so we can check they have been reassigned later
namespace IceEngine
{
	GLSLProgram::GLSLProgram() : 
	_numAttributes(0),
	_programID(0),
	_vertexShaderID(0),
	_fragmentShaderID(0)
	{
	}

	//We don't need the deconstructor
	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		//We need to tell OpenGl to create a shader program and the set our
		//_programID to it
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
		{
			FATAL_ERROR("Vertex shader failed to be created");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
		{
			FATAL_ERROR("Fragment shader failed to be created");
		}

		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}
	void GLSLProgram::linkShaders()
	{

		//Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			//print the error log and quit
			std::printf("%s\n", &(errorLog[0]));
			FATAL_ERROR("Shaders failed to link!");
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	//Used to add attributes to our shaders such as textures
	void GLSLProgram::addAttribute(const std::string attributeName)
	{
		//We add one to numAtributes so it is one larger for the next time
		//This is why postfix ++ is used 
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	//If we need to find the location of a uniform in our shaders we can use this
	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			FATAL_ERROR("Uniform " + uniformName + " Not found in shader");
		}
		return location;
	}

	//Tells OpenGL to use our shaders
	void GLSLProgram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}
	//Tells OpenGL to stop using our shaders
	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}
	void GLSLProgram::compileShader(const std::string& filePath, GLuint ID)
	{
		std::ifstream shaderFile(filePath);
		if (shaderFile.fail())
		{
			FATAL_ERROR("Failed to open: " + filePath);
		}
		std::string fileContents = "";
		std::string line;
		while (std::getline(shaderFile, line))
		{
			fileContents += line + "\n";
		}
		shaderFile.close();

		const char* contentsPointer = fileContents.c_str();

		glShaderSource(ID, 1, &contentsPointer, nullptr);
		glCompileShader(ID);

		GLint success = 0;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(ID, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(ID);

			std::printf("%s\n", &errorLog[0]);
			FATAL_ERROR("Shader " + filePath + " failed to compile");
		}
	}
}