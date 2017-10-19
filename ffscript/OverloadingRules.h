#pragma once
#include "expressionunit.h"

namespace ffscript {
	class OverloadingRulesBase
	{
	public:
		OverloadingRulesBase();
		virtual ~OverloadingRulesBase();
	};

	class OverloadingRules1 : public OverloadingRulesBase {
	public:
		OverloadingRules1();
		virtual ~OverloadingRules1();
	};
}
