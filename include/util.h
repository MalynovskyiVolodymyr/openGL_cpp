#ifndef UTIL
#define UTIL

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
/*
#ifdef _WIN64
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#pragma warning (disable: 4566)
#else*/
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define MAX(a,b) ((a) > (b) ? (a) : (b))


void OgldevError(const char* pFileName, uint line, const char* format, ... ){
	char msg[1000];
	va_list args;
	va_start(args, format);
	VSNPRINTF(msg, sizeof(msg), format, args);
	fprintf(stderr, "%s:%d - %s", pFileName, line, msg);
}

void OgldevFileError(const char* pFileName, uint line, const char* pFileError){
	fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
}

#define OGLDEV_ERROR0(msg) OgldevError(__FILE__, __LINE__, msg)
#define OGLDEV_ERROR(msg, ...) OgldevError(__FILE__, __LINE__, msg, __VA_ARGS__)
#define OGLDEV_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

using std::string;
using std::ifstream;


bool ReadFile(const char* pFileName, string &outFile){
	ifstream f(pFileName);
	
	bool ret = false;
	
	if(f.is_open()){
		string line;
		
		while(getline(f, line)){
			outFile.append(line);
			outFile.append("\n");
		}
		
		f.close();
		
		ret = true;
	}else{
		OGLDEV_FILE_ERROR(pFileName);
	}
	return ret;
}


struct PersProjInfo
{
    float FOV;
    float Width;
    float Height;
    float zNear;
    float zFar;
};

struct OrthoProjInfo
{
    float r;        // right
    float l;        // left
    float b;        // bottom
    float t;        // top
    float n;        // z near
    float f;        // z far

    void Print()
    {
        printf("Left %f   Right %f\n", l, r);
        printf("Bottom %f Top %f\n", b, t);
        printf("Near %f   Far %f\n", n, f);
    }
};

#endif
