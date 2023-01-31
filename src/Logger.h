#pragma once

#include "spdlog/spdlog.h"


#define LOG_ENABLED 1

#if LOG_ENABLED
#define LOG(...) spdlog::info(__VA_ARGS__);
#define LOGDEBUG(...) spdlog::debug(__VA_ARGS__);
#define LOGWARN(...) spdlog::warn(__VA_ARGS__);
#define LOGERROR(...) spdlog::error(__VA_ARGS__);
#define LOGCRIT(...) spdlog::critical(__VA_ARGS__);
#else
#define LOG(...)  
#define LOGDEBUG(...) 
#define LOGWARN(...) 
#define LOGERROR(...) 
#define LOGCRIT(...) 
#endif