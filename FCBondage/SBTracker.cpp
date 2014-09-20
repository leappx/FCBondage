/*
	Copyright (C) 2014  DevNull:ffevo

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**=================================================================================================
 * File: SBTracker.cpp
 * =================================================================================================
 * \file SBTracker.cpp
 *
 * \brief Implements the tracker class.
 */

#include "stdafx.h"

/**=================================================================================================
 * Constructor: FCBondage::SBTracker::SBTracker
 * =================================================================================================
 * \fn FCBondage::SBTracker::SBTracker(IncomingCommandFunction IncomingCommand)
 *
 * \brief Constructor.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param Pointer to the game's command handler.
 */

FCBondage::SBTracker::SBTracker(IncomingCommandFunction IncomingCommand)
{
	this->IncomingCommand = IncomingCommand;
	ClearValues();
	AutoUpdateFlag = true;
	ConsoleFlag = false;
	ShowAllFlag = false;
	RawView = false;
	pcount = 0;
}

/**=================================================================================================
 * Destructor: FCBondage::SBTracker::~SBTracker
 * =================================================================================================
 * \fn FCBondage::SBTracker::~SBTracker()
 *
 * \brief Destructor.
 *
 * \author DevNull
 * \date 9/19/2014
 */

FCBondage::SBTracker::~SBTracker()
{

}

/**=================================================================================================
 * Method: FCBondage::SBTracker::HandleItemPacket
 * =================================================================================================
 * \fn void FCBondage::SBTracker::HandleItemPacket(unsigned __int16 itemID, unsigned __int16 newSB)
 *
 * \brief Handles the item packet.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 * \param newSB  The new Spirit Bind value.
 */

