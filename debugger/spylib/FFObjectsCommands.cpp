#include "FFObjectsCommands.h"
#include "spylib_interfaces.h"
#include "expressionunit.h"
#include <iostream>
#include <InstructionCommand.h>
#include <Program.h>

#include "json.hpp"
using json = nlohmann::json;

// using namespaces
using namespace std;
using namespace ffscript;

ReturnData readUnitList(void* objectAddres) {
	const std::list<ExpUnitRef>& expList = *(const std::list<ExpUnitRef>*)objectAddres;

	size_t bufferSize = 0;
	for (auto it = expList.begin(); it != expList.end(); it++) {
		bufferSize += (*it)->toString().size() + 1;
	}

	size_t rawSize = bufferSize + sizeof(StringBufferArray) - sizeof(StringBufferArray::data);
	StringBufferArray* rawData = (StringBufferArray*)malloc(rawSize);
	rawData->elmCount = (int)expList.size();
	rawData->size = (unsigned int)bufferSize;
	char* c = rawData->data;

	for (auto it = expList.begin(); it != expList.end(); it++) {
		auto& str = (*it)->toString();
		memcpy_s(c, str.size(), str.c_str(), str.size());

		c += str.size();
		*c = 0;
		c++;
	}

	ReturnData returnData;

	// set custom data to an allocated buffer, spy client should be responsible to free the memory after using
	returnData.customData = (char*)rawData;
	returnData.sizeOfCustomData = (int)rawSize;

	return returnData;
}

json convertToTree(ExpUnit* expUnit) {
	json unitNodeJS;	
		
	Function* pFunction = dynamic_cast<Function*>(expUnit);
	if (pFunction) {
		json children = json::array();
		int childCount = pFunction->getChildCount();
		for (int i = 0; i < childCount; i++) {
			auto& expUnitRef = pFunction->getChild(i);
			json childJS = convertToTree(expUnitRef.get());

			children.push_back(childJS);
		}
		unitNodeJS[expUnit->toString()] = children;
	}
	else {
		unitNodeJS = expUnit->toString();
	}

	return unitNodeJS;
}

ReturnData readUnitNode(void* objectAddres) {
	ExpUnit* expUnit = (ExpUnit*)objectAddres;
	string jsonStr;

	// convert to json tree
	json unitNodeJS = convertToTree(expUnit);
	if (unitNodeJS.is_object()) {
		// convert json tree to string
		stringstream ss;
		ss << std::setw(4) << unitNodeJS << std::endl;
		jsonStr = ss.str();
	}
	else {
		jsonStr = expUnit->toString();
	}	

	ReturnData returnData;
	returnData.sizeOfCustomData = (int)(jsonStr.size() + 1);
	// set custom data to an allocated buffer, spy client should be responsible to free the memory after using
	returnData.customData = (char*)malloc(returnData.sizeOfCustomData);
	returnData.customData[jsonStr.size()] = 0;

	memcpy_s(returnData.customData, returnData.sizeOfCustomData, jsonStr.c_str(), jsonStr.size());

	return returnData;
}

ReturnData readCommandList(CommandPointer cmdStart, CommandPointer cmdEnd) {
	list<string> commandTexts;	

	for (auto it = cmdStart; it != cmdEnd; it++) {
		(*it)->buildCommandText(commandTexts);
	}

	size_t bufferSize = 0;
	for (auto it = commandTexts.begin(); it != commandTexts.end(); it++) {
		bufferSize += it->size() + 1;
	}

	size_t rawSize = bufferSize + sizeof(StringBufferArray) - sizeof(StringBufferArray::data);
	StringBufferArray* rawData = (StringBufferArray*)malloc(rawSize);
	rawData->elmCount = (int)commandTexts.size();
	rawData->size = (unsigned int)bufferSize;
	char* c = rawData->data;

	for (auto it = commandTexts.begin(); it != commandTexts.end(); it++) {
		auto& str = *it;
		memcpy_s(c, str.size(), str.c_str(), str.size());

		c += str.size();
		*c = 0;
		c++;
	}

	ReturnData returnData;

	// set custom data to an allocated buffer, spy client should be responsible to free the memory after using
	returnData.customData = (char*)rawData;
	returnData.sizeOfCustomData = (int)rawSize;

	return returnData;
}

ReturnData readCommandList(void* objectAddres) {
	auto command = (InstructionCommand*) objectAddres;
	return readCommandList(&command, &command + 1);
}

ReturnData readProgramCommand(void* objectAddres) {
	auto program = (Program*)objectAddres;	

	return readCommandList(program->getFirstCommand(), program->getEndCommand());
}