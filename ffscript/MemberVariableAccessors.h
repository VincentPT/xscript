/******************************************************************
* File:        MemberVariableAccessors.h
* Description: declare member variable accessor classes. Each member
*              variable accessor has its own data and behavior to
*              access its data base on parent's address.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
namespace ffscript {
	class MemberVariableAccessor
	{
	public:
		MemberVariableAccessor();
		virtual ~MemberVariableAccessor();
		virtual void* access(void* address) = 0;
	};

	class MVContextAccessor : public MemberVariableAccessor {
	public:
		void* access(void* address);
	};

	class MVGlobalAccessor : public MemberVariableAccessor {
	public:
		void* _address;
	public:
		MVGlobalAccessor(void* address);
		void* access(void* address);
	};

	class MVOffsetAccessor : public MemberVariableAccessor {
	public:
		int _offset;
	public:
		MVOffsetAccessor(int offset);
		void* access(void* address);		
	};

	class MVPointerAccessor : public MemberVariableAccessor {
	public:
		void* access(void* address);
	};
}

