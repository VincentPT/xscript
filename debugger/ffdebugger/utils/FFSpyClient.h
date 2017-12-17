#pragma once
#include "SpyClient.h"
#include "spylib_interfaces.h"

#include <Windows.h>
#include <functional>

class FFSpyClient : public SpyClient {
	CustomCommandId _predefinedBase;
	void* _checkFunctionPtrs[(int)UserCommandId::PRE_DEFINED_COMMAND_COUNT];
	ModuleId _hSpyLib;
protected:
	void setPredefinedCommandIdBase(int predefinedBase);
	bool loadSpyLib();
	void unloadSpyLib();
public:
	FFSpyClient();
	~FFSpyClient();

	bool startMonitorProcess(const char* processName);
	bool stopMonitorProcess();
	bool restartMonitorProcess();
	bool checkCommandsReady();
	
	int readUnitList(void* address, std::list<std::string>& unitNames);
	int readUnitNode(void* address, std::string& unitNodeJS);
	int readProgramCommands(void* address, std::list<std::string>& commands);
	int readCommands(void* address, std::list<std::string>& commands);
};