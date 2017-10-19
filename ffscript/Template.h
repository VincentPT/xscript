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
