/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-7-10 18:35
Description:FileSystem
**************************************************/

#pragma once

namespace Dark {

  //extern const std::filesystem::path g_AssetPath = "assets";

  class DARK_API FileSystem
  {
  public:
    template <typename TFunction>
    static void TraverseFiles(const std::filesystem::path& path, TFunction tFunc)
    {
      for (auto& directoryEntry : std::filesystem::directory_iterator(path))
      {
        const auto& dirPath           = directoryEntry.path();
        std::string strPath = dirPath.string();
        std::replace(strPath.begin(), strPath.end(), '\\', '/');

        if (dirPath.has_extension())
        {
          tFunc(strPath, dirPath.extension());
        }

        // Traverse
        if (!directoryEntry.is_directory())
          continue;

        TraverseFiles(directoryEntry, tFunc);
      }
    }


    static std::pair<void*, size_t> ReadFile(const std::string& path)
    {
      std::ifstream infile;
      infile.open(path, std::ios::in | std::ios::binary | std::ios::ate);

      size_t size = infile.tellg();
      infile.seekg(0);

      DK_CORE_TRACE("{0} Size:{1}", path, size);

      void* result = s_MemoryManager.AllocateMemory(size);
      DK_CORE_TRACE("MemoryManager: NumsAllocations:{0}, UsedMemory:{1}", s_MemoryManager.GetNumAllocations(), s_MemoryManager.GetUsedMemory());

      infile.read((char*)result, size);

      infile.close();

      return {result, size};
    }

    /*

     /// Mounts a file device to the file system
    void Mount(FileDevice* device);

    /// Unmounts a file device from the file system
    void Unmount(FileDevice* device);

    /// Opens a file for synchronous operations.
    /// NOTE: A nullptr is returned if no device for opening the file could be found.
    File* Open(const char* deviceList, const char* path, Mode mode);

    /// Opens a file for asynchronous operations.
    /// NOTE: A nullptr is returned if no device for opening the file could be found.
    AsyncFile* OpenAsync(const char* deviceList, const char* path, Mode mode);

    /// Closes a file previously returned by a call to Open()
    void Close(File* file);

    /// Closes a file previously returned by a call to OpenAsync()
    void Close(AsyncFile* file);

    */
  };

} // namespace Dark