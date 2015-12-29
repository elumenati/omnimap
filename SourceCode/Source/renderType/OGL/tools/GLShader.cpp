
#include "GLShader.h"
#include <string>
#include "OM_ErrorHandling.h"



bool checkGLError() {
  EH_DECLARE;

  EH_Test(glGetError());
  
  EH_OnError() {
    switch(EH_ERRORCODE) {
      case GL_INVALID_ENUM:
        EH_Log("GL_INVALID_ENUM ");
        return false;
      
      case GL_INVALID_VALUE:
        EH_Log("GL_INVALID_VALUE ");
        return false;

      case GL_INVALID_OPERATION:
        EH_Log("GL_INVALID_OPERATION ");
        return false;

      case GL_STACK_OVERFLOW:
        EH_Log("GL_STACK_OVERFLOW ");
        return false;

      case GL_STACK_UNDERFLOW:
        EH_Log("GL_STACK_UNDERFLOW ");
        return false;

      case GL_OUT_OF_MEMORY:
        EH_Log("GL_OUT_OF_MEMORY ");
        return false;

      default:
        EH_Log("GL OTHER PROBLEM???");
        return false;
    }
  }

  return true;
}




//------- GLShader ------------//
GLShader::GLShader(const char *filename, const char *prelude, int shaderType)
{
	char* file = NULL;
  char *shaderSource = NULL;
  EH_DECLARE;

  if (filename) {
    EH_Ptr(file = readFile(filename), "Error reading file : %s",filename);
  }

  if (file) {
    EH_Ptr(shaderSource = new char[strlen(file) + strlen(prelude) + 1]);
    strcpy(shaderSource, prelude);
    strcat(shaderSource, file);
  } else {
    EH_Ptr(shaderSource = new char[strlen(prelude) + 1]);
    strcpy(shaderSource, prelude);
  }


	_shaderHandle = glCreateShaderObjectARB(shaderType);
  EH_Zero(checkGLError(), "Error in GLShader::GLShader");

	glShaderSourceARB(_shaderHandle, 1, (const char **) (&shaderSource), NULL);
  EH_Zero(checkGLError(), "Error in GLShader::GLShader");

	glCompileShaderARB(_shaderHandle);
  EH_Zero(checkGLError(), "Error in GLShader::GLShader");

  EH_OnError() {}

  if (file) delete[] file;
  if (shaderSource) delete[] shaderSource;
}





GLShader::GLShader(const char* filename, int shaderType)
{
	const char* file = readFile(filename);
  EH_DECLARE;

  EH_Ptr(file = readFile(filename), "Error reading file : %s",filename);

	_shaderHandle = glCreateShaderObjectARB(shaderType);
  EH_Zero(checkGLError(), "Error in GLShader::GLShader");

	glShaderSourceARB(_shaderHandle, 1, &file, NULL);
  EH_Zero(checkGLError(), "Error in GLShader::GLShader");

	glCompileShaderARB(_shaderHandle);
  EH_Zero(checkGLError(), "Error in GLShader::GLShader");

  EH_OnError() {}

  if (file) delete[] file;
}




GLShader::GLShader(GLhandleARB shaderHandle)
{
	_shaderHandle = shaderHandle;
}




GLShader** GLShader::getAttachedShaders(GLProgram* program, int &outCount)
{
	GLint count = 0;
  GLShader** shaders = NULL;
  GLhandleARB *shaderHandles = NULL;
  int maxCount = 0;
  EH_DECLARE;

	glGetObjectParameterivARB(program->getProgramHandle(), GL_OBJECT_ATTACHED_OBJECTS_ARB, &count);
  EH_Zero(checkGLError(), "Error in GLShader::getAttachedShaders");
  EH_Zero(count > 0);  // if count <= 0 then error
	
	EH_Ptr(shaderHandles = new GLhandleARB[count]);

	maxCount = count;
	
	glGetAttachedObjectsARB(program->getProgramHandle(), maxCount, &count, shaderHandles);
  EH_Zero(checkGLError(), "Error in GLShader::getAttachedShaders");
  EH_Zero(count > 0);  // if count <= 0 then error

	outCount = count;

	EH_Ptr(shaders = new GLShader*[count]);
  memset(shaders, NULL, sizeof(GLShader*) * count);

  for(int i = 0; i < count; i++)
	{
		EH_Ptr(shaders[i] = new GLShader(shaderHandles[i]));
	}


  EH_OnError() {
    for(int i = 0; i < count; i++)
    {
      if (shaders[i]) delete shaders[i];
    }
    if (shaders) delete[] shaders;
  }

  if (shaderHandles) delete[] shaderHandles;

	return shaders;
}




