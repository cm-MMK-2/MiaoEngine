#pragma once

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Shader
{
public:
	static uint32_t Create()
	{
		return glCreateProgram();
	}

	// state
	GLuint ID;
	// Constructor
	Shader(){}
	Shader(uint32_t id, const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr) :ID(id) {
		GLuint sVertex, sFragment, gShader;
		// Vertex Shader
		sVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sVertex, 1, &vertexSource, NULL);
		glCompileShader(sVertex);
		CheckCompileErrors(sVertex, "VERTEX");
		// Fragment Shader
		sFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sFragment, 1, &fragmentSource, NULL);
		glCompileShader(sFragment);
		CheckCompileErrors(sFragment, "FRAGMENT");
		// If geometry shader source code is given, also compile geometry shader
		if (geometrySource != nullptr)
		{
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &geometrySource, NULL);
			glCompileShader(gShader);
			CheckCompileErrors(gShader, "GEOMETRY");
		}
		// Shader Program
		glAttachShader(this->ID, sVertex);
		glAttachShader(this->ID, sFragment);
		if (geometrySource != nullptr)
			glAttachShader(this->ID, gShader);
		glLinkProgram(this->ID);
		CheckCompileErrors(this->ID, "PROGRAM");
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(sVertex);
		glDeleteShader(sFragment);
		if (geometrySource != nullptr)
			glDeleteShader(gShader);
	}
	Shader(Shader & copy) = delete;
	~Shader()
	{
		std::cout << "Shader deconstructor called" << this->ID << std::endl;
	}

	//Shader & Shader::operator=(Shader & shader)
	//{
	//	shader = *this;
	//	return shader;
	//}

	// Sets the current shader as active
	Shader& UseProgram() {
		glUseProgram(this->ID);
		return *this;
	}
	// Compiles the shader from given source code
	//Shader& Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr)
	//{
	//	
	//	return *this;
	//}

	// Utility functions
	void SetFloat(const GLchar *name, GLfloat value)
	{
		glUniform1f(glGetUniformLocation(this->ID, name), value);
	}
	void SetInteger(const GLchar *name, GLint value)
	{
		glUniform1i(glGetUniformLocation(this->ID, name), value);
	}
	void SetVector2f(const GLchar *name, GLfloat x, GLfloat y)
	{
		glUniform2f(glGetUniformLocation(this->ID, name), x, y);
	}
	void SetVector2f(const GLchar *name, const glm::vec2 &value)
	{
		glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
	}
	void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z)
	{
		glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
	}
	void SetVector3f(const GLchar *name, const glm::vec3 &value)
	{
		glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
	}
	void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
	}
	void SetVector4f(const GLchar *name, const glm::vec4 &value)
	{
		glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
	}
	void SetMatrix4(const GLchar *name, const glm::mat4 &matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

private:
	// Checks if compilation or linking failed and if so, print the error logs
	void CheckCompileErrors(GLuint object, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		}
		else
		{
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(object, 1024, NULL, infoLog);
				std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		}
	}
};