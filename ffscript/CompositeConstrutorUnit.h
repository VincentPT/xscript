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