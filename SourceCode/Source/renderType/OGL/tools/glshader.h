/*****************************************************
*	GLShader - Jason Lindquist, February 2005
*
*	Define various objects used to wrap OpenGL calls
*	to create vertex and fragment shaders
*
*	NOTE: This file includes glew.h, which must be
*	included before gl.h
******************************************************/

#ifndef __GLSHADER_H
#define __GLSHADER_H

#include <GL/glew.h>
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <fstream>
#include <iostream>
#include "OmniMapBase.h"



class GLProgram;

class GLShader;
class GLVertexShader; // extends GLShader
class GLFragmentShader; // extends GLShader
class GLAttributeVar;
class GLUniformVar;

bool checkGLError();

/*********** Example code ************

main()
{
	... setup glut ...

	glewInit();
	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	{
		printf("OpenGL Shaders Supported");
		initShaders();
	}
	else 
	{
		printf("OpenGL Shaders Not Supported\n");
	}

	glutMainLoop();
}

initShaders()
{	
	GLVertexShader vertexShader = new GLVertexShader("../vertex.vert");
	GLFragmentShader fragmentShader = new GLFragmentShader("../fragment.frag");

	if(vertexShader->isCompiled())
	{
		vertexShader->printSource();
	}
	else
	{
		vertexShader->printLog();
	}

	if(fragmentShader->isCompiled())
	{
		fragmentShader->printSource();
	}
	else
	{
		fragmentShader->printLog();
	}


	program = new GLProgram();
	program->addShader(vertexShader);
	program->addShader(fragmentShader);

	GLAttributeVar* attribVar1 = program->getAttributeVar("attribVar1");
	GLAttributeVar* attribVar2 = program->getAttributeVar("attribVar2");
	...

	program->use();

	GLUniformVar* uniVar1 = program->getUniformVar("uniVar1");
	GLUniformVar* uniVar2 = program->getUniformVar("uniVar2");
	...

	if(attribVar1 == NULL) exit(0);
	if(attribVar2 == NULL) exit(0);
	...

	if(uniVar1 == NULL) exit(0);
	if(uniVar2 == NULL) exit(0);
	...

	program->flagProgramDeletion();
}

display()
{
	...

	uniVar1->set(...);
	uniVar2->set(...);
	
	glBegin();
	
	for each vertex
		attribVar1->set(...);
		attribVar2->set(...);
		glVertex(...);
	end for

	glEnd();

	...
}

**************************************/


/*********************************************************
*	GLShader - generic GLSL shader implementation
*	
*	Creates and compiles GLSL shader
*
*	Note: Use the subclasses GLVertexShader and
*		GLFragmentShader instead of this class
*		to make cleaner code and to abstract away 
*		from the OpenGL extensions
*********************************************************/
#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API
#else
class
#endif
GLShader
{
public:
	/********************* Constructors *************************
	*
	*	GLShader::GLShader(char* filename, int shaderType)
	*		filename - File that contains the source code for
	*			the shader
	*		shaderType - Either GL_VERTEX_SHADER_ARB for
	*			vertex shaders or GL_FRAGMENT_SHADER_ARB
	*			for fragment shaders
	*		
	*	Creates and compiles a GLSL shader of type shaderType
	*	from the file filename
	*
	*
	*	GLShader::GLShader(GLhandleARB shaderHandle)
	*		shaderHandle - A predefined, precompiled handle
	*			for a shader
	*
	*	Used by GLShader::getAttachedShaders(..), simply copies
	*		shaderHandle to _shaderHandle
	************************************************************/
	GLShader(const char *filename, const char *prelude, int shaderType);
	GLShader(const char *filename, int shaderType);
	GLShader(GLhandleARB shaderHandle);
	
	/***********************************************************
	*	bool GLShader::isCompiled()
	*
	*	Returns whether or not the shader was 
	*	compiled successfully
	************************************************************/
	bool isCompiled();
	
	/***********************************************************
	*	void GLShader::printLog()
	*
	*	Prints log file for the shader,
	*	use after a filed compile
	************************************************************/
	void printLog();
	std::string GetLog();
	
	/***********************************************************
	*	void GLShader::printSource()
	*
	*	Prints source file for the shader
	************************************************************/
	void printSource();
	std::string GetSource();
	/***********************************************************
	*	void GLShader::flagShaderDeletion()
	*
	*	Flags that this shader can be deleted.
	*	Note that the shader will most likely be deleted if it
	*	has not been attaced to a program yet, but any shader
	*	that has been attached will waint to be detached before
	*	being deleted
	***********************************************************/
	void flagShaderDeletion();

	/**********************************************************
	*	GLhandleARB GLShader::getShaderHandle()
	*
	*	Returns the shader handle for this shader
	***********************************************************/
	GLhandleARB getShaderHandle(){ return _shaderHandle; }

	/***********************************************************
	*	static GLShader** GLShader::getAttachedShaders(
	*						GLProgram* program,
	*						int &outCount)
	*		program - The program that has the shaders attached
	*		outCount - return variable: contains the number of
	*			shaders returned
	*
	*		returns - An array of GLShader*s
	*
	*	Use this static method to query a program for all of it's
	*	attached shaders
	*************************************************************/
	static GLShader** getAttachedShaders(GLProgram* program, int &outCount);
protected:
	char *readFile(const char* filename);
	GLhandleARB _shaderHandle;
};



