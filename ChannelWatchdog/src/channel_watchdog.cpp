#include "channel_watchdog.h"

ChannelWatchdog::ChannelWatchdog()
{
	alert_status_ = true;
	list = std::make_unique<ChannelList>();

}
int ChannelWatchdog::Initialize()
{
	return 0;
}
void ChannelWatchdog::Shutdown()
{

}
void ChannelWatchdog::ChangeChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage)
{

}
void ChannelWatchdog::MenuEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) 
{
	switch (menuItemID)
	{
		case GLOBAL_NOTIFICATION_ON:
			break;
		case GLOBAL_NOTIFICATION_OFF:
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
	ts3Functions.setPluginMenuEnabled(pluginID, item, 1);
}
void ChannelWatchdog::HideMenuItem(MenuID item)
{
	ts3Functions.setPluginMenuEnabled(pluginID, item, 0);
}

/*** Getters ***/
std::string ChannelWatchdog::plugin_name() { return kPluginName; }
std::string ChannelWatchdog::plugin_author() { return kPluginAuthor; }
std::string ChannelWatchdog::plugin_description() { return kPluginDescription; }
std::string ChannelWatchdog::plugin_version() { return kPluginVersion; }
/*** Setters ***/