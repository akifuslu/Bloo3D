#pragma once

#include "spdlog/spdlog.h"


#define LOG_ENABLED 1

#if LOG_ENABLED
#define LOG(x) spdlog::info(x);
#define LOGDEBUG(x) spdlog::debug(x);
#define LOGWARN(x) spdlog::warn(x);
#define LOGERROR(x) spdlog::error(x);
#define LOGCRIT(x) spdlog::critical(x);
#else
#define LOG(x)  
#define LOGDEBUG(x) 
#define LOGWARN(x) 
#define LOGERROR(x) 
#define LOGCRIT(x) 
#endif