void FCBondage::SBTracker::HandleItemPacket(unsigned __int16 itemID, unsigned __int16 newSB)
{
	// Determine if this packet pertains to a novus item, Set new values if it does, and print results if we are only tracking Novus.
	if (SetValues(itemID, newSB) && AutoUpdateFlag && !ShowAllFlag)
		std::async(std::launch::async, &FCBondage::SBTracker::DisplayCurrentValues, this);

	// Determine if we should print this info to the console, and/or print regardless if this is novus.
	if (ConsoleFlag || ShowAllFlag)
	{
		auto newSBf = newSB * 0.01f;
		if (ShowAllFlag)
			std::async(std::launch::async, &FCBondage::SBTracker::PrintInfoToChat,this,itemID,newSBf);
		if (ConsoleFlag)
			std::async(std::launch::async, &FCBondage::SBTracker::PrintInfoToConsole,this,itemID,newSBf);
	}
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::SetValues
 * =================================================================================================
 * \fn bool FCBondage::SBTracker::SetValues(unsigned __int16 itemID, unsigned __int16 newSB)
 *
 * \brief Sets the novus values.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 * \param newSB  The new Spirit Bind value.
 *
 * \return true if this is a novus, false if other item.
 */

bool FCBondage::SBTracker::SetValues(unsigned __int16 itemID, unsigned __int16 newSB)
{
	auto newSBf = newSB * 0.01f;
	switch (itemID)
	{
	case 7863:
		Curtana = newSBf;
		return true;
	case 7864:
		Sphairai = newSBf;
		return true;
	case 7865:
		Bravura = newSBf;
		return true;
	case 7866:
		GaeBolg = newSBf;
		return true;
	case 7867:
		ArtemisBow = newSBf;
		return true;
	case 7868:
		Thyrus = newSBf;
		return true;
	case 7869:
		StardustRod = newSBf;
		return true;
	case 7870:
		VeilOfWiyu = newSBf;
		return true;
	case 7871:
		Omnilex = newSBf;
		return true;
	case 7872:
		HolyShield = newSBf;
		return true;
	default:
		return false;
	}
	return false;
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::DisplayCurrentValues
 * =================================================================================================
 * \fn void FCBondage::SBTracker::DisplayCurrentValues()
 *
 * \brief Displays current values to chat.
 *
 * \author DevNull
 * \date 9/19/2014
 */

void FCBondage::SBTracker::DisplayCurrentValues()
{
	if (Curtana != 0.0f)
		PrintInfoToChat(7863,Curtana);
	if (Sphairai != 0.0f)
		PrintInfoToChat(7864,Sphairai);
	if (Bravura != 0.0f)
		PrintInfoToChat(7865,Bravura);
	if (GaeBolg != 0.0f)
		PrintInfoToChat(7866,GaeBolg);
	if (ArtemisBow != 0.0f)
		PrintInfoToChat(7867,ArtemisBow);
	if (Thyrus != 0.0f)
		PrintInfoToChat(7868,Thyrus);
	if (StardustRod != 0.0f)
		PrintInfoToChat(7869,StardustRod);
	if (VeilOfWiyu != 0.0f)
		PrintInfoToChat(7870,VeilOfWiyu);
	if (Omnilex != 0.0f)
		PrintInfoToChat(7871,Omnilex);
	if (HolyShield != 0.0f)
		PrintInfoToChat(7872,HolyShield);
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::PrintInfoToChat
 * =================================================================================================
 * \fn void FCBondage::SBTracker::PrintInfoToChat(unsigned __int16 itemID, float SB)
 *
 * \brief Prints information to chat.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 * \param SB	 The Spirit Bind value.
 */

void FCBondage::SBTracker::PrintInfoToChat(unsigned __int16 itemID, float SB)
{
	// Attempt to retrieve an info structure of the given item.
	auto itemName = GetItemName(itemID);
	if (itemName.empty())
	{
		// Most likely a new item. Wait for transfer to complete.
		Sleep(500);
		itemName = GetItemName(itemID);
	}
	std::stringstream ss;
	ss << "/echo ~" << (!itemName.empty() ? itemName : "Item Not Found") << "~ || SB: " << (!RawView ? SB : SB*100.0f) << (!RawView ? " %" : "");
	SendCommand(ss.str().c_str());
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::PrintInfoToConsole
 * =================================================================================================
 * \fn void FCBondage::SBTracker::PrintInfoToConsole(unsigned __int16 itemID, float SB)
 *
 * \brief Print information to console.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 * \param SB	 The Spirit Bind value.
 */

void FCBondage::SBTracker::PrintInfoToConsole(unsigned __int16 itemID, float SB)
{
	// Same process as above.
	auto itemName = GetItemName(itemID);
	if (itemName.empty())
	{
		Sleep(500);
		itemName = GetItemName(itemID);
	}
	std::cout << ++pcount <<". Item: ~" << (!itemName.empty() ? itemName : "Item Not Found") << "~ || New SB: " << (!RawView ? SB : SB*100.0f) << (!RawView ? " %" : "") << "\n";
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::ClearValues
 * =================================================================================================
 * \fn void FCBondage::SBTracker::ClearValues()
 *
 * \brief Clears the values.
 *
 * \author DevNull
 * \date 9/19/2014
 */

void FCBondage::SBTracker::ClearValues()
{
	Curtana = 0.0f;
	Sphairai = 0.0f;
	Bravura = 0.0f;
	GaeBolg = 0.0f;
	ArtemisBow = 0.0f;
	Thyrus = 0.0f;
	StardustRod = 0.0f;
	VeilOfWiyu = 0.0f;
	Omnilex = 0.0f;
	HolyShield = 0.0f;
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::GetItemName
 * =================================================================================================
 * \fn std::string FCBondage::SBTracker::GetItemName(unsigned __int16 itemID)
 *
 * \brief Calls the sub routine of the tool tip class, that retrieves info of an item.
 * 		  NOTE: This function occasionally asks the server for the needed info, and does not wait for a response.
 * 				You may need to wait and re-call if this happens.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param itemID Identifier for the item.
 *
 * \return The item name.
 */

std::string FCBondage::SBTracker::GetItemName(unsigned __int16 itemID)
{
	// This routine uses a shared buffer, and needs to be thread safe.
	std::lock_guard<std::mutex> lock(ItemInfoSyncObject);

	std::string result;
	try
	{
		auto itemInfo = GetItemInfo(itemID);
		if (itemInfo == NULL)
			return result;
		// The returned structure stores the name with a dynamic offset. The bellow reads the offset and determines where the string starts.
		result = std::string(&reinterpret_cast<const char*>(itemInfo)[itemInfo->OffsetOfName + offsetof(BasicItemInfo,OffsetOfName)]);
	}
	catch (...)
	{
		result = std::string("");
	}
	return result;
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::SendCommand
 * =================================================================================================
 * \fn void FCBondage::SBTracker::SendCommand(const char* pCommandStr)
 *
 * \brief Sends a command.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param pCommandStr The command string.
 */

void FCBondage::SBTracker::SendCommand(const char* pCommandStr)
{
	// We need to store a local copy of the given string, there is no guarantee the pointer on the stack will always be valid.
	auto localString = std::string(pCommandStr);

	// This routine uses a shared buffer, and needs to be thread safe.
	std::lock_guard<std::mutex> lock(CommandSyncObject);

	if (!ppRootInputObject)
		return;

	// The game is not always ready to receive a new command, the mutex does cut out virtually all issues, but some do still occ. arise
	try
	{
		CommandStruct cmdStruct;
		InitCommandStruct(&cmdStruct,localString.c_str(),-1);
		auto userInputObject = GetUserInputObjectFromRootInput(*ppRootInputObject);
		auto chatLogObject = userInputObject->CalculateAddressOfChatLog[8](userInputObject);
		IncomingCommand(chatLogObject,&cmdStruct,userInputObject);
	}
	catch (...)
	{
	}
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::HandleCommand
 * =================================================================================================
 * \fn bool FCBondage::SBTracker::HandleCommand(const char* pCommandStr)
 *
 * \brief Handles the command described by pCommandStr.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param pCommandStr The command string.
 *
 * \return true if the command was handled, false if this is a game command.
 */

bool FCBondage::SBTracker::HandleCommand(const char* pCommandStr)
{
	// All custom commands start with "//"
	if (pCommandStr[1] != 0x2F)
		return false;

	// Get the first argument or null if no argument.
	auto firstArg = FirstArg(pCommandStr);

	// Console toggle command
	if (strstr(pCommandStr, "//console"))
	{
		if (firstArg == NULL)
			return false;
		if (!strcmp(firstArg,"on"))
		{
			ToggleConsole(true);
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Console Opened.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			ToggleConsole(false);
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Console Closed.");
		}
		return true;
	}

	// AutoUpdate toggle command
	if (strstr(pCommandStr, "//autoupdate"))
	{
		if (firstArg == NULL)
			return false;
		if (!strcmp(firstArg,"on"))
		{
			AutoUpdateFlag = true;
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Auto Updating Values.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			AutoUpdateFlag = false;
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Not Auto Updating Values.");
		}
		return true;
	}

	// ShowAll toggle command
	if (strstr(pCommandStr, "//showall"))
	{
		if (firstArg == NULL)
			return false;
		if (!strcmp(firstArg,"on"))
		{
			ShowAllFlag = true;
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Showing All Items.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			ShowAllFlag = false;
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Showing Only Novus Items.");
		}
		return true;
	}

	// Invoke DisplayCurrentValues
	if (strstr(pCommandStr, "//showcur"))
	{
		std::async(std::launch::async, &FCBondage::SBTracker::DisplayCurrentValues,this);
		return true;
	}

	// Set all values to 0.
	if (strstr(pCommandStr, "//reset"))
	{
		std::async(std::launch::async, &FCBondage::SBTracker::ClearValues,this);
		std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Values Cleared.");
		return true;
	}

	// Toggle value display mode
	if (strstr(pCommandStr, "//rawview"))
	{
		if (firstArg == NULL)
			return false;
		if (!strcmp(firstArg,"on"))
		{
			RawView = true;
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Showing values as raw integers.");
		}
		else if (!strcmp(firstArg,"off"))
		{
			RawView = false;
			std::async(std::launch::async, &FCBondage::SBTracker::SendCommand,this,"/echo Showing values as decimals.");
		}
		return true;
	}

	// Badly formatted command.
	return false;
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::FirstArg
 * =================================================================================================
 * \fn const char* FCBondage::SBTracker::FirstArg(const char* pCmd)
 *
 * \brief Gets the first argument of a command.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param pCmd The command.
 *
 * \return null if it fails, else a char*.
 */

const char* FCBondage::SBTracker::FirstArg(const char* pCmd)
{
	while (pCmd[0])
		if (pCmd++[0] == ' ')
			return pCmd;
	return NULL;
}

/**=================================================================================================
 * Method: FCBondage::SBTracker::ToggleConsole
 * =================================================================================================
 * \fn void FCBondage::SBTracker::ToggleConsole(bool on)
 *
 * \brief Toggle console visibility.
 *
 * \author DevNull
 * \date 9/19/2014
 *
 * \param false = hidden, true = visible.
 */

void FCBondage::SBTracker::ToggleConsole(bool on)
{
	ConsoleFlag = on;
	if (on)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		return;
	}
	FreeConsole();
}
