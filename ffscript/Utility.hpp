/******************************************************************
* File:        ScopeAutoFunction.hpp
* Description: define ScopeAutoFunction class. A class alows invoke
*              a specific function when an its instance is destroyed.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <functional>

class ScopeAutoFunction {
	std::function<void()> _fx;
public:

	ScopeAutoFunction(const std::function<void()>& fx) {
		_fx = fx;
	}

	ScopeAutoFunction(std::function<void()>&& fx) {
		_fx = fx;
	}

	~ScopeAutoFunction() {
		_fx();
	}
};