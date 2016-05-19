#pragma once

#ifndef WIN32
#include <unistd.h>
#endif

#include <string>

#include "ogl/types.h"

bool readFile(const char *fileName, std::string &outFile);

void oglError(const char *pFileName, uint line, const char *pError);
void oglFileError(const char *pFileName, uint line, const char *pFileError);

#define OGL_ERROR(Error) oglError(__FILE__, __LINE__, Error);
#define OGL_FILE_ERROR(FileError) oglFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_LEN(a) (sizeof(a)/sizeof(*a))


#ifdef WIN32

#define SNPRINTF _snprintf_s
#define RANDOM rand
#define SRANDOM() srand((unsigned)time(NULL))

//float fmax(float a, float b);

#else

#define SNPRINTF sprintf
#define RANDOM random
#define SRANDOM() srandom(getpid())

#endif // WIN32


#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError() \
{ \
	GLenum _error_ = glGetError(); \
	if (_error_ != GL_NO_ERROR) { \
		printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, _error_); \
		exit(0); \
	} \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

long long getCurrentTimeMillis();

