#pragma once
#include "common.h"
class ChannelList
{
public:
private:

};
/*
When a user switches a channel, the channel list is called.
Each time list is called it saves the loaded list for quick reuse if the events happen on same server, if event from different server comes in list reloads
Servers tracked by UID
Lists are identified by UID in filename
List can be loaded and unloaded
Use JSON
*/