/*********************************************************
*	GLVertexShader - GLSL vertex shader implementation
*	
*	Creates and compiles GLSL vertex shader
*
*	Extends GLShader
*********************************************************/
#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API
#else
class
#endif
GLVertexShader : public GLShader
{
public:
	/********************* Constructors *************************
	*
	*	GLVertexShader::GLVertexShader(char* filename)
	*		filename - File that contains the source code for
	*			the shader
	*		
	*	Creates and compiles a GLSL vertex shader from the 
	*		file filename
	************************************************************/
	GLVertexShader(const char *filename)					  : GLShader(filename, GL_VERTEX_SHADER_ARB){}
	GLVertexShader(const char *filename, const char *prelude) : GLShader(filename, prelude, GL_VERTEX_SHADER_ARB){}
};



/*********************************************************
*	GLFragmentShader - GLSL fragment shader implementation
*	
*	Creates and compiles GLSL fragment shader
*
*	Extends GLShader
*********************************************************/
#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API
#else
class
#endif 
GLFragmentShader : public GLShader
{
public:
	/********************* Constructors *************************
	*	GLFragmentShader::GLFragmentShader(char* filename)
	*		filename - File that contains the source code for
	*			the shader
	*		
	*	Creates and compiles a GLSL fragment shader from the 
	*		file filename
	************************************************************/
	GLFragmentShader(const char *filename) : GLShader(filename, GL_FRAGMENT_SHADER_ARB){}
	GLFragmentShader(const char *filename, const char *prelude) : GLShader(filename, prelude, GL_FRAGMENT_SHADER_ARB){}
};


