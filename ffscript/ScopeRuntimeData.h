/******************************************************************
* File:        ScopeRuntimeData.h
* Description: declare ScopeRuntimeData and its derived classes.
*              A scope runtime data object contains information
*              of current scope in a context. It created at run time
*              when the command pointer enter an scope and destroyed
*              when the command pointer leave current scope.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <vector>
namespace ffscript {
	class ScopeRuntimeData
	{		
	protected:
		unsigned char* _executedConstructor;
		ScopeRuntimeData();
	public:
		static ScopeRuntimeData* createRuntimeData(int scopeContructorCount);
		virtual ~ScopeRuntimeData();
		unsigned char isContructorExecuted(int index);
		void markContructorExecuted(int index);
		void markContructorNotExecuted(int index);
	};

	class ScopeRuntimeDataFixSize : public ScopeRuntimeData
	{
		unsigned char _data;
	public:
		ScopeRuntimeDataFixSize();
		virtual ~ScopeRuntimeDataFixSize();
	};

	class ScopeRuntimeDataDynamicSize : public ScopeRuntimeData
	{
		unsigned char *_data;
	public:
		ScopeRuntimeDataDynamicSize(int size);
		virtual ~ScopeRuntimeDataDynamicSize();
	};
}
