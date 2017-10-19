#include "stdafx.h"
#include "remotelogger.h"
#include <thread>

#include <iomanip>
#include <chrono>
#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace RemoteLogger {

	static RLogger* g_LoggerInstance = nullptr;

	using namespace std;

	RLogger::RLogger(const char* hostName, int port) {
		struct addrinfo *result = NULL,
			*ptr = NULL,		
			hints;
		WSADATA wsaData;
		_socket = INVALID_SOCKET;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(hostName, std::to_string(port).c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return;
		}

		SOCKET connectSocket;
		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (connectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return;
			}

			// Connect to server.
			iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(connectSocket);
				connectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (connectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return;
		}

		_socket = connectSocket;
		g_LoggerInstance = this;

	}

	RLogger::~RLogger() {
		g_LoggerInstance = nullptr;
		if (_socket != INVALID_SOCKET) {

			// shutdown the connection since no more data will be sent
			int iResult = shutdown(_socket, SD_BOTH);
			if (iResult == SOCKET_ERROR) {
				printf("shutdown failed with error: %d\n", WSAGetLastError());
			}
			closesocket(_socket);
			WSACleanup();
		}
	}

	void RLogger::pushLog(const std::string& message) {
		if (_socket == INVALID_SOCKET) return;

		int iResult = send(_socket, message.c_str(), (int)message.size(), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(_socket);
			_socket = INVALID_SOCKET;
			WSACleanup();			
		}
	}

	void pushLog(const std::string& message) {
		if (g_LoggerInstance == nullptr) return;

		tm localTime;
		std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
		time_t now = std::chrono::system_clock::to_time_t(t);
		localtime_r(&now, &localTime);

		const std::chrono::duration<double> tse = t.time_since_epoch();
		int milliseconds = (std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000);

		//2016-06-18 14:24:51.994
		int year = 1900 + localTime.tm_year;
		int month = localTime.tm_mon + 1;
		
		string messageLog;
		messageLog.resize(25 + message.size() + 2);
		char* c = &messageLog[0];

		//begin
		*c++ = '[';
		//year
		*c++ = (year / 1000) + '0'; year %= 1000;
		*c++ = (year / 100) + '0'; year %= 100;
		*c++ = (year / 10) + '0'; year %= 10;
		*c++ = year + '0';
		*c++ = '-';
		//month
		*c++ = (month / 10) + '0'; month %= 10;
		*c++ = month + '0';
		*c++ = '-';
		//day
		*c++ = (localTime.tm_mday / 10) + '0'; localTime.tm_mday %= 10;
		*c++ = localTime.tm_mday + '0';
		*c++ = ' ';
		//hour
		*c++ = (localTime.tm_hour / 10) + '0'; localTime.tm_hour %= 10;
		*c++ = localTime.tm_hour + '0';
		*c++ = ':';
		//min
		*c++ = (localTime.tm_min / 10) + '0'; localTime.tm_min %= 10;
		*c++ = localTime.tm_min + '0';
		*c++ = ':';
		//sec
		*c++ = (localTime.tm_sec / 10) + '0'; localTime.tm_sec %= 10;
		*c++ = localTime.tm_sec + '0';
		*c++ = '.';
		//msec
		*c++ = (milliseconds / 100) + '0'; milliseconds %= 100;
		*c++ = (milliseconds / 10) + '0'; milliseconds %= 10;
		*c++ = milliseconds + '0';
		//end
		*c++ = ']';

		memcpy_s(c, messageLog.size() - 25, message.c_str(), message.size());
		c += message.size();
		*c++ = '\n';
		*c = 0;

		g_LoggerInstance->pushLog(messageLog);
	}

	void pushLogWithThreadSpecific(const std::string& message) {
		if (g_LoggerInstance == nullptr) return;

		DWORD threadId = GetCurrentThreadId();
		DWORD proccessId = GetCurrentProcessId();

		string messageLog;
		messageLog.append("[");
		messageLog.append(std::to_string(proccessId));
		messageLog.append("]");
		messageLog.append("[");
		messageLog.append(std::to_string(threadId));
		messageLog.append("]");
		messageLog.append(message);

		pushLog(messageLog);
	}
}
