#include <iostream>
#include <fstream>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif // WIN32

#include "ogl/util.h"


bool readFile(const char * pFileName, std::string &outFile)
{
	std::ifstream f(pFileName);
	
	bool ret = false;

	if (f.is_open())
	{
		std::string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();

		ret = true;
	}
	else
	{
		OGL_FILE_ERROR(pFileName);
	}

	return ret;
}

void oglError(const char *pFileName, uint line, const char *pError)
{
#ifdef WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: %s", pFileName, line, pError);
	//MessageBoxA(NULL, msg, NULL, 0);
	OutputDebugStringA(msg);
#else
	fprintf(std::stderr, "%s:%d: %s", pFileName, line, pError);
#endif // WIN32
}

void oglFileError(const char *pFileName, uint line, const char *pFileError)
{
#ifdef WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file '%s'", pFileName, line, pFileError);
	//MessageBoxA(NULL, msg, NULL, 0);
	OutputDebugStringA(msg);

#else
	fprintf(std::stderr, "%s:%d: unable to open file '%s'", pFileName, line , pFileError);
#endif // WIN32
}

long long getCurrentTimeMillis()
{
#ifdef WIN32
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif // WIN32
}

//#ifdef WIN32
//#if _MSC_VER != 1800
//
//float fmax(float a, float b)
//{
//	return a > b ? a : b;
//}
//
//#endif // _MSC_VER != 1800
//#endif // WIN32

