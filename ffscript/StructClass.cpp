/******************************************************************
* File:        StructClass.cpp
* Description: implement StructClass class. A class used to store
*              information of struct of the C Lambda language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "StructClass.h"
#include "ScriptCompiler.h"

namespace ffscript {
	StructClass::StructClass(ScriptCompiler* scriptCompiler) : _scriptCompiler(scriptCompiler)
	{
	}
	StructClass::StructClass(ScriptCompiler* scriptCompiler, const std::string& name) : _scriptCompiler(scriptCompiler), _name(name)
	{
	}
	StructClass::~StructClass()
	{
	}

	int StructClass::getSize() const {
		int size = 0;
		for (auto& elm : _members) {
			size += _scriptCompiler->getTypeSize(elm.first);
		}

		return size;
	}
	void StructClass::setName(const std::string& name) {
		_name = name;
	}

	const std::string& StructClass::getName() const {
		return _name;
	}

	int StructClass::getMemberCount() const {
		return (int) _members.size();
	}

	void StructClass::addMember(const ScriptType& type, const std::string& memberName) {
		_members.push_back(std::make_pair(type, memberName));
	}

	bool StructClass::getInfo(const std::string& memberName, MemberInfo& info) const {
		int size = 0;
		for (auto& elm : _members) {
			if (memberName == elm.second) {
				info.offset = size;
				info.type = elm.first;
				return true;
			}
			size += _scriptCompiler->getTypeSize(elm.first);
		}

		return false;
	}

	void StructClass::retreiveMemberInfo(std::string* memberName, MemberInfo* info) const {
		if (memberName) {
			*memberName = _iterator->second;
		}
		if (info) {
			info->offset = _offset;
			info->type = _iterator->first;
		}

		_offset += _scriptCompiler->getTypeSize(_iterator->first);
		_iterator++;
	}

	bool StructClass::getMemberFirst(std::string* memberName, MemberInfo* info) const {
		_iterator = _members.begin();
		_offset = 0;
		if (_iterator == _members.end()) {
			return false;
		}
		retreiveMemberInfo(memberName,info);
		return true;
	}

	bool StructClass::getMemberNext(std::string* memberName, MemberInfo* info) const {
		if (_iterator == _members.end()) {
			return false;
		}

		retreiveMemberInfo(memberName, info);
		return true;
	}
}