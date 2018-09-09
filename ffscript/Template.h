/******************************************************************
* File:        Template.h
* Description: declare Template class. A class is designed to store
*              template information of the C Lambda language. It
*              may be used in future when the library support template
*              functions and template structures.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <string>
#include <vector>
#include <memory>

namespace ffscript {
	class Template
	{
		std::string _name;
		std::vector<std::string> _args;
	public:
		Template(const std::string& name);
		virtual ~Template();

		void addArg(const std::string& arg);
		int getArgCount() const;
		const std::string& operator[](int i) const;
		const std::string& name() const;
	};

	typedef std::shared_ptr<Template> TemplateRef;
}
