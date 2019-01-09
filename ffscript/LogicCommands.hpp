/******************************************************************
* File:        LogicCommands.hpp
* Description: define some logic command template classes. Instances
*              of these classes are compiled code of logic commands
*              (operator &&, operator ||).
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "CommandTree.h"
#include <sstream>

namespace ffscript {
	template <class T>
	T PointerToVal(void* p) { return *((T*)p); }
	template <class T>
	T Pointer2ToVal(void* p) { return *(*((T**)p)); }

	template <class T1, class T2>
	class OptimizedLogicCommandT : public OptimizedLogicCommand {
	protected:
		T1(*fVal1)(void*);
		T2(*fVal2)(void*);
	public:
		OptimizedLogicCommandT(bool param1IsRef, bool param2IsRef) {
			if (param1IsRef) this->fVal1 = Pointer2ToVal<T1>;
			else this->fVal1 = PointerToVal<T1>;
			if (param2IsRef) this->fVal2 = Pointer2ToVal<T2>;
			else this->fVal2 = PointerToVal<T2>;
		}

		virtual ~OptimizedLogicCommandT() {}
	};
	////////////////////////////////////////////////////
	template <class T1, class T2>
	class LogicAndCommandT : public OptimizedLogicCommandT<T1, T2> {
	public:
		LogicAndCommandT(bool param1IsRef, bool param2IsRef) : OptimizedLogicCommandT<T1, T2>(param1IsRef, param2IsRef) {}
		virtual void execute() {
			this->_commandParam1->execute();

			Context* context = Context::getCurrent();
			int paramOffset = this->_commandParam1->getTargetOffset() + context->getCurrentOffset();
			int returnOffset = this->getTargetOffset() + context->getCurrentOffset();
			void* paramValueRef1 = context->getAbsoluteAddress(paramOffset);
			bool* resultValueRef = (bool*)context->getAbsoluteAddress(returnOffset);

			if (this->fVal1(paramValueRef1) == 0) {
				*resultValueRef = false;
			}
			else {
				this->_commandParam2->execute();
				paramOffset = this->_commandParam2->getTargetOffset() + context->getCurrentOffset();
				void* paramValueRef2 = context->getAbsoluteAddress(paramOffset);
				*resultValueRef = (this->fVal2(paramValueRef2) != 0);
			}
		}
		virtual void buildCommandText(std::list<std::string>& strCommands) {
			this->_commandParam1->buildCommandText(strCommands);
			this->_commandParam2->buildCommandText(strCommands);

			std::stringstream ss;
			ss << "and ([" << this->_commandParam1->getTargetOffset() << "], [" << this->_commandParam1->getTargetOffset() << "])";
			strCommands.emplace_back(ss.str());
		}
	};

	////////////////////////////////////////////////////
	template <class T1, class T2>
	class LogicOrCommandT : public OptimizedLogicCommandT<T1, T2> {
	public:
		LogicOrCommandT(bool param1IsRef, bool param2IsRef) : OptimizedLogicCommandT<T1, T2>(param1IsRef, param2IsRef) {}
		virtual void execute() {
			this->_commandParam1->execute();

			Context* context = Context::getCurrent();
			int paramOffset = this->_commandParam1->getTargetOffset() + context->getCurrentOffset();
			int returnOffset = this->getTargetOffset() + context->getCurrentOffset();
			void* paramValueRef1 = (T1*)context->getAbsoluteAddress(paramOffset);
			bool* resultValueRef = (bool*)context->getAbsoluteAddress(returnOffset);

			if (this->fVal1(paramValueRef1)) {
				*resultValueRef = true;
			}
			else {
				this->_commandParam2->execute();
				paramOffset = this->_commandParam2->getTargetOffset() + context->getCurrentOffset();
				void* paramValueRef2 = context->getAbsoluteAddress(paramOffset);
				*resultValueRef = (this->fVal1(paramValueRef2) != 0);
			}
		}

		virtual void buildCommandText(std::list<std::string>& strCommands) {
			this->_commandParam1->buildCommandText(strCommands);
			this->_commandParam2->buildCommandText(strCommands);

			std::stringstream ss;
			ss << "or ([" << this->_commandParam1->getTargetOffset() << "], [" << this->_commandParam1->getTargetOffset() << "])";
			strCommands.emplace_back(ss.str());
		}
	};
}