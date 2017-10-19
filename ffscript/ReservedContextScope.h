#pragma once

namespace ffscript {

	class Context;

	class ReservedContextScope
	{
		Context* _reservedContext;
	public:
		ReservedContextScope(Context* reservedContext);
		~ReservedContextScope();
	};
}
