#pragma once
#include "Logger.h"

#ifndef UINT_PTR
#if defined(_WIN64)
typedef unsigned __int64 UINT_PTR;
#else
typedef unsigned int UINT_PTR;
#endif
#endif // !UINT_PTR


namespace RemoteLogger {
	class RLogger :
		public Logger
	{
	protected:
		UINT_PTR _socket;
	public:
		RLogger(const char* hostName, int port);
		virtual ~RLogger();
		void pushLog(const std::string& message);
	};

	void pushLog(const std::string& message);
	void pushLogWithThreadSpecific(const std::string& message);
}
