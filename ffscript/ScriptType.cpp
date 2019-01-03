/******************************************************************
* File:        ScriptType.cpp
* Description: implement ScriptType class. A class is used to store
*              information of a type of the script.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "ScriptType.h"
#include "ffscript.h"
#include "unitType.h"
#include "ScriptCompiler.h"
#include "Internal.h"
#include "Utils.h"

using namespace std;

namespace ffscript {
	ScriptType::ScriptType() : ScriptType(DATA_TYPE_UNKNOWN, DATA_TYPE_UNKNOWN_INSTRING)
	{
	}

	ScriptType::ScriptType(int iType, const std::string& sType) : _iType(iType), _sType(sType) {
	}


	ScriptType::~ScriptType()
	{
	}

	bool ScriptType::operator ==(const ScriptType& type) const {
		return (type._iType == _iType && type._sType == _sType);
	}

	bool ScriptType::operator !=(const ScriptType& type) const {
		return (type._iType != _iType || type._sType != _sType);
	}

	bool ScriptType::isRefType() const {
		return (_iType & DATA_TYPE_POINTER_MASK) != 0;
	}

	bool ScriptType::isSemiRefType() const {
		return (_iType & DATA_TYPE_REF_MASK) != 0;
	}

	bool ScriptType::isFunctionType() const {
		return (_iType & DATA_TYPE_FUNCTION_MASK) != 0;
	}

	bool ScriptType::isUnkownType() const {
		return IS_UNKNOWN_TYPE(_iType);
	}
	
	int ScriptType::refLevel() const {
		int iLevel = 0;
		if (isRefType()) {
			const char* s = _sType.c_str();
			const int size = sizeof(POINTER_SIGN " ") - 1;
			auto end = s + _sType.size();

			while (s < end) {
				s = trimLeft(s, end);
				if (strncmp(s, POINTER_SIGN " ", size) != 0) {
					break;
				}
				s += size;
				iLevel++;
			}
		}
		return iLevel;
	}

	int ScriptType::semiRefLevel() const {
		int iLevel = 0;
		
		const char* s = _sType.c_str();
		const char* c = s + _sType.size();
		char temp[] = SEMIREF_SIGN;

		while (c >= s) {
			c = trimRight(s, c);
			if (c < s || *c != temp[0]) break;

			iLevel++;
		}
		return iLevel;
	}
	
	ScriptType ScriptType::makeRef() const {
		return ScriptType(_iType | DATA_TYPE_POINTER_MASK, POINTER_SIGN " " + _sType);
	}

	ScriptType ScriptType::makeSemiRef() const {
		return ScriptType(_iType | DATA_TYPE_REF_MASK, _sType + "&");
	}

	ScriptType ScriptType::deRef() const {
		const char* s = _sType.c_str();
		const int size = sizeof(POINTER_SIGN " ") - 1;
		const char* r;
		if (r = strstr(s, POINTER_SIGN " ")) {
			s = r + size;
		}

		int newType;
		string newTypeS = s;
		if ( (r = strstr(s, POINTER_SIGN " ")) == nullptr) {
			newType = (_iType & ~DATA_TYPE_POINTER_MASK);
		}
		else {
			newType = _iType;
		}

		return ScriptType(newType, newTypeS);
	}

	ScriptType ScriptType::deSemiRef() const {
		const int size = sizeof(SEMIREF_SIGN) - 1;
		string newTypeS = _sType;
		auto posTemplate = newTypeS.find_last_of('>');
		auto semiRefPos = newTypeS.find_last_of(SEMIREF_SIGN);
		if (semiRefPos != string::npos && (posTemplate == string::npos || posTemplate < semiRefPos)) {
			newTypeS.erase(semiRefPos, size);
		}

		int newType = (~DATA_TYPE_REF_MASK) & _iType;

		return ScriptType(newType, newTypeS);
	}

	int ScriptType::origin() const {
		return (_iType & ~(DATA_TYPE_POINTER_MASK | DATA_TYPE_REF_MASK));
	}

	int ScriptType::stringTypeToInt(ScriptCompiler* scriptCompiler, const string& sType) {
		wstring wsType(sType.begin(), sType.end());
		auto s = wsType.c_str();
		auto end = s + wsType.size();
		ScriptType scriptType;
		scriptCompiler->readType(s, end, scriptType);

		return scriptType.iType();
	}

	ScriptType ScriptType::buildScriptType(ScriptCompiler* scriptCompiler, int iType, char refLevel) {
		ScriptType stype;
		stype.setType(iType);
		auto& typeStr = stype._sType;
		typeStr.clear();
		auto typeOriginStr = scriptCompiler->getType(stype.origin());
		while (refLevel > 0)
		{
			typeStr.append(POINTER_SIGN " ");
		}

		typeStr.append(typeOriginStr);

		if (stype.isSemiRefType()) {
			typeStr.push_back('&');
		}

		return stype;
	}

	void ScriptType::updateType(ScriptCompiler* scriptCompiler) {
		_iType = stringTypeToInt(scriptCompiler, _sType);
	}

	void ScriptType::updateType(ScriptCompiler* scriptCompiler, const std::string& sType) {
		_sType = sType;
		updateType(scriptCompiler);
	}

	const std::string& ScriptType::sType() const {
		return _sType;
	}

	int ScriptType::iType() const {
		return _iType;
	}

	void ScriptType::setType(int iType) {
		_iType = iType;
	}

	void ScriptType::setTypeStr(const std::string& sType) {
		_sType = sType;
	}

	ScriptType ScriptType::parseType(ScriptCompiler* scriptCompiler, const string& sType) {
		int iType = stringTypeToInt(scriptCompiler, sType);
		return ScriptType(iType, sType);
	}
}