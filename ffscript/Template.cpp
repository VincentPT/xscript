/******************************************************************
* File:        Template.cpp
* Description: implement Template class. A class is designed to store
*              template information of the C Lambda language. It
*              may be used in future when the library support template
*              functions and template structures.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "Template.h"

namespace ffscript {
	Template::Template(const std::string& name) : _name(name)
	{
	}

	Template::~Template()
	{
	}

	void Template::addArg(const std::string& arg) {
		_args.push_back(arg);
	}

	int Template::getArgCount() const {
		return (int)_args.size();
	}

	const std::string& Template::operator[](int i) const {
		return _args[i];
	}

	const std::string& Template::name() const {
		return _name;
	}
}