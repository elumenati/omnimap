#include "GLShader.h"
#include "../omnimap.h"
#include <string>

bool checkGLError() {
	int ret = glGetError();
	if (ret!=0L) {
		if (ret == GL_INVALID_ENUM) {
			LogSystem()->ReportError("GL_INVALID_ENUM ");
		}
		else if (ret == GL_INVALID_VALUE) {
			LogSystem()->ReportError("GL_INVALID_VALUE ");
		}
		else if (ret == GL_INVALID_OPERATION) {
			LogSystem()->ReportError("GL_INVALID_OPERATION ");
		}
		else if (ret == GL_STACK_OVERFLOW) {
			LogSystem()->ReportError("GL_STACK_OVERFLOW ");
		}
		else if (ret == GL_STACK_UNDERFLOW) {
			LogSystem()->ReportError("GL_STACK_UNDERFLOW ");
		}
		else if (ret == GL_OUT_OF_MEMORY) {
			LogSystem()->ReportError("GL_OUT_OF_MEMORY ");
		}
		else LogSystem()->ReportError("GL OTHER PROBLEM???");

		return false;
	}
	return true;
}

//------- GLShader ------------//
GLShader::GLShader(const char* filename, int shaderType)
{
	using namespace std;

	const char* file = readFile(filename);
	if(file == NULL)
	{
		LogSystem()->ReportError("Error reading file : %s",filename );
		return;
	}

	_shaderHandle = glCreateShaderObjectARB(shaderType);
	if(!checkGLError())
			LogSystem()->ReportError("Error in GLShader::GLShader");

	glShaderSourceARB(_shaderHandle, 1, &file, NULL);
		if(!checkGLError())
			LogSystem()->ReportError("Error in GLShader::GLShader");

	delete[] file;

	glCompileShaderARB(_shaderHandle);
	if(!checkGLError())
			LogSystem()->ReportError("Error in GLShader::GLShader");
}


GLShader::GLShader(GLhandleARB shaderHandle)
{
	_shaderHandle = shaderHandle;
}


GLShader**
GLShader::getAttachedShaders(GLProgram* program, int &outCount)
{
	using namespace std;
	int count = 0;
	glGetObjectParameterivARB(program->getProgramHandle(), GL_OBJECT_ATTACHED_OBJECTS_ARB, &count);
	if(!checkGLError())
			LogSystem()->ReportError("Error in GLShader::getAttachedShaders");

	if(count <= 0)
	{
		outCount = 0;
		return NULL;
	}
	
	GLhandleARB *shaderHandles = new GLhandleARB[count];
	int maxCount = count;
	
	glGetAttachedObjectsARB(program->getProgramHandle(), maxCount, &count, shaderHandles);
	if(!checkGLError())
			LogSystem()->ReportError("Error in GLShader::getAttachedShaders");

	if(count <= 0)
	{
		outCount = 0;
		return NULL;
	}

	outCount = count;
	GLShader** shaders = new GLShader*[count];
	for(int i = 0; i < count; i++)
	{
		shaders[i] = new GLShader(shaderHandles[i]);
	}

	return shaders;
}

bool
GLShader::isCompiled()
{ 
	using namespace std;
	int compiled = 0; 
	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::isCompiled");

	
	return compiled != 0;
}

void
GLShader::printLog()
{
	using namespace std;

	int length = 0;
	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::printLog");

	char *log = new char[length];
	glGetInfoLogARB(_shaderHandle, length, NULL, log);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::printLog");

	cout << "---------- Begin: GL Shader Log ----------" << endl;
	cout << log << endl;
	cout << "----------- End: GL Shader Log -----------" << endl << endl;

	delete[] log;
}


std::string GLShader::GetLog()
{
	using namespace std;

	int length = 0;
	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::getlog");

	char *log = new char[length];
	glGetInfoLogARB(_shaderHandle, length, NULL, log);
	


	std::string myshaderlog;
	if(!checkGLError())
		myshaderlog = "Error in GLShader::printLog";
	else
		myshaderlog = "Shader Compiled Sucessfully";
	myshaderlog = myshaderlog + std::string("\nLOG OF COMPILATION:\n") + std::string(log );

	myshaderlog += GetSource();
	delete[] log;



	return 	myshaderlog;
}
void 
GLShader::printSource()
{
	using namespace std;

	int length = 0;
	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB, &length);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::printSource");
	char *source = new char[length];
	glGetShaderSourceARB(_shaderHandle, length, NULL, source);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::printSource");
	cout << "---------- Begin: GL Shader Source ----------" << endl;
	cout << source << endl;
	cout << "----------- End: GL Shader Source -----------" << endl << endl;

	delete[] source;
}


