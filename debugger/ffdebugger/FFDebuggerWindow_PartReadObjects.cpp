#include "FFDebuggerWindow.h"
#include "spy_interfaces.h"

#include "drawobjs/FFPoint.h"
#include "drawobjs/FFRect.h"
#include "drawobjs/FFPolygon.h"
#include "drawobjs/FFContainer.h"

#include <iostream>

using namespace std;
using namespace ci;
using namespace ci::app;

FFDrawObj* FFDebuggerWindow::readObject(void* desireReadObjectAddress, int type) {
	if (type < 0 || type >= (int)CUSTOM_COMMAND_END) {
		return nullptr;
	}

	if (!_spyClient) {
		_spyClient = std::make_shared<FFSpyClient>();
	}

	auto& currentMonitorProcessName = _spyClient->getProcessName();
	if (currentMonitorProcessName.empty()) {
		this->onStartMonitorProcess(nullptr);
	}

	bool res = _spyClient->checkCommandsReady();
	if (res == false) {
		cout << "Command cannot be executed, try restart the applications" << endl;
		return nullptr;
	}

	typedef FFDrawObj* (FFDebuggerWindow::*ReadObjectFunc) (void*);

	static ReadObjectFunc readObjectFuncs[] = {
		&FFDebuggerWindow::readUnitList,
		&FFDebuggerWindow::readUnitNode,
	};

	FFDrawObj* pNewObjectCreated = (this->*readObjectFuncs[type])(desireReadObjectAddress);

	// need update FBO on draw method because of new object created by above handlers
	needUpdate();

	return pNewObjectCreated;
}

FFDrawObj* FFDebuggerWindow::readUnitList(void* desireReadObjectAddress) {
	list<string> unitNames;
	_spyClient->readUnitList(desireReadObjectAddress, unitNames);

	cout << "unit list: ";
	for (auto it = unitNames.begin(); it != unitNames.end(); it++) {
		cout << *it << ", ";
	}

	return nullptr;
}

FFDrawObj* FFDebuggerWindow::readUnitNode(void* desireReadObjectAddress) {
	string unitNodeJS;
	_spyClient->readUnitNode(desireReadObjectAddress, unitNodeJS);

	cout << "Expression Unit Node:" << endl;
	cout << unitNodeJS << endl;

	return nullptr;
}