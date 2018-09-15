#ifndef _GAME_UTIL_
#define _GAME_UTIL_
#include<vector>
#include<string>
#include<mutex>
using namespace std;

#ifdef __LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif // __LINUX

//void log(const char* format,...);
void getDirList(const char* dirname, std::vector<string>& outList);
void getFileList(const char* dirname, std::vector<string>& outList);
void createDirectory(const char* dirname);
#endif // _GAME_UTIL_
