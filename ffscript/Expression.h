#pragma once
#include "ffscript.h"
#include "expressionunit.h"
#include <string>

namespace ffscript {

	class FFSCRIPT_API Expression
	{
		ExecutableUnitRef _rootUnit;
		std::wstring _expressionString;
	public:
		Expression(ExecutableUnitRef rootUnit);
		virtual ~Expression();

		void setExpString(const std::wstring& exp);
		const std::wstring& getExpString();
		const ExecutableUnitRef& getRoot() const;
		ExecutableUnitRef& getRoot();
	};

	typedef shared_ptr<Expression> ExpressionRef;
}

