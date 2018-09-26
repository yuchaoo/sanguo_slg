#include "FileSystem.h"
#include <memory>
#include "Log.h"

FileSystem* FileSystem::getInstance()
{
    static FileSystem fs;
    return &fs;
}

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

void FileSystem::setWorkspace(const char* workspace) 
{
    m_workspace = workspace; 
    g_log("set the workspace : %s", m_workspace.c_str());
}

std::string FileSystem::getAbsolutePath(const char* filepath)
{
    return m_workspace + "/" + filepath;
}

std::string FileSystem::getLuaFilePath(const string& path)
{
    size_t end = path.rfind(".lua");
    std::string tmp = path;
    if (end != std::string::npos)
        tmp = path.substr(0, end);

    size_t pos = tmp.find(".");
    while (pos != std::string::npos)
    {
        tmp.replace(pos, pos + 1, "/");
        pos = tmp.find(".", pos + 1);
    }
    return m_workspace + "scripts/" + tmp + ".lua";
}

std::shared_ptr<unsigned char> FileSystem::getFileData(const char* filepath, size_t& size)
{
    FILE* file = fopen(filepath, "rb");
    if (!file)
    {
        g_log("read the file %s failed!!", filepath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char text[3];
    if (size >= 3)
    {
        fread(text, 1, 3, file);
        unsigned bom = text[0] | (text[1] << 8) | (text[2] << 16);
        if (bom != 0xBFBBEF && bom != 0xEFBBBF)
        {
            fseek(file, 0, SEEK_SET);
        }
        else
        {
            size -= 3;
        }
    }

    std::shared_ptr<unsigned char> buffer = std::shared_ptr<unsigned char>(new  unsigned char[size]);
    size = fread(buffer.get(), sizeof(char), size, file);
    fclose(file);
    return buffer;
}