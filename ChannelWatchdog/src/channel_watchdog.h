#pragma once
#include "common.h"
#include "channel_list.h"
#include "log.h"
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
	std::unique_ptr<Log> console;

public:
	ChannelWatchdog();
	int Initialize();
	void Shutdown();
	std::string ChannelInfoData(uint64 serverConnectionHandlerID, uint64 id);
	void ChangeChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage);
	void MenuEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID);
	void ShowMenuItem(MenuID item);
	void HideMenuItem(MenuID item);
	/* Takes a TS3 Func as param, returns if bool based on success (true) or failure */
	bool CheckError(int error_code);

	/*** Getters ***/
	std::string plugin_name();
	std::string plugin_author();
	std::string plugin_description();
	std::string plugin_version();
	/*** Setters ***/

private:
	std::string Get_Alert_Msg(uint64 server_id, anyID client_id, uint64 ch_id);
	std::string Get_Poke_Msg(uint64 server_id, anyID client_id, uint64 ch_id);
	std::string Sanitize_UID(std::string UID);
};

static std::unique_ptr<ChannelWatchdog> plugin = std::make_unique<ChannelWatchdog>();

static char banned_list[9] = { (char) 2567200, '/', ':', '*', '?', '<', '>', '"', '|'};