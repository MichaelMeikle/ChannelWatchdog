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
	ts3handle()->logMessage(msg.c_str(), LogLevel_ERROR, ts3plugin_name(), log_id_);
	log_id_++;
}
void Log::Warning(std::string msg)
{
	if (log_level_ < LogLevel_WARNING)
		return;
	ts3handle()->logMessage(msg.c_str(), LogLevel_WARNING, ts3plugin_name(), log_id_);
	log_id_++;
} 
void Log::Debug(std::string msg)
{
	if (log_level_ < LogLevel_DEBUG)
		return;
	ts3handle()->logMessage(msg.c_str(), LogLevel_DEBUG, ts3plugin_name(), log_id_);
	log_id_++;
}
void Log::Info(std::string msg)
{
	if (log_level_ < LogLevel_INFO)
		return;
	ts3handle()->logMessage(msg.c_str(), LogLevel_INFO, ts3plugin_name(), log_id_);
	log_id_++;
}