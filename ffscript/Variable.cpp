#include "stdafx.h"
#include "Variable.h"
#include "ScriptCompiler.h"
#include "ScriptScope.h"


namespace ffscript {
	
	Variable::Variable(const std::string& name) :
		_name(name),
		_offset(0),
		_ownerScope(nullptr),
		_groupType(VariableGroupType::InScope)
	{		
	}

	Variable::~Variable() {}

	const ScriptType& Variable::getDataType()  const { return _type; }

	void Variable::setDataType(const ScriptType& type) { _type = type;}

	void Variable::setGroupType(VariableGroupType groupType) {
		_groupType = groupType;
	}

	VariableGroupType Variable::getGroupType() const {
		return _groupType;
	}

	const std::string& Variable::getName()  const { return _name; }

	int Variable::getOffset() const { return _offset; }
	void Variable::setOffset(int offset) {
		_offset = offset;
		for (auto it = _children.begin(); it != _children.end(); it++) {
			it->get()->setOffset(offset);
		}
	}
	ScriptScope* Variable::getScope() const {
		return _ownerScope;
	}

	void Variable::setScope(ScriptScope* ownerScope) {
		_ownerScope = ownerScope;
	}

	Variable* Variable::clone() {
		Variable* child = new Variable(_name);
		child->_offset = _offset;
		child->_type = _type;
		child->_ownerScope = _ownerScope;

		_children.push_back(std::shared_ptr<Variable>(child));

		return child;
	}

	int Variable::getSize() const {
		if (_groupType == VariableGroupType::FuntionParameter) {
			return _ownerScope->getCompiler()->getTypeSizeInStack(_type.iType());
		}
		return _ownerScope->getCompiler()->getTypeSize(_type);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MemberVariable::MemberVariable(Variable* parent, const std::string& name) : Variable(name), _parent(parent) {}

	MemberVariable::~MemberVariable() {}

	void MemberVariable::setParent(Variable* parent) {
		_parent = parent;
	}

	Variable* MemberVariable::getParent() const {
		return _parent;
	}
}