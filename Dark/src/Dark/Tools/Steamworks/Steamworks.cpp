#include "dkpch.h"

#include "Steamworks.h"

#include <steam/steam_api.h>

namespace Dark {

  void Steamworks::InitSteamAPI()
  {
    if (!SteamAPI_Init())
    {
      DK_CORE_ERROR("Init SteamAPI Error");
    }
    else
    {
      DK_CORE_INFO("Init SteamAPI Success");
    }
  }

  void Steamworks::ShutdownSteamAPI()
  {
    SteamAPI_Shutdown();
  }

} // namespace Dark