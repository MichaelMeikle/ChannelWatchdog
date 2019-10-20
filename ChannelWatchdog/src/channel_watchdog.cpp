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
	list->Shutdown();
}

std::string ChannelWatchdog::ChannelInfoData(uint64 serverConnectionHandlerID, uint64 itemID)
{
	return "";
}

void ChannelWatchdog::ChangeChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage)
{
	if (!alert_status_)
		return;

	char* result;
	if (!CheckError(ts3handle()->getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &result)))
		return;
	std::string server_unique_id(result);
	
	if (list->SearchItem(server_unique_id, newChannelID))
	{
		console->Debug("Switch to monitored channel detected " + server_unique_id);
		console->PrintMsg(serverConnectionHandlerID, Get_Alert_Msg(serverConnectionHandlerID, clientID, newChannelID));
	}
}

void ChannelWatchdog::MenuEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) 
{
	char* result;
	if (!CheckError(ts3handle()->getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &result)))
		return;

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
		case 12:
			list->WriteList();
			printf("Write function called...\n");
			break;
		case CHANNEL_ENABLE:
			list->AddItem(result, selectedItemID);
			break;
		case CHANNEL_DISABLE:
			list->RemoveItem(result, selectedItemID);
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
bool ChannelWatchdog::CheckError(int error_code) { return (error_code == ERROR_ok); }

std::string ChannelWatchdog::Get_Alert_Msg(uint64 server_id, anyID client_id, uint64 ch_id)
{
	char name[64];
	ts3handle()->getClientDisplayName(server_id, client_id, name, 64);
	std::string client_name(name);
	char* result;
	ts3handle()->getChannelVariableAsString(server_id, ch_id, CHANNEL_NAME, &result);
	std::string channel_name(result);
	delete result;
	ts3handle()->getClientVariableAsString(server_id, client_id, CLIENT_UNIQUE_IDENTIFIER, &result);
	std::string client_unique(result);
	delete result;
	uint64 p_chid;
	ts3handle()->getParentChannelOfChannel(server_id, ch_id, &p_chid);
	ts3handle()->getChannelVariableAsString(server_id, p_chid, CHANNEL_NAME, &result);
	std::string parent_name(result);
	delete result;

	auto current = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(current);
	std::string timeStr = std::ctime(&currentTime);
	return "[color=Orange]--------------------------------\n" + timeStr + "--------------------------------"
		+ "\nParent Channel Name: [color=red]" + parent_name + "[/color]"
		+ "\nChannel Name: [color=red]" + channel_name + "[/color]"
		+ "\nClient Name: [color=red]" + client_name + "[/color]"
		+ "\nClient UID: [color=red]" + client_unique + "[/color]"
		+ "[/color]";
}

/*** Getters ***/
std::string ChannelWatchdog::plugin_name() { return kPluginName; }
std::string ChannelWatchdog::plugin_author() { return kPluginAuthor; }
std::string ChannelWatchdog::plugin_description() { return kPluginDescription; }
std::string ChannelWatchdog::plugin_version() { return kPluginVersion; }
/*** Setters ***/