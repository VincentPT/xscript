#pragma once
#include <string>
namespace RemoteLogger {
	class Logger
	{
	public:
		Logger();
		virtual ~Logger();
		virtual void pushLog(const std::string& message) = 0;
	};
}