#pragma once
#include <GL/glew.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint Width, Height; // width and height of loaded image in pixels
						  // Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
						 // Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
					   // Constructor (sets default texture modes)

	Texture2D()
	{
	}

	Texture2D(uint32_t id) :ID(id), Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST)
	{
	}


	Texture2D(const Texture2D& copy) = delete;

	~Texture2D()
	{
		std::cout << "texture deconstructor called" << this->ID << std::endl;
	}

	//Texture2D & Texture2D::operator=(Texture2D & texture)
	//{
	//	texture = *this;
	//	return texture;
	//}

	static uint32_t Create()
	{
		uint32_t id;
		glGenTextures(1, &id);
		return id;
	}

	// Generates texture from image data
	Texture2D& Generate(GLuint width, GLuint height, unsigned char* data)
	{
		this->Width = width;
		this->Height = height;
		// Create Texture
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
		// Set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
		return *this;
	}
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}
};