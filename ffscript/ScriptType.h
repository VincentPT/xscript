/******************************************************************
* File:        ScriptType.h
* Description: declare ScriptType class. A class is used to store
*              information of a type of the script.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <string>
#include <memory>

namespace ffscript {
	class ScriptCompiler;

	class ScriptType
	{
		int _iType;
		std::string _sType;
	public:
		ScriptType();
		ScriptType(int iType, const std::string& sType);
		virtual ~ScriptType();
		bool operator ==(const ScriptType&) const;
		bool operator !=(const ScriptType&) const;
		bool isRefType() const;
		bool isSemiRefType() const;
		bool isFunctionType() const;
		bool isUnkownType() const;
		int refLevel() const;
		int semiRefLevel() const;
		ScriptType makeRef() const;
		ScriptType makeSemiRef() const;
		ScriptType deRef() const;
		ScriptType deSemiRef() const;
		int origin() const;
		void updateType(ScriptCompiler* scriptCompiler);
		void updateType(ScriptCompiler* scriptCompiler, const std::string& type);
		const std::string& sType() const;
		int iType() const;
		void setType(int iType);
		void setTypeStr(const std::string& sType);

		static ScriptType parseType(ScriptCompiler* scriptCompiler, const std::string& sType);
		static int stringTypeToInt(ScriptCompiler* scriptCompiler, const std::string& sType);
		static ScriptType buildScriptType(ScriptCompiler* scriptCompiler, int iType, char refLevel);
	};

	typedef std::shared_ptr<ScriptType> ScriptTypeRef;
}
