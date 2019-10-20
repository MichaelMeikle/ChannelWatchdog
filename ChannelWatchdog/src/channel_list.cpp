#include "channel_list.h"
#include <iostream>
#include <fstream>
#include <algorithm>

/* Called when change channel event is registered, checks for server uid list, then checks to see if the list contains the channel_id */
ChannelList::ChannelList()
{
	list_updated_ = false;
	log = std::make_unique<Log>();
}
ChannelList::~ChannelList()
{
}
void ChannelList::Shutdown()
{
	printf("Writing file...\n");
	WriteList();
	printf("Write complete\n");
}
bool ChannelList::SearchItem(std::string UID, uint64 channel_id)
{
	if (active_server_.compare(UID) != 0)
		if (!LoadList(UID))
			return false;

	return InActiveList(channel_id);
}
#ifdef _WIN32
#include <windows.h>
#define SYSERROR()  GetLastError()
#else
#include <errno.h>
#define SYSERROR()  errno
#endif

bool ChannelList::LoadList(std::string UID)
{
	QLog t("Loading " + UID + " list...", "LoadList");
	if(!active_server_.empty())
		WriteList();
	active_server_ = UID;
	notify_channel_list_.clear();

	std::fstream file;
	std::string line;
	file.open(active_server_ + ".txt");
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			uint64 pullint;
			try {
				pullint = std::stoi(line);
			}
			catch (int e)
			{

			}
			notify_channel_list_.push_back(pullint);
		}
		file.close();
	}
	else
	{
		//std::string(get_path()) + 
		log->Debug("No file found or opened, creating new file for server UID");
		std::ofstream outfile(UID + ".txt");
		if (!file.is_open())
		{
			log->Error("New file failed to be created for " + UID + "...");
			std::cerr << "Failed to open file : " << SYSERROR() << std::endl;
		}
		outfile.close();
	}
	return true;
}

void ChannelList::WriteList()
{
	QLog t("Writing " + active_server_ + " list...", "WriteList");
	if (active_server_.empty() || notify_channel_list_.empty())
		return;

	std::ofstream file(std::string(get_path()) + "channel_watchdog/" + active_server_ + ".txt");
	if (file.is_open())
	{
		for (uint64 ch_id : notify_channel_list_)
		{
			printf("Pushing CH_ID: %i\n", (int)ch_id);
			file << ch_id << std::endl;

		}
		file.flush();
		file.close();
	}
	else
		log->Error("File failed to open");
}

/* Adds item to list if not already added (Returns 0 for success, 1 for already exists in list) */
int ChannelList::AddItem(std::string UID, uint64 channel_id)
{
	log->Debug("Adding " + std::to_string(channel_id) + " from " + UID);
	if (active_server_.compare(UID) == 0)
	{
		if (InActiveList(channel_id))
			return 1;
	}
	else
	{
		LoadList(UID);
		if (InActiveList(channel_id))
			return 1;
	}
	notify_channel_list_.push_back(channel_id);
	return 0;
}
/* Removes item from list if it exists (Returns 0 for success, -1 for failure) */
int ChannelList::RemoveItem(std::string UID, uint64 channel_id)
{
	log->Debug("Removing " + std::to_string(channel_id) + " from " + UID);
	if (active_server_.compare(UID) == 0)
	{
		if (!InActiveList(channel_id))
			return -1;
	}
	else
		LoadList(UID);

	notify_channel_list_.remove(channel_id);
	return 0;
}

bool ChannelList::InActiveList(uint64 channel_id)
{
	for (uint64 ch_id : notify_channel_list_)
	{
		if (channel_id == ch_id)
			return true;
	}
	return false;
}
