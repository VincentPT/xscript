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

	void Variable::setName(const std::string& name) {
		_name = name;
	}

	int Variable::getOffset() const { return _offset; }
	void Variable::setOffset(int offset) {
		_offset = offset;
		for (auto it = _copies.begin(); it != _copies.end(); it++) {
			auto& copyRef = *it;
			copyRef->setOffset(offset);
		}
	}
	ScriptScope* Variable::getScope() const {
		return _ownerScope;
	}

	void Variable::setScope(ScriptScope* ownerScope) {
		_ownerScope = ownerScope;
	}

	Variable* Variable::clone() {
		Variable* aCopy = new Variable(_name);
		aCopy->_offset = _offset;
		aCopy->_type = _type;
		aCopy->_ownerScope = _ownerScope;

		_copies.push_back(std::shared_ptr<Variable>(aCopy));

		return aCopy;
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

	int MemberVariable::getOffset() const {
		return Variable::getOffset() + _parent->getOffset();
	}
}