std::string
GLShader::GetSource()
{
		using namespace std;

	int length = 0;
	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB, &length);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::printSource");
	char *source = new char[length];
	glGetShaderSourceARB(_shaderHandle,length , &length, source);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::printSource");

	std::string sourcecode = "";
	sourcecode += "---------- Begin: GL Shader Source ----------\n" ;
	sourcecode +=std::string( source);
	
	
	sourcecode += "----------- End: GL Shader Source -----------\n" ;

	delete[] source;
	return sourcecode;
}


void
GLShader::flagShaderDeletion()
{
	using namespace std;
	glDeleteObjectARB(_shaderHandle);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLShader::flagShaderDeletion");
}

char*
GLShader::readFile(const char* filename)
{	
	using namespace std;
	ifstream file;
	file.open(filename);
	
	// open file
	if(!file.is_open())
	{
		LogSystem()->ReportError("Error opening file: %s",filename);
		return NULL;
	}



	// read file into a string
	string contents = "";
	char *intermediate = new char[1025];
	while(!file.eof())
	{
		for(int i = 0; i < 1025; i++)
		{
			intermediate[i] = '\0';
		}

		file.read(intermediate, 1024);
		contents += intermediate;
	}

	// close file
	file.close();
	delete file;
	delete[] intermediate;
	
	// copy string to char*
/*	char *out = new char[contents.length()+1];
	contents.copy(out, contents.length());
	out[contents.length()] = '\0';
*/
	char *out = new char[contents.length()+1];
#if defined(USE_VC7)
    strcpy(out, contents.c_str());
#else
	contents._Copy_s(out, contents.length(), contents.length(),0);// fix depereciated version!
#endif
	out[contents.length()] = '\0';


	return out;
}



//--------- GLProgram -----------//
GLProgram::GLProgram()
{
	using namespace std;
	attributeIndex = 1;
	_programHandle = glCreateProgramObjectARB();
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::GLProgram");

}

GLProgram::GLProgram(GLhandleARB programHandle)
{
	attributeIndex = 1;
	_programHandle = programHandle;
}

