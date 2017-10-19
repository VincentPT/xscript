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