bool GLShader::isCompiled()
{ 
	GLint compiled = 0; 
  EH_DECLARE;
  glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  EH_Zero(checkGLError(), "Error in GLShader::isCompiled");
  EH_OnError() {}
  return compiled != 0;
}




void GLShader::printLog()
{
	using namespace std;
	GLint length = 0;
  char *log = NULL;
  EH_DECLARE;

	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
  EH_Zero(checkGLError(), "Error in GLShader::printLog");
  EH_Zero(length > 0);  // if length <= 0 then error
  EH_Ptr(log = new char[length]);

	glGetInfoLogARB(_shaderHandle, length, NULL, log);
  EH_Zero(checkGLError(), "Error in GLShader::printLog");

	cout << "---------- Begin: GL Shader Log ----------" << endl;
	cout << log << endl;
	cout << "----------- End: GL Shader Log -----------" << endl << endl;

  EH_OnError() {}

	if (log) delete[] log;
}




std::string GLShader::GetLog()
{
	GLint length = 0;
  char *log = NULL;
  std::string myshaderlog;
  EH_DECLARE;

	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
  EH_Zero(checkGLError(), "Error in GLShader::getlog");
  EH_Zero(length > 0);  // if length <= 0 then error
  EH_Ptr(log = new char[length]);

  glGetInfoLogARB(_shaderHandle, length, NULL, log);
	
	if(!checkGLError())
		myshaderlog = "Error in GLShader::printLog";
	else
		myshaderlog = "Shader Compiled Sucessfully";
	myshaderlog = myshaderlog + std::string("\nLOG OF COMPILATION:\n") + std::string(log );

	myshaderlog += GetSource();

  EH_OnError() {}

  if (log) delete[] log;

	return 	myshaderlog;
}





void GLShader::printSource()
{
	using namespace std;
	GLint length = 0;
  char *source = NULL;
  EH_DECLARE;

  glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB, &length);
  EH_Zero(checkGLError(), "Error in GLShader::printSource");
  EH_Zero(length > 0);  // if length <= 0 then error
  EH_Ptr(source = new char[length]);
	
  glGetShaderSourceARB(_shaderHandle, length, NULL, source);
  EH_Zero(checkGLError(), "Error in GLShader::printSource");

  cout << "---------- Begin: GL Shader Source ----------" << endl;
	cout << source << endl;
	cout << "----------- End: GL Shader Source -----------" << endl << endl;

  EH_OnError() {}

  if (source) delete[] source;
}




std::string GLShader::GetSource()
{
	GLint length = 0;
  char *source = NULL;
  std::string sourcecode = "";
  EH_DECLARE;

	glGetObjectParameterivARB(_shaderHandle, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB, &length);
  EH_Zero(checkGLError(), "Error in GLShader::GetSource");
  EH_Zero(length > 0);  // if length <= 0 then error
  EH_Ptr(source = new char[length]);
	
  glGetShaderSourceARB(_shaderHandle,length ,(GLsizei *)  &length, source);
  EH_Zero(checkGLError(), "Error in GLShader::GetSource");

	sourcecode += "---------- Begin: GL Shader Source ----------\n" ;
	sourcecode +=std::string( source);
	sourcecode += "----------- End: GL Shader Source -----------\n" ;

  EH_OnError() {}

  if (source) delete[] source;

  return sourcecode;
}




void GLShader::flagShaderDeletion()
{
  EH_DECLARE;
  glDeleteObjectARB(_shaderHandle);
  EH_Zero(checkGLError(), "Error in GLShader::flagShaderDeletion");
  EH_OnError() {}
}




char* GLShader::readFile(const char* filename)
{	
	using namespace std;
	ifstream file;
  char *out = NULL;
  char *intermediate = NULL;
  string contents = "";
  EH_DECLARE;

  file.open(filename);
	
	// open file
  EH_Zero(file.is_open(), "Error opening file: %s",filename);

	// read file into a string
	EH_Ptr(intermediate = new char[1025]);

	while(!file.eof())
	{
    memset(intermediate, '\0', 1025);
		file.read(intermediate, 1024);
		contents += intermediate;
	}

  // copy string to char*
  EH_Ptr(out = new char[contents.length()+1]);
  memcpy(out, contents.c_str(), contents.length());
  out[contents.length()] = '\0';

  EH_OnError() {}

	// close file
	if (file.is_open()) file.close();
	if (intermediate) delete[] intermediate;
	
/*	char *out = new char[contents.length()+1];
	contents.copy(out, contents.length());
	out[contents.length()] = '\0';
*/
  /*
	char *out = new char[contents.length()+1];
#if defined(USE_VC7) || __APPLE__
    strcpy(out, contents.c_str());
#else
	contents._Copy_s(out, contents.length(), contents.length(),0);// fix depereciated version!
#endif
  out[contents.length()] = '\0';
  */

	return out;
}




