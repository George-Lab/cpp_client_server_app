#pragma once
#include <cstdint>


enum class MessageTypes : uint32_t
{
	Server_GetStatus,
	Server_GetPing,
	Server_StartsNewExperiment,
	Server_RepliesToClient_Correct,
	Server_RepliesToClient_NumberTooHigh,
	Server_RepliesToClient_NumberTooLow,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,
	Client_TriesToGuess

};