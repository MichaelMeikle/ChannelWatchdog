#include "channel_watchdog.h"

ChannelWatchdog::ChannelWatchdog()
{
	alert_status_ = true;
	list = std::make_unique<ChannelList>();
	console = std::make_unique<Log>();
	printf("Constructor complete\n");
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
	return "";
}

void ChannelWatchdog::ChangeChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage)
{
	char* result;
	if (!CheckError(ts3handle()->getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_UNIQUE_IDENTIFIER, &result)))
		return;
	std::string server_unique_id(result);
	if (list->SearchItem(server_unique_id, newChannelID))
	{
		console->Debug("Checking " + server_unique_id);
	}
}

void ChannelWatchdog::MenuEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) 
{
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

			break;
		case CHANNEL_DISABLE:

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

/*** Getters ***/
std::string ChannelWatchdog::plugin_name() { return kPluginName; }
std::string ChannelWatchdog::plugin_author() { return kPluginAuthor; }
std::string ChannelWatchdog::plugin_description() { return kPluginDescription; }
std::string ChannelWatchdog::plugin_version() { return kPluginVersion; }
/*** Setters ***/