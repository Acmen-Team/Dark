#pragma once

#include <steam/steamclientpublic.h>
#include <steam/isteammatchmaking.h>

namespace Dark {

	enum EClientGameState
	{
		k_EClientGameStartServer,
		k_EClientGameActive,
		k_EClientGameWaitingForPlayers,
		k_EClientGameMenu,
		k_EClientGameQuitMenu,
		k_EClientGameExiting,
		k_EClientGameInstructions,
		k_EClientGameDraw,
		k_EClientGameWinner,
		k_EClientGameConnecting,
		k_EClientGameConnectionFailure,
		k_EClientFindInternetServers,
		k_EClientStatsAchievements,
		k_EClientCreatingLobby,
		k_EClientInLobby,
		k_EClientFindLobby,
		k_EClientJoiningLobby,
		k_EClientFindLANServers,
		k_EClientRemoteStorage,
		k_EClientLeaderboards,
		k_EClientFriendsList,
		k_EClientMinidump,
		k_EClientConnectingToSteam,
		k_EClientLinkSteamAccount,
		k_EClientAutoCreateAccount,
		k_EClientRetrySteamConnection,
		k_EClientClanChatRoom,
		k_EClientWebCallback,
		k_EClientMusic,
		k_EClientWorkshop,
		k_EClientHTMLSurface,
		k_EClientInGameStore,
		k_EClientRemotePlay,
		k_EClientOverlayAPI,
	};

	class Lobby
	{
	public:

		Lobby() = default;
		~Lobby() = default;

		void SetLobbySteamID(const CSteamID &steamIDLobby);

		void SetGameState(EClientGameState eState);

		//创建大厅
		void CreateLobby();

		//设置大厅元数据
		void SetLobbyData(CSteamID steamIDLobby, const char *pchKey, const char *pchValue);

		void OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure);

		void Refresh();

		void OnLobbyMatchListCallback(LobbyMatchList_t *pCallback, bool bIOFailure);

		////邀请好友进入大厅
		//bool InviteUserToLobby(CSteamID steamIDLobby, CSteamID steamIDInvitee);

	//加入大厅
		void JoinLobby(uint64 id);

		void OnLobbyEntered(LobbyEnter_t *pCallback, bool bIOFailure);

		//离开大厅
		void LeaveLobbys(uint64 id);

	private:

		struct Lobby_t
		{
			CSteamID m_steamIDLobby;
			char m_rgchName[256];
		};

		EClientGameState m_eGameState;

		bool m_bTransitionedGameState;


		uint64 m_SteamId;

		CSteamID m_steamIDLobby;

		Lobby *m_pLobby;


		std::list< Lobby_t > m_ListLobbies;

		CCallResult<Lobby, LobbyCreated_t> m_LobbyCreated;

		CCallResult<Lobby, LobbyMatchList_t> m_SteamCallResultLobbyMatchList;

		CCallResult<Lobby, LobbyEnter_t> m_SteamCallResultLobbyEntered;


	};
	
}