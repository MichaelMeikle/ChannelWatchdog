#include "log.h"
#include "teamlog/logtypes.h"

int Log::log_id_ = 0;

Log::Log()
{
	log_level_ = LogLevel_INFO;
	if (log_id_ == NULL)
		log_id_ = 1;
}
void Log::Error(std::string msg)
{
	ts3handle()->logMessage(msg.c_str(), LogLevel_ERROR, LOG_NAME, log_id_);
	log_id_++;
	printf("[%s] %s\n", LOG_NAME, msg.c_str());
}
void Log::Warning(std::string msg)
{
	if (log_level_ < LogLevel_WARNING)
		return;
	ts3handle()->logMessage(msg.c_str(), LogLevel_WARNING, LOG_NAME, log_id_);
	log_id_++;
	printf("[%s] %s\n", LOG_NAME, msg.c_str());
} 
void Log::Debug(std::string msg)
{
	if (log_level_ < LogLevel_DEBUG)
		return;
	ts3handle()->logMessage(msg.c_str(), LogLevel_DEBUG, LOG_NAME, log_id_);
	log_id_++;
	printf("[%s] %s\n", LOG_NAME, msg.c_str());
}
void Log::Info(std::string msg)
{
	if (log_level_ < LogLevel_INFO)
		return;
	ts3handle()->logMessage(msg.c_str(), LogLevel_INFO, LOG_NAME, log_id_);
	log_id_++;
	printf("[%s] %s\n", LOG_NAME, msg.c_str());
}
void Log::PrintMsg(uint64 serverConnectionHandlerID, std::string msg)
{
	ts3handle()->printMessage(serverConnectionHandlerID, msg.c_str(), PLUGIN_MESSAGE_TARGET_CHANNEL);
}