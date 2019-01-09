/******************************************************************
* File:        Context.h
* Description: declare Context class. A class that supplied memory
*              and a mechanism to run the code.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "SingleList.h"
#include "FFStack.h"
class DFunction;

#define THROW_EXCEPTION_ON_ERROR
#define REDUCE_SCOPE_ALLOCATING_MEM

namespace ffscript {

	class ScopeRuntimeData;

	struct ContextInfo {
		CommandPointer _command;
		ScopeRuntimeData* _scopeData;
	};

	//typedef SingleList<unsigned int> ScopeAllocatedStack;
	//typedef SingleList<ContextInfo> ContextStack;

	typedef FFStack<unsigned int, 4096> ScopeAllocatedStack;
	typedef FFStack<ContextInfo, 4096> ContextStack;

	class Context
	{
		unsigned char* _threadData;
		const unsigned int _dataSize;

		unsigned int _currentOffset;
		bool _allocatedBuffer;
		bool _isError;
		CommandPointer _currentCommand;
		CommandPointer _beforeJump;
		CommandPointer _endCommand;
		ScopeAllocatedStack _allocatedStack;
#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		ScopeAllocatedStack _scopeCodeSize;
#endif
		ContextStack _contextStack;
	public:
		Context(unsigned char* threadData, unsigned int bufferSize);
		Context(unsigned int stackSize);
		virtual ~Context();		
		//int getCurrentOffset() const;
		inline int getCurrentOffset() const { return _currentOffset; }
		int getCurrentScopeSize() const;
		unsigned int getTotalAllocatedSize() const;
		int getMemCapacity() const;
		bool isError() const;
		void moveOffset(int size);
		void pushScope();
		void scopeAllocate(unsigned int dataSize, unsigned int codeSize);
		void scopeUnallocate(unsigned int dataSize, unsigned int codeSize);
		void popScope();
		void pushContext(unsigned int scopeParam);
		void popContext();
		ScopeRuntimeData* getScopeRuntimeData() const;
		void write(unsigned int offset, const void* data, unsigned int size);
		void read(unsigned int offset, void* data, unsigned int size);
		void lea(unsigned int offset, void* value);
		bool prepareWrite(unsigned int offset, unsigned int size);
		inline void* getAbsoluteAddress(unsigned int offset) { return (void*)(_threadData + offset); }
		CommandPointer getCurrentCommand() const;
		CommandPointer getEndCommand() const;
		void jump(CommandPointer commandPointer);
		void setCurrentCommand(CommandPointer commandPointer);
		void setEndCommand(CommandPointer endCommand);

		virtual void run();
		virtual void runFunctionScript();

		static Context* getCurrent();
		static void makeCurrent(Context* context);
	};
}

