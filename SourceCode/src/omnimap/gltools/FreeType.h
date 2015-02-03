/****************************************************************************
*
*  FreeType font class.  Created from the NeHe Tutorial: Lesson 43
*  http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=43
*
****************************************************************************/
#ifndef FREE_TYPE_H
#define FREE_TYPE_H

// FreeType Headers
#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

// OpenGL Headers 
#include <windows.h>										// (The GL Headers Need It)
#include <GL/gl.h>
#include <GL/glu.h>

// Some STL Headers
#include <vector>
#include <string>

// Using The STL Exception Library Increases The
// Chances That Someone Else Using Our Code Will Correctly
// Catch Any Exceptions That We Throw.
#include <stdexcept>

// Wrap Everything In A Namespace, That Way We Can Use A Common
// Function Name Like "print" Without Worrying About
// Overlapping With Anyone Else's Code.
namespace freetype {

	// Inside Of This Namespace, Give Ourselves The Ability
	// To Write Just "vector" Instead Of "std::vector"
	using std::vector;

	// Ditto For String.
	using std::string;

	// This Holds All Of The Information Related To Any
	// FreeType Font That We Want To Create.  
	struct font_data {
		float h;											// Holds The Height Of The Font.
		GLuint * textures;									// Holds The Texture Id's 
		GLuint list_base;									// Holds The First Display List Id

		// The Init Function Will Create A Font With
		// The Height h From The File fname.
		void init(const char * fname, unsigned int h);

		// Free All The Resources Associated With The Font.
		void clean();
	};

	void print(const font_data &ft_font, float x, float y, const char *fmt, ...);

	void printText(const font_data &ft_font, float x, float y, const char *text);

}// Close The Namespace

#endif