//--------- GLProgram -----------//
GLProgram::GLProgram()
{
  EH_DECLARE;
  attributeIndex = 1;
	_programHandle = glCreateProgramObjectARB();
  EH_Zero(checkGLError(), "Error in GLProgram::GLProgram");
  EH_OnError() {}
}


GLProgram::GLProgram(GLhandleARB programHandle)
{
	attributeIndex = 1;
	_programHandle = programHandle;
}



void GLProgram::printLog()
{
	using namespace std;
	GLint length = 0;
  char *log = NULL;
  EH_DECLARE;

  glGetObjectParameterivARB(_programHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
  EH_Zero(checkGLError(), "Error in GLProgram::printLog");
  EH_Zero(length > 0);  // if length <= 0 then error
  EH_Ptr(log = new char[length]);

  glGetInfoLogARB(_programHandle, length, NULL, log);
  EH_Zero(checkGLError(), "Error in GLProgram::printLog");

	cout << "---------- Begin: GL Program Log ----------" << endl;
	cout << log << endl;
	cout << "----------- End: GL Program Log -----------" << endl << endl;

  EH_OnError() {}

	if (log) delete[] log;
}



std::string GLProgram::GetLog()
{
  GLint length = 0;
  char *log = NULL;
  std::string myshaderlog;
  EH_DECLARE;

	glGetObjectParameterivARB(_programHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
  EH_Zero(checkGLError(), "Error in GLProgram::GetLog");
  EH_Zero(length > 0);  // if length <= 0 then error
  EH_Ptr(log = new char[length]);

  glGetInfoLogARB(_programHandle, length, NULL, log);

  if(!checkGLError())
    myshaderlog = "Error in GLProgram::printLog";
  else
    myshaderlog = "Shader Compiled Sucessfully";
  myshaderlog += log;
  
  EH_OnError() {}

  if (log) delete[] log;

  return myshaderlog;
}








GLProgram* GLProgram::getCurrentProgram()
{
	return new GLProgram(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
}



void GLProgram::addShader(GLShader* shader)
{
  EH_DECLARE;
	glAttachObjectARB(_programHandle, shader->getShaderHandle());
  EH_Zero(checkGLError(), "Error in GLProgram::addShader");
  EH_OnError() {}
}



void GLProgram::removeShader(GLShader* shader)
{
  EH_DECLARE;
	glDetachObjectARB(_programHandle, shader->getShaderHandle());
  EH_Zero(checkGLError(), "Error in GLProgram::removeShader");
  EH_OnError() {}
}



void GLProgram::use()
{
  EH_DECLARE;
  EH_Zero(checkGLError(), "Error in GLProgram::use   (ErrorIntrumentionA)");
	glLinkProgramARB(_programHandle);
  EH_Zero(checkGLError(), "Error in GLProgram::use   (ErrorIntrumentionB)");
	glUseProgramObjectARB(_programHandle);
  EH_Zero(checkGLError(), "Error in GLProgram::use   (ErrorIntrumentionC)");
  EH_OnError() {}
}



void GLProgram::link()
{
  EH_DECLARE;
	glLinkProgramARB(_programHandle);
  EH_Zero(checkGLError(), "Error in GLProgram::link");
  EH_OnError() {}
}



void GLProgram::enable()
{
  EH_DECLARE;
	glUseProgramObjectARB(_programHandle);
  EH_Zero(checkGLError(), "Error in GLProgram::enable");
  EH_OnError() {}
}



void GLProgram::flagProgramDeletion()
{
  EH_DECLARE;
	glDeleteObjectARB(_programHandle);
  EH_Zero(checkGLError(), "Error in GLProgram::flagProgramDeletion");
  EH_OnError() {}
}




GLAttributeVar* GLProgram::getAttributeVar(const char* varName)
{
	int index = attributeIndex;
  EH_DECLARE;
  attributeIndex++;

	glBindAttribLocationARB(_programHandle, index, varName);
  EH_Zero(checkGLError(), "Error in GLProgram::getAttributeVar(\"%s\")",varName);

  EH_OnError() {
    return NULL;
  }

	return new GLAttributeVar(index);
}



GLUniformVar* GLProgram::getUniformVar(const char* varName)
{
  int location = 0;
  EH_DECLARE;
 
	EH_Cmp(location = glGetUniformLocationARB(_programHandle, varName), -1, "Error in GLProgram::getUniformVar(\"%s\")",varName);
  EH_Zero(checkGLError(), "Error in GLProgram::getUniformVar(\"%s\")",varName);

  EH_OnError() {
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
void GLAttributeVar::set(GLshort x, GLshort y, GLshort z)
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