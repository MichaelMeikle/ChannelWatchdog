#include <ctime>
#include <thread>
#include "channel_watchdog.h"


ChannelWatchdog::ChannelWatchdog()
{
	alert_status_ = true;
	list = std::make_unique<ChannelList>();
	console = std::make_unique<Log>();
}

int ChannelWatchdog::Initialize()
{
	QLog t("", "Initialize");
	return 0;
}

void ChannelWatchdog::Shutdown()
{
	QLog t("", "Shutdown");
}

std::string ChannelWatchdog::ChannelInfoData(uint64 serverConnectionHandlerID, uint64 itemID)
{
	char* result;
	if (!CheckError(ts3handle()->getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &result)))
		return "";
	std::string server_unique_id(Sanitize_UID(result));
	std::string status = "Watch Status: ";

	if (list->SearchItem(server_unique_id, itemID))
	{
		ShowMenuItem(CHANNEL_DISABLE);
		HideMenuItem(CHANNEL_ENABLE);
		status.append("[color=green]Monitored[/color]");
	}
	else
	{
		HideMenuItem(CHANNEL_DISABLE);
		ShowMenuItem(CHANNEL_ENABLE);
		status.append("[color=red]Unmonitored[/color]");
	}

	return status;
}

void ChannelWatchdog::ChangeChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage)
{
	if (!alert_status_)
		return;

	char* result;
	if (!CheckError(ts3handle()->getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &result)))
		return;
	std::string server_unique_id(Sanitize_UID(result));
	delete result;

	anyID own_id = NULL;
	ts3handle()->getClientID(serverConnectionHandlerID, &own_id);

	if (own_id == clientID)
		return;

	if (list->SearchItem(server_unique_id, newChannelID))
	{
		console->Debug("Switch to monitored channel detected " + server_unique_id);
		console->SelfMsg(serverConnectionHandlerID, Get_Alert_Msg(serverConnectionHandlerID, clientID, newChannelID), own_id);
		ts3handle()->requestClientPoke(serverConnectionHandlerID, own_id, Get_Poke_Msg(serverConnectionHandlerID, clientID, newChannelID).c_str(), NULL);
		std::string sound_path = std::string(get_path()) + "channel_watchdog/alert.wav";
		ts3handle()->playWaveFile(serverConnectionHandlerID, sound_path.c_str());
	}
}

void ChannelWatchdog::MenuEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) 
{
	char* result;
	if (!CheckError(ts3handle()->getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &result)))
		return;
	std::string sanitized(Sanitize_UID(result));
	switch (menuItemID)
	{
		case GLOBAL_NOTIFICATION_ON:
			alert_status_ = true;
			HideMenuItem(GLOBAL_NOTIFICATION_ON);
			ShowMenuItem(GLOBAL_NOTIFICATION_OFF);
			break;
		case GLOBAL_NOTIFICATION_OFF:
			alert_status_ = false;
			HideMenuItem(GLOBAL_NOTIFICATION_OFF);
			ShowMenuItem(GLOBAL_NOTIFICATION_ON);
			break;
		case CHANNEL_ENABLE:
			list->AddItem(sanitized, selectedItemID);
			ShowMenuItem(CHANNEL_DISABLE);
			HideMenuItem(CHANNEL_ENABLE);
			break;
		case CHANNEL_DISABLE:
			list->RemoveItem(sanitized, selectedItemID);
			HideMenuItem(CHANNEL_DISABLE);
			ShowMenuItem(CHANNEL_ENABLE);
			break;
		default:
			break;
	}
}

void ChannelWatchdog::ShowMenuItem(MenuID item)
{
	ts3handle()->setPluginMenuEnabled(plugin_id(), item, 1);
}
void ChannelWatchdog::HideMenuItem(MenuID item)
{
	ts3handle()->setPluginMenuEnabled(plugin_id(), item, 0);
}
//Returns true if no issue
bool ChannelWatchdog::CheckError(int error_code) { return (error_code == ERROR_ok); }

std::string ChannelWatchdog::Get_Alert_Msg(uint64 server_id, anyID client_id, uint64 ch_id)
{
	char name[64];
	std::string client_name, channel_name, client_unique, parent_name;
	if (CheckError(ts3handle()->getClientDisplayName(server_id, client_id, name, 64)))
	{
		client_name = std::string(name);
		delete name;
	}
	else
		client_name = "[Hidden]";

	char* result;
	if (CheckError(ts3handle()->getChannelVariableAsString(server_id, ch_id, CHANNEL_NAME, &result)))
	{
		channel_name = std::string(result);
		delete result;
	}
	else
		channel_name = "[Hidden]";

	if (CheckError(ts3handle()->getClientVariableAsString(server_id, client_id, CLIENT_UNIQUE_IDENTIFIER, &result)))
	{
		client_unique = std::string(result);
		delete result;
	}
	else
		client_unique = "[Hidden]";

	uint64 p_chid;
	if (CheckError(ts3handle()->getParentChannelOfChannel(server_id, ch_id, &p_chid)))
	{
		if (CheckError(ts3handle()->getChannelVariableAsString(server_id, p_chid, CHANNEL_NAME, &result)))
		{
			parent_name = std::string(result);
			delete result;
		}
	}
	if (parent_name.empty())
		parent_name = "[Hidden]";

	auto current = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(current);
	std::string timeStr = std::ctime(&currentTime);
	return "[color=Orange]\n--------------------------------\n" + timeStr + "--------------------------------"
		+ "\nParent Channel Name: [color=red]" + parent_name + "[/color]"
		+ "\nChannel Name: [color=red]" + channel_name + "[/color]"
		+ "\nClient Name: [color=red]" + client_name + "[/color]"
		+ "\nClient UID: [color=red]" + client_unique + "[/color]"
		+ "\n--------------------------------[/color]";
}

std::string ChannelWatchdog::Get_Poke_Msg(uint64 server_id, anyID client_id, uint64 ch_id)
{
	char name[64];
	std::string client_name, channel_name;
	if (CheckError(ts3handle()->getClientDisplayName(server_id, client_id, name, 64)))
	{
		client_name = std::string(name);
		delete name;
	}
	else
		client_name = "[Hidden]";

	char* result;
	if (CheckError(ts3handle()->getChannelVariableAsString(server_id, ch_id, CHANNEL_NAME, &result)))
	{
		channel_name = std::string(result);
		delete result;
	}
	else
		channel_name = "[Hidden]";

	return client_name + " joined " + channel_name;
}

std::string ChannelWatchdog::Sanitize_UID(std::string UID)
{
	std::string sanitized;
	for (size_t i = 0; i < UID.size(); i++)
	{
		bool valid = true;
		for (char x : banned_list)
		{
			if (UID[i] == x)
				valid = false;
		}
		if (valid)
			sanitized += UID[i];
	}
	return sanitized;
}

/*** Getters ***/
std::string ChannelWatchdog::plugin_name() { return kPluginName; }
std::string ChannelWatchdog::plugin_author() { return kPluginAuthor; }
std::string ChannelWatchdog::plugin_description() { return kPluginDescription; }
std::string ChannelWatchdog::plugin_version() { return kPluginVersion; }
/*** Setters ***/