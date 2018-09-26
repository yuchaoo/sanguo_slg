#pragma once
#include "Ref.h"
#include <string>
#include <memory>
using namespace std;

class FileSystem : public Ref
{
public:
    static FileSystem* getInstance();
    const std::string& getWorkspace() const { return m_workspace; }
    void setWorkspace(const char* workspace);
    std::string getAbsolutePath(const char* filepath);
    std::string FileSystem::getLuaFilePath(const string& path);
    std::shared_ptr<unsigned char> getFileData(const char* filepath, size_t& size);
private:
    FileSystem();
    ~FileSystem();
private:
    std::string m_workspace;
};