void
GLProgram::printLog()
{
	using namespace std;

	int length = 0;
	glGetObjectParameterivARB(_programHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::printLog");

	char *log = new char[length];
	glGetInfoLogARB(_programHandle, length, NULL, log);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::printLog");

	cout << "---------- Begin: GL Program Log ----------" << endl;
	cout << log << endl;
	cout << "----------- End: GL Program Log -----------" << endl << endl;

	delete[] log;
}

std::string GLProgram::GetLog()
{
	using namespace std;

	int length = 0;
	glGetObjectParameterivARB(_programHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::getlog");

	char *log = new char[length];
	glGetInfoLogARB(_programHandle, length, NULL, log);

	std::string myshaderlog;
	if(!checkGLError())
		myshaderlog = "Error in GLProgram::printLog";
	else
		myshaderlog = "Shader Compiled Sucessfully";
	myshaderlog += log;
	delete[] log;

return myshaderlog;
}








GLProgram*
GLProgram::getCurrentProgram()
{
	return new GLProgram(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
}

void
GLProgram::addShader(GLShader* shader)
{
	using namespace std;
	glAttachObjectARB(_programHandle, shader->getShaderHandle());
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::addShader");
}

void
GLProgram::removeShader(GLShader* shader)
{
	using namespace std;
	glDetachObjectARB(_programHandle, shader->getShaderHandle());
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::removeShader");
}

void
GLProgram::use()
{
	using namespace std;

	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::use   (ErrorIntrumentionA)");

	glLinkProgramARB(_programHandle);

	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::use   (ErrorIntrumentionB)");

	glUseProgramObjectARB(_programHandle);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::use   (ErrorIntrumentionC)");
}

void
GLProgram::link()
{
	using namespace std;
	glLinkProgramARB(_programHandle);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::link");

}

void
GLProgram::enable()
{
	using namespace std;
	glUseProgramObjectARB(_programHandle);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::enable");
}


void GLProgram::flagProgramDeletion()
{
	using namespace std;
	glDeleteObjectARB(_programHandle);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::flagProgramDeletion");

}

GLAttributeVar* GLProgram::getAttributeVar(const char* varName)
{
	using namespace std;
	int index = attributeIndex;
	attributeIndex++;

	glBindAttribLocationARB(_programHandle, index, varName);
	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::getAttributeVar(\"%s\")",varName);

	return new GLAttributeVar(index);
}

GLUniformVar* GLProgram::getUniformVar(const char* varName)
{
	using namespace std;
 

	int location = glGetUniformLocationARB(_programHandle, varName);

	if(!checkGLError())
		LogSystem()->ReportError("Error in GLProgram::getUniformVar(\"%s\")",varName);

	if(location == -1)
	{
		LogSystem()->ReportError("Invalid Uniform Variable : %s",varName);

		return NULL;
	}
	return new GLUniformVar(location);
}



//-------- GLAttributeVar --------//
GLAttributeVar::GLAttributeVar(unsigned int index)
{
	_index = index;
}

// glVertexAttrib{1|2|3|4}{s|f|d}ARB(index, var);
void GLAttributeVar::set(GLshort x) 
{ glVertexAttrib1sARB(_index, x); }
void GLAttributeVar::set(GLshort x, GLshort y)
{ glVertexAttrib2sARB(_index, x, y); }
void GLAttributeVar::set(GLshort x, GLshort y, short z)
{ glVertexAttrib3sARB(_index, x, y, z); }
void GLAttributeVar::set(GLshort x, GLshort y, GLshort z, GLshort w)
{ glVertexAttrib4sARB(_index, x, y, z, w); }

void GLAttributeVar::set(GLfloat x)
{ glVertexAttrib1fARB(_index, x); }
void GLAttributeVar::set(GLfloat x, GLfloat y)
{ glVertexAttrib2fARB(_index, x, y); }
void GLAttributeVar::set(GLfloat x, GLfloat y, GLfloat z)
{ glVertexAttrib3fARB(_index, x, y, z); }
void GLAttributeVar::set(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{ glVertexAttrib4fARB(_index, x, y, z, w); }

void GLAttributeVar::set(GLdouble x)
{ glVertexAttrib1dARB(_index, x); }
void GLAttributeVar::set(GLdouble x, GLdouble y)
{ glVertexAttrib2dARB(_index, x, y); }
void GLAttributeVar::set(GLdouble x, GLdouble y, GLdouble z)
{ glVertexAttrib3dARB(_index, x, y, z); }
void GLAttributeVar::set(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{ glVertexAttrib4dARB(_index, x, y, z, w); }

// glVertexAttrib{1|2|3}{s|f|d}vARB(index, var);
void GLAttributeVar::set1v(const GLshort *v)
{ glVertexAttrib1svARB(_index, v); }
void GLAttributeVar::set1v(const GLfloat *v)
{ glVertexAttrib1fvARB(_index, v); }
void GLAttributeVar::set1v(const GLdouble *v)
{ glVertexAttrib1dvARB(_index, v); }

void GLAttributeVar::set2v(const GLshort *v)
{ glVertexAttrib2svARB(_index, v); }
void GLAttributeVar::set2v(const GLfloat *v)
{ glVertexAttrib2fvARB(_index, v); }
void GLAttributeVar::set2v(const GLdouble *v)
{ glVertexAttrib2dvARB(_index, v); }

void GLAttributeVar::set3v(const GLshort *v)
{ glVertexAttrib3svARB(_index, v); }
void GLAttributeVar::set3v(const GLfloat *v)
{ glVertexAttrib3fvARB(_index, v); }
void GLAttributeVar::set3v(const GLdouble *v)
{ glVertexAttrib3dvARB(_index, v); }

// glVertexAttrib4{b|s|i|f|d|ub|us|ui}vARB(index, var);
void GLAttributeVar::set4v(const GLbyte *v)
{ glVertexAttrib4bvARB(_index, v); }
void GLAttributeVar::set4v(const GLshort *v)
{ glVertexAttrib4svARB(_index, v); }
void GLAttributeVar::set4v(const GLint *v)
{ glVertexAttrib4ivARB(_index, v); }
void GLAttributeVar::set4v(const GLfloat *v)
{ glVertexAttrib4fvARB(_index, v); }
void GLAttributeVar::set4v(const GLdouble *v)
{ glVertexAttrib4dvARB(_index, v); }
void GLAttributeVar::set4v(const GLubyte *v)
{ glVertexAttrib4ubvARB(_index, v); }
void GLAttributeVar::set4v(const GLushort *v)
{ glVertexAttrib4usvARB(_index, v); }
void GLAttributeVar::set4v(const GLuint *v)
{ glVertexAttrib4uivARB(_index, v); }

// glVertexAttrib4NubARB(index, x, y, z, w);
void GLAttributeVar::set4N(GLubyte x, GLubyte y, GLubyte z, GLubyte w)
{ glVertexAttrib4NubARB(_index, x, y, z, w); }

// glVertexAttrib4N{b|s|i|ub|us|ui}vARB(index var);
void GLAttributeVar::set4Nv(GLbyte *v)
{ glVertexAttrib4NbvARB(_index, v); }
void GLAttributeVar::set4Nv(GLshort*v)
{ glVertexAttrib4NsvARB(_index, v); }
void GLAttributeVar::set4Nv(GLint *v)
{ glVertexAttrib4NivARB(_index, v); }
void GLAttributeVar::set4Nv(GLubyte *v)
{ glVertexAttrib4NubvARB(_index, v); }
void GLAttributeVar::set4Nv(GLushort *v)
{ glVertexAttrib4NusvARB(_index, v); }
void GLAttributeVar::set4Nv(GLuint *v)
{ glVertexAttrib4NuivARB(_index, v); }

// glVertexAttribPointerARB(index, size, type, normalized, stride, pointer)
void GLAttributeVar::setPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{ glVertexAttribPointerARB(_index, size, type, GL_FALSE, stride, pointer); }
void GLAttributeVar::setPointerN(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{ glVertexAttribPointerARB(_index, size, type, GL_TRUE, stride, pointer); }

void GLAttributeVar::enableArray()
{ glEnableVertexAttribArrayARB(_index); }
void GLAttributeVar::disableArray()
{ glDisableVertexAttribArrayARB(_index); }



//-------- GLUniformVar --------//
GLUniformVar::GLUniformVar(int location)
{
	_location = location;
}


// glUniform{1|2|3|4|}{f|i}ARB(location, v);
void GLUniformVar::set(GLfloat x)
{ glUniform1fARB(_location, x); }
void GLUniformVar::set(GLfloat x, GLfloat y)
{ glUniform2fARB(_location, x, y); }
void GLUniformVar::set(GLfloat x, GLfloat y, GLfloat z)
	{ glUniform3fARB(_location, x, y, z); }
void GLUniformVar::set(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{ glUniform4fARB(_location, x, y, z, w); }
void GLUniformVar::set(GLint x)
{ glUniform1iARB(_location, x); }
void GLUniformVar::set(GLint x, GLint y)
{ glUniform2iARB(_location, x, y); }
void GLUniformVar::set(GLint x, GLint y, GLint z)
 
{ glUniform3iARB(_location, x, y, z); 
}
void GLUniformVar::set(GLint x, GLint y, GLint z, GLint w)
{ glUniform4iARB(_location, x, y, z, w); }

// glUniform{1|2|3|4}{f|i}vARB(location, count, v);
void GLUniformVar::set1v(GLuint count, const GLfloat* v)
{ glUniform1fvARB(_location, count, v); }
void GLUniformVar::set1v(GLuint count, const GLint* v)
{ glUniform1ivARB(_location, count, v); }
void GLUniformVar::set2v(GLuint count, const GLfloat* v)
{ glUniform2fvARB(_location, count, v); }
void GLUniformVar::set2v(GLuint count, const GLint* v)
{ glUniform2ivARB(_location, count, v); }
void GLUniformVar::set3v(GLuint count, const GLfloat* v)
{ glUniform3fvARB(_location, count, v); }
void GLUniformVar::set3v(GLuint count, const GLint* v)
{ glUniform3ivARB(_location, count, v); }
void GLUniformVar::set4v(GLuint count, const GLfloat* v)
{ glUniform4fvARB(_location, count, v); }
void GLUniformVar::set4v(GLuint count, const GLint* v)
{ glUniform4ivARB(_location, count, v); }

void GLUniformVar::setMatrix2(GLuint count, GLboolean transpose, const GLfloat* v)
{ glUniformMatrix2fvARB(_location, count, transpose, v); }
void GLUniformVar::setMatrix3(GLuint count, GLboolean transpose, const GLfloat* v)
{ glUniformMatrix3fvARB(_location, count, transpose, v); }
void GLUniformVar::setMatrix4(GLuint count, GLboolean transpose, const GLfloat* v)
{ glUniformMatrix4fvARB(_location, count, transpose, v); }