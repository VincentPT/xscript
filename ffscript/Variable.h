/******************************************************************
* File:        Variable.h
* Description: declare Variable and MemberVariable classes.
*              Variable is a class used to store information of a
*              variable in the script.
*              MemberVariable is a class that store information of
*              a member of a variable has type is a structure.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <string>
#include <list>
#include <memory>
#include "ffscript.h"
#include "ScriptType.h"

namespace ffscript {

	class ScriptScope;

	enum class VariableGroupType : char {
		InScope = 0,
		FuntionParameter,
	};

	class Variable
	{
		ScriptType _type;
		VariableGroupType _groupType;
		int _offset;
		ScriptScope* _ownerScope;
		std::string _name;
		std::list<std::shared_ptr<Variable>> _copies;
	public:
		Variable(const std::string& name);
		virtual ~Variable();
		const ScriptType& getDataType() const;
		void setDataType(const ScriptType& type);
		void setGroupType(VariableGroupType groupType);
		VariableGroupType getGroupType() const;
		const std::string& getName() const;
		void setName(const std::string& name);
		virtual int getOffset() const;
		void setOffset(int offset);
		ScriptScope* getScope() const;
		void setScope(ScriptScope* ownerScope);
		virtual Variable* clone(bool keepManaged = true);
		int getSize() const;
	};

	class MemberVariable : public Variable {
		Variable* _parent;
	public:
		MemberVariable(Variable* parent, const std::string& name);
		virtual ~MemberVariable();

		void setParent(Variable* parent);
		Variable* getParent() const;
		int getOffset() const;
	};
}