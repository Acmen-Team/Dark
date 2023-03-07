#include "dkpch.h"

#include "Steamworks.h"

#include <steam/steam_api.h>
#include <steam/steam_api_common.h>

#include "Lobby.h"

namespace Dark {
  Lobby lobby;
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

  void Steamworks::RunCallBacks()
  {
    SteamAPI_RunCallbacks();
  }

  void Steamworks::CreatLobbys()
  {
    lobby.CreateLobby();
  }

  void Steamworks::JoinLobbys()
  {
    lobby.JoinLobby(109775243282076632);
  }

} // namespace Dark