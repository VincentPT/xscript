#pragma once
#include <string>
#include <list>
#include <memory>
#include "ScriptType.h"

namespace ffscript {
	struct MemberInfo {
		ScriptType type;
		int offset;
	};

	class ScriptCompiler;

	class StructClass
	{
		std::string _name;
		std::list<std::pair<ScriptType, std::string>> _members;
		mutable decltype(_members)::const_iterator _iterator;
		mutable int _offset;
		ScriptCompiler* _scriptCompiler;
	public:
		StructClass(ScriptCompiler* scriptCompiler);
		StructClass(ScriptCompiler* scriptCompiler, const std::string& name);
		virtual ~StructClass();

		int getSize() const;
		void setName(const std::string& name);
		const std::string& getName() const;
		int getMemberCount() const;
		void addMember(const ScriptType& type, const std::string& memberName);
		bool getInfo(const std::string& memberName, MemberInfo& info) const;
		bool getMemberFirst(std::string* memberName, MemberInfo* info) const;
		bool getMemberNext(std::string* memberName, MemberInfo* info) const;

	protected:
		void retreiveMemberInfo(std::string* memberName, MemberInfo* info) const;

	};

	typedef std::shared_ptr<StructClass> StructClassRef;
}
