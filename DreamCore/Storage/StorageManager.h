#pragma once

#include "File.h"
#include "Directory.h"


#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::unique_ptr;

namespace octronic::dream
{
  class File;
  class Directory;

  class StorageManager
  {
  public:
    StorageManager();

    StorageManager(const StorageManager&) = delete;
    StorageManager& operator=(const StorageManager&) = delete;

    StorageManager(StorageManager&&) = default;
    StorageManager& operator=(StorageManager&&) = default;

    virtual File& openFile(const string& path);
    void closeFile(const File& f);

    virtual Directory& openDirectory(const string& path);
    void closeDirectory(const Directory& d);

  protected:
    vector<unique_ptr<File>> mOpenFiles;
    vector<unique_ptr<Directory>> mOpenDirectories;
  };
}
