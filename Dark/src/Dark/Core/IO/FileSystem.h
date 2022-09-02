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
  };

} // namespace Dark