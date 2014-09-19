/**=================================================================================================
 * File: SBTracker.h
 * =================================================================================================
 * \file SBTracker.h
 *
 * \brief Declares the tracker class.
 */
#ifndef _SB_TRACKER_H_
#define _SB_TRACKER_H_

namespace FCBondage
{
	class SBTracker
	{
	public:
		GetItemInfoFunction GetItemInfo;
		GetUserInputObjectFromRootInputFunction GetUserInputObjectFromRootInput;
		InitCommandStructFunction InitCommandStruct;
		void** ppRootInputObject;
		void* pThreadVerify;
		unsigned __int32* pThreadVerifyIndex;
		SBTracker(IncomingCommandFunction IncomingCommand);
		void SendCommand(const char* pCommandStr);
		void ClearValues();
		void DisplayCurrentValues();
		void PrintInfoToChat(unsigned __int16 itemID, float SB);
		void PrintInfoToConsole(unsigned __int16 itemID, float SB);
		bool HandleCommand(const char* pCommandStr);
		void HandleItemPacket(unsigned __int16 itemID, unsigned __int16 newSB);
		~SBTracker();

	private:
		float Curtana;
		float Sphairai;
		float Bravura;
		float GaeBolg;
		float ArtemisBow;
		float Thyrus;
		float StardustRod;
		float VeilOfWiyu;
		float Omnilex;
		float HolyShield;

		bool AutoUpdateFlag;
		bool ConsoleFlag;
		bool ShowAllFlag;
		std::atomic<unsigned __int32> pcount;

		std::mutex CommandSyncObject;
		std::mutex ItemInfoSyncObject;

		IncomingCommandFunction IncomingCommand;	
		std::string GetItemName(unsigned __int16 itemID);
		bool SetValues(unsigned __int16 itemID, unsigned __int16 newSB);
		const char* FirstArg(const char* pCmd);
		void ToggleConsole(bool on);
	};
}
#endif