/******************************************************************
* File:        Context.cpp
* Description: implement Context class. A class that supplied memory
*              and a mechanism to run the code.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "Context.h"
#include <stdlib.h>
#include "InstructionCommand.h"
#include "ScopeRuntimeData.h"

#include <iomanip>
#include <sstream>

#ifdef THROW_EXCEPTION_ON_ERROR
#include <exception>

#define RAISE_STACK_OVERFLOW_ERROR() throw std::runtime_error("stack is overflow")
#define RAISE_ESP_MISMATCH_ERROR() throw std::runtime_error("function calling is mismatch")
#else
#define RAISE_STACK_OVERFLOW_ERROR() _isError = false
#define RAISE_ESP_MISMATCH_ERROR() _isError = false
#endif

namespace ffscript {
	
#if _WIN32 || _WIN64
	__declspec(thread) Context* _threadContext = nullptr;
// Check GCC
#elif __GNUC__
	__thread Context* _threadContext = nullptr;
#endif

	void RaiseStackOverflow() {
		RAISE_STACK_OVERFLOW_ERROR();
	}

	Context::Context(unsigned int stackSize) :
		_dataSize(stackSize),
		_currentOffset(0),
		_currentCommand(nullptr),
		_endCommand(nullptr),
		_beforeJump(nullptr),
		_allocatedStack(RaiseStackOverflow),
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		_scopeCodeSize(RaiseStackOverflow),
#endif
		_contextStack(RaiseStackOverflow)
	{
		Context::makeCurrent(this);
		_threadData = (unsigned char*)malloc(_dataSize);

		_allocatedBuffer = true;
		_isError = false;
		_allocatedStack.push_front(0);
	}

	Context::Context(unsigned char* threadData, unsigned int bufferSize) :
		_dataSize(bufferSize), _threadData(threadData), _currentOffset(0), _allocatedBuffer(false),
		_currentCommand(nullptr), _endCommand(nullptr),
		_allocatedStack(RaiseStackOverflow),
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		_scopeCodeSize(RaiseStackOverflow),
#endif
		_contextStack(RaiseStackOverflow)
	{
		Context::makeCurrent(this);
		_isError = false;
		_allocatedStack.push_front(0);
	}

	Context::~Context()
	{
		_threadContext = nullptr;
		if (_allocatedBuffer) {
			free(_threadData);
		}
		_threadData = nullptr;
	}

	Context* Context::getCurrent() {
		return _threadContext;
	}

	void Context::makeCurrent(Context* context) {
		_threadContext = context;
	}

	bool Context::isError() const {
		return _isError;
	}

	void Context::moveOffset(int size) {
		if (_currentOffset + size > _dataSize) {
			RAISE_STACK_OVERFLOW_ERROR();
			return;
		}
		_currentOffset += size;
	}

	void Context::pushScope() {
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		_allocatedStack.front() += _scopeCodeSize.front();
#endif
		_currentOffset += _allocatedStack.front();
		_allocatedStack.push_front(0);
	}

	void Context::popScope() {
		_allocatedStack.pop_front();
		_currentOffset -= _allocatedStack.front();
        _allocatedStack.front() -= _scopeCodeSize.front();
	}

	void Context::scopeAllocate(unsigned int scopeDataSize, unsigned int scopeCodeSize) {
		_allocatedStack.front() += scopeDataSize;
#ifndef REDUCE_SCOPE_ALLOCATING_MEM
		_allocatedStack.front() += scopeCodeSize;
#endif
		if (_currentOffset  + 
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
			scopeCodeSize +
#endif
			_allocatedStack.front() > _dataSize) {
			RAISE_STACK_OVERFLOW_ERROR();
			return;
		}
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		_scopeCodeSize.push_front(scopeCodeSize);
#endif
	}

	void Context::scopeUnallocate(unsigned int scopeDataSize, unsigned int scopeCodeSize) {
#ifndef REDUCE_SCOPE_ALLOCATING_MEM
		scopeDataSize += scopeDataSize;
#endif
		if (_allocatedStack.front() < scopeDataSize) {
			_allocatedStack.front() = 0;
		}
		else {
			_allocatedStack.front() -= scopeDataSize;
		}
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		_scopeCodeSize.pop_front();
#endif
	}	

	void Context::pushContext(unsigned int scopeParam) {
		ScopeRuntimeData* scopeData = ScopeRuntimeData::createRuntimeData(scopeParam);
		_contextStack.push_front({_beforeJump, scopeData });
	}
	
	void Context::popContext() {
		auto& contextInfo = _contextStack.front();
		_currentCommand = contextInfo._command;
		if (contextInfo._scopeData) {
			delete contextInfo._scopeData;
		}

		_contextStack.pop_front();
	}
	
	ScopeRuntimeData* Context::getScopeRuntimeData() const {
		return _contextStack.front()._scopeData;
	}
	

	int Context::getCurrentScopeSize() const {
		return (int)_allocatedStack.front();
	}

	unsigned int Context::getTotalAllocatedSize() const {
		unsigned int size = 0;
		for (auto it = _allocatedStack.begin(); it != _allocatedStack.end(); it++) {
			size += *it;
		}

		return size;
	}

	int Context::getMemCapacity() const {
		return _dataSize;
	}

	void Context::write(unsigned int offset, const void* data, unsigned int size) {
		if ( offset + size > _dataSize) {
			RAISE_STACK_OVERFLOW_ERROR();
			return;
		}
		void* target = getAbsoluteAddress(offset);
		memcpy_s(target, _dataSize - offset, data, size);		
	}

	bool Context::prepareWrite(unsigned int offset, unsigned int size) {
		if (offset + size > _dataSize) {
			RAISE_STACK_OVERFLOW_ERROR();
			return false;
		}
		return true;
	}

	void Context::read(unsigned int offset, void* data, unsigned int size) {
		if (offset + size > _dataSize) {
			RAISE_STACK_OVERFLOW_ERROR();
			return;
		}
		void* source = getAbsoluteAddress(offset);
		memcpy_s(data, size, source, size);
	}

	void Context::lea(unsigned int offset, void* value) {
		//if (offset + sizeof(value) > _dataSize) return -1;
		size_t* pointer = (size_t*)getAbsoluteAddress(offset);
		*pointer = (size_t) value;
	}
	
	CommandPointer Context::getCurrentCommand() const {
		return _currentCommand;
	}

	CommandPointer Context::getEndCommand() const {
		return _endCommand;
	}

	void Context::jump(CommandPointer commandPointer) {
		_beforeJump = _currentCommand;
		_currentCommand = commandPointer;
	}

	void Context::setCurrentCommand(CommandPointer commandPointer) {
		if (_beforeJump == nullptr) {
			_beforeJump = commandPointer;
		}
		_currentCommand = commandPointer;
	}


	void Context::setEndCommand(CommandPointer endCommand) {
		_endCommand = endCommand;
	}

	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}

	void Context::runFunctionScript() {
#ifndef THROW_EXCEPTION_ON_ERROR
		if (_isError) return;
#endif
		int stackLevel = _allocatedStack.getSize();
		while (_currentCommand != _endCommand) {
			//const std::string& commandText = (*_currentCommand)->toString();
			//Logger::WriteMessage((int_to_hex((size_t)_currentCommand) + " " + commandText).c_str());
			(*_currentCommand)->execute();

			if (_allocatedStack.getSize() != stackLevel
#ifndef THROW_EXCEPTION_ON_ERROR
				|| _isError
#endif 
				)
			{
				break;
			}
			_currentCommand++;
		}
	}

	void Context::run() {
		if (_currentCommand
#ifndef THROW_EXCEPTION_ON_ERROR
			&& !_isError
#endif
			) {
			while (_currentCommand != _endCommand) {
				//const std::string& commandText = (*_currentCommand)->toString();
				//Logger::WriteMessage((int_to_hex((size_t)_currentCommand) + " " + commandText).c_str());
				(*_currentCommand)->execute();
#ifndef THROW_EXCEPTION_ON_ERROR
				if (_isError) {
					break;
				}
#endif
				_currentCommand++;
			}
		}
	}
}
