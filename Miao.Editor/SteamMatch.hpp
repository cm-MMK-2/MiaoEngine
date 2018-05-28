#pragma once
#include "steam/steam_api.h"
#include <iostream>

class SteamMatch
{
public:
	SteamMatch()
	{	
		if (SteamAPI_Init())
		{
			std::cout << "steam api started..." << std::endl;
			steamMatchMaking = SteamMatchmaking();
		}
	}

	void RequestLobbiesList()
	{
		readyForNewRequest = false;
		SteamAPICall_t hSteamAPICall = steamMatchMaking->RequestLobbyList();
		m_CallResultLobbyMatchList.Set(hSteamAPICall, this, &SteamMatch::OnGetLobbiesList);
		std::cout << "RequestLobbiesList" << std::endl;
	}

	void Update()
	{
		SteamAPI_RunCallbacks();
	}

	bool readyForNewRequest = true;
private:
	CCallResult<SteamMatch, LobbyMatchList_t> m_CallResultLobbyMatchList;
	ISteamMatchmaking* steamMatchMaking;

	void OnGetLobbiesList(LobbyMatchList_t *pLobbyMatchList, bool bIOFailure)
	{
		readyForNewRequest = true;
		if (bIOFailure)
		{
			std::cout << "Get lobbies list failed" << std::endl;
			return;
		}
		std::cout << "Lobbies count: " << pLobbyMatchList->m_nLobbiesMatching << std::endl;
		char pchKey[k_nMaxLobbyKeyLength];
		char pchValue[k_cubChatMetadataMax];
		for (uint32 i = 0; i < pLobbyMatchList->m_nLobbiesMatching; i++)
		{
			CSteamID lobbyID = steamMatchMaking->GetLobbyByIndex(i);
			steamMatchMaking->RequestLobbyData(lobbyID);
			int dataCount = steamMatchMaking->GetLobbyDataCount(lobbyID);
			for (int j = 0; j < dataCount; j++)
			{
				if (steamMatchMaking->GetLobbyDataByIndex(lobbyID, j, pchKey, k_nMaxLobbyKeyLength, pchValue, k_cubChatMetadataMax))
				{
					std::cout << "index: " << j << ", key: " << pchKey << ", value: " << pchValue << std::endl;
				}
			}
		}
	}
};