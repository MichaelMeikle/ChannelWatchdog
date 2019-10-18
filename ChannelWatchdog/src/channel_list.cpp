#include "channel_list.h"
/* Called when change channel event is registered, checks for server uid list, then checks to see if the list contains the channel_id */
ChannelList::ChannelList()
{
	list_updated_ = false;
}
bool ChannelList::SearchItem(std::string UID, uint64 channel_id)
{
	if (active_server_.compare(UID) == 0)
	{
		for (uint64 ch_id : notify_channel_list_)
		{
			if (channel_id == ch_id)
				return true;
		}
	}
	else
	{
		if (!LoadList(UID))
			return false;

	}
	return false;
}

bool ChannelList::LoadList(std::string UID)
{
	active_server_ = UID;
	notify_channel_list_.clear();
}

/* Adds item to list if not already added (Returns 0 for success, -1 for failure, 1 for already exists in list) */
int ChannelList::AddItem(std::string UID, uint64 channel_id)
{
	return 0;
}
/* Removes item from list if it exists (Returns 0 for success, -1 for failure) */
int ChannelList::RemoveItem(std::string UID, uint64 channel_id)
{
	return 0;
}