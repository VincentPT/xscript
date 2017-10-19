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
