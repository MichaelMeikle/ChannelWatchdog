#pragma 
#include "log.h"
#include "common.h"
#include <string>
#include <list>

class ChannelList
{
	std::string active_server_;
	bool list_updated_;
	std::list<uint64> notify_channel_list_;
	std::unique_ptr<Log> log;

public:
	ChannelList();
	~ChannelList();
	/* Called when change channel event is registered, checks for server uid list, then checks to see if the list contains the channel_id */
	bool SearchItem(std::string UID, uint64 channel_id);
	/* Adds item to list if not already added (Returns 0 for success, -1 for failure, 1 for already exists in list) */
	int AddItem(std::string UID, uint64 channel_id);
	/* Removes item from list if it exists (Returns 0 for success, -1 for failure) */
	int RemoveItem(std::string UID, uint64 channel_id);
private:
	void WriteList();
	bool InActiveList(uint64 channel_id);
	bool LoadList(std::string UID);
};
