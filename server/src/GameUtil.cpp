#include"GameUtil.h"
#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#endif

/*void log(const char* format,...)
{
    static char text[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(text, sizeof(text), format, ap);
    va_end(ap);
    printf("%s\n", text);
}*/

void getDirList(const char* dirname, std::vector<string>& outList)
{
#ifdef _WIN32 
    char szFind[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    strcpy(szFind, dirname);
    strcat(szFind, "/*.*");
    HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind)
        return;
    while (TRUE)
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FindFileData.cFileName[0] != '.')
            {
                outList.push_back(FindFileData.cFileName);
            }
        }
        if (!FindNextFile(hFind, &FindFileData))
            break;
    }
    FindClose(hFind);
#else
        DIR * pdir = NULL;
        struct dirent* pdirent = NULL;
        struct stat st;
        char fullpath[PATH_MAX] = {0};

        pdir = opendir(dirname);
        if(pdir == NULL)
                return;

        for(pdirent = readdir(pdir); pdirent != NULL; pdirent = readdir(pdir))
        {
                if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0)
                        continue;
                snprintf(fullpath,sizeof(fullpath),"%s/%s",dirname,pdirent->d_name);
                lstat(fullpath,&st);
                if(S_ISDIR(st.st_mode))
                {
                        outList.push_back(pdirent->d_name);
                }
        }
        closedir(pdir);
#endif
}

void getFileList(const char* dirname, std::vector<string>& outList)
{

}

void createDirectory(const char* dirname)
{
#ifdef _WIN32 
    _mkdir(dirname);
#else

#endif
}
