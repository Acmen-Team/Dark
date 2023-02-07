#include "dkpch.h"

#include "Lobby.h"

#include <steam/steam_api_common.h>

namespace Dark {
	void Lobby::SetLobbySteamID(const CSteamID & steamIDLobby)
	{
	}
	void Lobby::SetGameState(EClientGameState eState)
	{
		if (m_eGameState == eState)
			return;

		m_bTransitionedGameState = true;
		//m_ulStateTransitionTime = m_pGameEngine->GetGameTickCount();
		m_eGameState = eState;

	}
	void Lobby::CreateLobby() {
		
		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic , 4);
		DK_CORE_INFO("CreateLobby");
		// set the function to call when this completes
		m_LobbyCreated.Set(hSteamAPICall, this, &Lobby::OnLobbyCreated);

		//SteamFriends()->SetRichPresence("status", "Creating a lobby");
	}


	void Lobby::SetLobbyData(CSteamID steamIDLobby, const char * pchKey, const char * pchValue)
	{
		m_steamIDLobby = steamIDLobby;
	}

	void Lobby::OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure)
	{
		DK_CORE_INFO("into Lobby");
	    // record which lobby we're in
	    if (pCallback->m_eResult == k_EResultOK)
	    {
	   	 // success
		m_SteamId = pCallback->m_ulSteamIDLobby;
	   	 

	   	 // set the name of the lobby if it's ours
	   	 char rgchLobbyName[256];
	   	 strcpy(rgchLobbyName, SteamFriends()->GetPersonaName());
	   	 SteamMatchmaking()->SetLobbyData(m_SteamId, "name", rgchLobbyName);

	    }

	}

	void Lobby::Refresh() {
	
		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
		m_SteamCallResultLobbyMatchList.Set(hSteamAPICall, this, &Lobby::OnLobbyMatchListCallback);

	}

	void Lobby::OnLobbyMatchListCallback(LobbyMatchList_t *pCallback, bool bIOFailure) {
	
		m_ListLobbies.clear();

		if (bIOFailure) {}

		for (uint32 iLobby = 0; iLobby < pCallback->m_nLobbiesMatching; iLobby++) {
		
			CSteamID steamIDLobby = SteamMatchmaking()->GetLobbyByIndex(iLobby);

			// add the lobby to the list
			Lobby_t lobby;
			lobby.m_steamIDLobby = steamIDLobby;
			// pull the name from the lobby metadata
			const char *pchLobbyName = SteamMatchmaking()->GetLobbyData(steamIDLobby, "name");
			if (pchLobbyName && pchLobbyName[0])
			{
				// set the lobby name
				strcpy(lobby.m_rgchName, pchLobbyName);
			}
			else
			{
				// we don't have info about the lobby yet, request it
				SteamMatchmaking()->RequestLobbyData(steamIDLobby);
				// results will be returned via LobbyDataUpdate_t callback
				strcpy(lobby.m_rgchName, (char*)steamIDLobby.GetAccountID());
			}

			m_ListLobbies.push_back(lobby);

		}

		// TODO:在大厅展示搜索出来的房间名

	}

	void Lobby::JoinLobby(uint64 id) {

		DK_CORE_INFO("Join Lobby" + id);

		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->JoinLobby(id);

		m_SteamCallResultLobbyEntered.Set(hSteamAPICall, this, &Lobby::OnLobbyEntered);
	}

	void Lobby::OnLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure)
	{
		if (pCallback->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess)
		{
			// failed, show error
			DK_CORE_ERROR("Failed to enter lobby");
			return;
		}

		m_steamIDLobby = pCallback->m_ulSteamIDLobby;
		m_pLobby->SetLobbySteamID(m_steamIDLobby);
	}

	void Lobby::LeaveLobbys(uint64 id)
	{
		DK_CORE_INFO("Leave Lobby" + id);
		SteamMatchmaking()->LeaveLobby(m_steamIDLobby);
	}

}