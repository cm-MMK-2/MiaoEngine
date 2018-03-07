#pragma once
#include <iostream>
#include <string>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM

#include <ft2build.h>
#include FT_FREETYPE_H  
#include <memory>
#include <map>

class FontLoader
{
public:
	FontLoader() {
		if (FT_Init_FreeType(&Library))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}
	}

	~FontLoader() {
		FT_Done_FreeType(Library);
	}

	FT_Face LoadFont(const char* fontPath, FT_UInt dpi = 300) {
		FT_Face face;
		if (FT_New_Face(Library, fontPath, 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return face;
	}

	void UnloadFont(FT_Face face) {
		FT_Done_Face(face);
	}

private:
	FT_Library Library;
};