/************************* GLProgram *******************************
*	GLProgram - GLProgram takes the shaders and links them together
*		to put the shaders in use
*
*	GLProgram also is what you use to get attribute and uniform
*		variables for your shaders
*
*********************************************************************/
#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API
#else
class
#endif
GLProgram
{
public:
	/********************* Constructors *************************
	*	GLProgram::GLProgram()
	*	
	*	Creates an empty program to put shaders into
	*
	*	GLProgram::GLProgram(GLhandleARB programHandle)
	*		programHandle - a pre-existing handle to a GLProgram
	*			used by getCurrentProgram
	*
	*	Creates a GLProgram from a pre-existing program handle
	*************************************************************/
	GLProgram();
	GLProgram(GLhandleARB programHandle);


	/***********************************************************
	*	void GLProgram::addShader(GLShader* shader)
	*		shader - attaches a GLShader for use in the program
	*
	*	Use this method to add the GLShaders that your program
	*	After adding all shaders that you will use, call use()
	************************************************************/
	void addShader(GLShader* shader);

	/***********************************************************
	*	void GLProgram::removeShader(GLShader* shader)
	*		shader - dettaches a GLShader from the program
	*
	*	Use this method to remove the GLShaders from your program
	*	After removing all shaders that you will no 
	*	longer use, call use()
	************************************************************/
	void removeShader(GLShader* shader);
	
	
	/***********************************************************
	*	void GLProgram::printLog()
	*
	*	Prints log file for the program
	************************************************************/
	void printLog();
	std::string GetLog();

	
	/***********************************************************
	*	void GLProgram::use()
	*	
	*	After adding or removing shaders from the program you must
	*	call this method to relink and set the program for use	
	************************************************************/
	void use();

	void link();
	void enable();

	/******************************************************************
	*	GLAttributeVar* GLProgram::getAttributeVar(char* varName)
	*		varName - name of the attribute variable in the shader(s)
	*
	*	All attribute variables in your shaders can be accessed through
	*	this method and set during the display loop
	*
	*	Note: call this method _before_ calling use()
	*******************************************************************/
	GLAttributeVar* getAttributeVar(const char* varName);

	/******************************************************************
	*	GLUniformVar* GLProgram::getUniformVar(char* varName)
	*		varName - name of the uniform variable in the shader(s)
	*
	*	All uniform variables in your shaders can be accessed through
	*	this method and set during the display loop
	*
	*	Note: call this method _after_ calling use()
	*******************************************************************/
	GLUniformVar* getUniformVar(const char* varName);

	/***********************************************************
	*	void GLProgram::flagShaderDeletion()
	*
	*	Flags that this program can be deleted
	***********************************************************/
	void flagProgramDeletion();

	/**********************************************************
	*	GLhandleARB GLProgram::getProgramHandle()
	*
	*	Returns the program handle for this shader
	***********************************************************/
	GLhandleARB getProgramHandle(){ return _programHandle; }

	/**********************************************************
	*	static GLProgram* GLProgram::getCurrentProgram()
	*
	*	Returns the currently running GLProgram
	**********************************************************/
	static GLProgram* getCurrentProgram();

	/**********************************************************
	*	static void GLProgram::useFixedPipeline()
	*
	*	Forgets about all user defined vertex and 
	*	fragment shaders and reverts to OpenGL fixed pipeline
	**********************************************************/
	static void GLProgram::useFixedPipeline() {glUseProgramObjectARB(0);}

private:
	GLhandleARB _programHandle;
	unsigned int attributeIndex;
};



