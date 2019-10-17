#pragma once
#include "common.h"
#include "channel_list.h"
#include <string>
#include <memory>

class ChannelWatchdog
{
	const std::string kPluginName = "Channel Watchdog";
	const std::string kPluginAuthor = "Michael Meikle";
	const std::string kPluginDescription = "Channel Notifier";
	const std::string kPluginVersion = "1.1.0";
	bool alert_status_;

	std::unique_ptr<ChannelList> list;

public:
	ChannelWatchdog();
	int Initialize();
	void Shutdown();
	void ChangeChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage);
	void MenuEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID);
	void ShowMenuItem(MenuID item);
	void HideMenuItem(MenuID item);

	/*** Getters ***/
	std::string plugin_name();
	std::string plugin_author();
	std::string plugin_description();
	std::string plugin_version();
	/*** Setters ***/

private:
};