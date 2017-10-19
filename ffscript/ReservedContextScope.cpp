#include "stdafx.h"
#include "ReservedContextScope.h"
#include "Context.h"

namespace ffscript {
	ReservedContextScope::ReservedContextScope(Context* reservedContext) : _reservedContext(reservedContext){}

	ReservedContextScope::~ReservedContextScope() {
		Context::makeCurrent(_reservedContext);
	}
}