/************************* GLAttributeVar *******************************
*	GLAttributeVar - GLAttributeVar is an object oriented way of
*		setting the attribute variables for a GLSL shader
*
*	You should never have to create your own GLAttributeVar object
*	Instead, get the object through the GLProgram
*
*	Methods:
*	set() - sets the variable with specified type.
*		Takes 1, 2, 3 or 4 arguments, if less than 4 arguments
*		are given, then the 2nd and 3rd arguments are assumed to be 0
*		and the 4th argument is assumed to be 1
*		Can take types GLshort, GLfloat or GLdouble
*
*	set*v() - same as set(), but initialised with and array
*		* can be 1, 2, 3, or 4.
*		When * is 1, 2, or 3, then it takes the types GLshort, GLfloat 
*			or GLdouble
*		When * is 4, then it takes the types GLbyte, GLshort, 
*			GLint, GLfloat, GLdouble, GLubyte, GLushort, or GLuint
*
*	set4N() - sets the variable by taking 4 GLubytes and normalising them
*
*	set4Nv() - sets the variable by taking a pointer to 4 elements 
*		and normalises them. Takes the types GLbyte, GLshort, 
*			GLint, GLfloat, GLdouble, GLubyte, GLushort, or GLuint'
*
*	setPointer() - 
*	setPointerN() -
*	enableArray() - 
*	disableArray() - 
*********************************************************************/
#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API
#else
class
#endif 
GLAttributeVar
{
public:
	GLAttributeVar(unsigned int index);

	// glVertexAttrib{1|2|3|4}{s|f|d}ARB(index, var);
	void set(GLshort x);
	void set(GLshort x, GLshort y);
	void set(GLshort x, GLshort y, GLshort z);
	void set(GLshort x, GLshort y, GLshort z, GLshort w);
	void set(GLfloat x);
	void set(GLfloat x, GLfloat y);
	void set(GLfloat x, GLfloat y, GLfloat z);
	void set(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void set(GLdouble x);
	void set(GLdouble x, GLdouble y);
	void set(GLdouble x, GLdouble y, GLdouble z);
	void set(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

	// glVertexAttrib{1|2|3}{s|f|d}vARB(index, var);
	void set1v(const GLshort *v);
	void set1v(const GLfloat *v);
	void set1v(const GLdouble *v);
	
	void set2v(const GLshort *v);
	void set2v(const GLfloat *v);
	void set2v(const GLdouble *v);
	
	void set3v(const GLshort *v);
	void set3v(const GLfloat *v);
	void set3v(const GLdouble *v);

	// glVertexAttrib4{b|s|i|f|d|ub|us|ui}vARB(index, var);
	void set4v(const GLbyte *v);
	void set4v(const GLshort *v);
	void set4v(const GLint *v);
	void set4v(const GLfloat *v);
	void set4v(const GLdouble *v);
	void set4v(const GLubyte *v);
	void set4v(const GLushort *v);
	void set4v(const GLuint *v);

	// glVertexAttrib4NubARB(index, x, y, z, w);
	void set4N(GLubyte x, GLubyte y, GLubyte z, GLubyte w);

	// glVertexAttrib4N{b|s|i|ub|us|ui}vARB(index var);
	void set4Nv(GLbyte *v);
	void set4Nv(GLshort*v);
	void set4Nv(GLint *v);
	void set4Nv(GLubyte *v);
	void set4Nv(GLushort *v);
	void set4Nv(GLuint *v);

	// glVertexAttribPointerARB(index, size, type, normalized, stride, pointer)
	void setPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer); // normalized = false
	void setPointerN(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer); // normalized = true

	void enableArray();
	void disableArray();

	int getIndex() { return _index; }
private:
	GLuint _index;
};


#ifdef ELUMENATI_INTERNAL
class OMNIMAP_API
#else
class
#endif 
GLUniformVar
{
public:
	GLUniformVar(int location);

	// glUniform{1|2|3|4|}{f|i}ARB(location, v);
	void set(GLfloat x);
	void set(GLfloat x, GLfloat y);
	void set(GLfloat x, GLfloat y, GLfloat z);
	void set(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void set(GLint x);
	void set(GLint x, GLint y);
	void set(GLint x, GLint y, GLint z);
	void set(GLint x, GLint y, GLint z, GLint w);

	// glUniform{1|2|3|4}{f|i}vARB(location, count, v);
	void set1v(GLuint count, const GLfloat* v);
	void set1v(GLuint count, const GLint* v);
	void set2v(GLuint count, const GLfloat* v);
	void set2v(GLuint count, const GLint* v);
	void set3v(GLuint count, const GLfloat* v);
	void set3v(GLuint count, const GLint* v);
	void set4v(GLuint count, const GLfloat* v);
	void set4v(GLuint count, const GLint* v);

	// glUniformMatrix{2|3|4}fvARB(location, count, transpose, v);
	void setMatrix2(GLuint count, GLboolean transpose, const GLfloat* v);
	void setMatrix3(GLuint count, GLboolean transpose, const GLfloat* v);
	void setMatrix4(GLuint count, GLboolean transpose, const GLfloat* v);

	int getLocation() { return _location; }
private:
	GLint _location;
};

#endif