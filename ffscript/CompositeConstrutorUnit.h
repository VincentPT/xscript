/******************************************************************
* File:        CompositeConstrutorUnit.h
* Description: declare CompositeConstrutorUnit class. A command
*              builder for expression unit. It is specialized for
*              composite constructor unit.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ExpresionParser.h"


namespace ffscript {
	class CompositeConstrutorUnit :
		public Function
	{
		list<pair<Variable*, ExecutableUnitRef>> _assigments;
	public:
		CompositeConstrutorUnit();
		CompositeConstrutorUnit(const list<pair<Variable*, ExecutableUnitRef>>& assigments);
		virtual ~CompositeConstrutorUnit();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
		virtual int getChildCount();

		void setAssigments(list<pair<Variable*, ExecutableUnitRef>>& assigments);
		list<pair<Variable*, ExecutableUnitRef>>& getAssigments();
	};
}