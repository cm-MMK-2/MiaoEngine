#pragma once
#include <map>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

#include <SOIL.h>

#include "Data/texture.hpp"
#include "Data/shader.hpp"
#include "Data/SpineData.hpp"

#include "FontLoader.hpp"

class MResourceLoader
{
public:
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader& LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		try
		{
			// Open files
			std::ifstream vertexShaderFile(vShaderFile);
			std::ifstream fragmentShaderFile(fShaderFile);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();
			// close file handlers
			vertexShaderFile.close();
			fragmentShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// If geometry shader path is present, also load a geometry shader
			if (gShaderFile != nullptr)
			{
				std::ifstream geometryShaderFile(gShaderFile);
				std::stringstream gShaderStream;
				gShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::exception e)
		{
			std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
		}
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		const GLchar *gShaderCode = geometryCode.c_str();
		// 2. Now create shader object from source code
		uint32_t shaderId = Shader::Create();
		shaders[shaderId] = std::make_unique<Shader>(shaderId, vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
		return *shaders[shaderId];
	}

	// Retrieves a stored sader
	static Shader& GetShader(uint32_t id)
	{
		return *shaders[id];
	}

	// Loads (and generates) a texture from file
	static Texture2D& LoadTexture(const char *file, bool alpha = true)
	{
		uint32_t textureId = Texture2D::Create();		
		// Create Texture object
		textures[textureId] = std::make_unique<Texture2D>(textureId);
		Texture2D& texture = *textures[textureId];
		if (alpha)
		{
			texture.Internal_Format = GL_RGBA;
			texture.Image_Format = GL_RGBA;
		}
		// Load image
		int width, height;
		unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		// Now generate texture
		texture.Generate(width, height, image);
		// And finally free image data
		SOIL_free_image_data(image);
		return texture;
	}

	// Retrieves a stored texture
	static Texture2D& GetTexture(uint32_t id)
	{
		return *textures[id];
	}

	//static SpineData& LoadSpineData(const char* atlasPath, const char* dataPath, SpineDataType dataType = SpineDataType::Binary)
	//{
	//	SpineData(atlasPath, dataPath, dataType)
	//	return;
	//}

	// Properly de-allocates all loaded resources
	static void UnloadAll()
	{
		// (Properly) delete all shaders	
		for (const auto &iter : shaders)
			glDeleteProgram(iter.first);
		// (Properly) delete all textures
		for (const auto &iter : textures)
			glDeleteTextures(1, &iter.first);
		shaders.clear();
		textures.clear();
	}

	static void UnloadShader(Shader& shader)
	{
		glDeleteProgram(shader.ID);
		shaders.erase(shader.ID);
	}

	static void UnloadShader(uint32_t id)
	{
		glDeleteProgram(id);
		shaders.erase(id);
	}

	static void UnloadTexture(Texture2D& texture)
	{
		textures.erase(texture.ID);
		glDeleteTextures(1, &texture.ID);
	}

	static void UnloadTexture(uint32_t id)
	{
		glDeleteTextures(1, &id);
		textures.erase(id);
	}

	static FT_Face LoadFont(const char* fontPath);

	static void UnloadFont(FT_Face face);


	static SpineData & LoadSpineData(const char *name, const char* atlasPath, const char* dataPath, SpineDataType dataType = SpineDataType::Binary)
	{
		spines[name] = std::make_unique<SpineData>(atlasPath, dataPath, dataType);
		return *spines[name];
	}

	static void UnloadSpineData(const char *name)
	{
		spines.erase(name);
	}

private:
	static FontLoader fontLoader;

	// ResourceLoader storage
	static std::map<uint32_t, std::unique_ptr<Shader>> shaders;
	static std::map<uint32_t, std::unique_ptr<Texture2D>> textures;
	static std::map<const char*, std::unique_ptr<SpineData>> spines;
};

FontLoader MResourceLoader::fontLoader;
std::map<uint32_t, std::unique_ptr<Shader>> MResourceLoader::shaders;
std::map<uint32_t, std::unique_ptr<Texture2D>> MResourceLoader::textures;
std::map<const char*, std::unique_ptr<SpineData>> MResourceLoader::spines;

FT_Face MResourceLoader::LoadFont(const char* fontPath)
{
	return fontLoader.LoadFont(fontPath);
}

void MResourceLoader::UnloadFont(FT_Face face)
{
	fontLoader.UnloadFont(face);
}