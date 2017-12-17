#pragma once
#include "spy_interfaces.h"
#include "spylib_interfaces.h"

ReturnData readUnitList(void* objectAddres);
ReturnData readUnitNode(void* objectAddres);
ReturnData readCommandList(void* objectAddres);
ReturnData readProgramCommand(void* objectAddres);