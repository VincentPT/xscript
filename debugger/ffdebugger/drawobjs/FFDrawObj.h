#pragma once
#include "FFDrawComponent.h"

#include <memory>
#include <string>
#include <map>

class FFDrawObj {
protected:
	std::string _name;
	bool _isVisible;

	struct StringComparator
	{
		bool operator()(const char* left, const char* right) const
		{
			return strcmp(left, right) > 0;
		}
	};

	std::map<const char*, FFDrawComponentRef, StringComparator> _components;
public:
	FFDrawObj();
	virtual ~FFDrawObj();

	// this function will be call each frame before draw function is called
	virtual void update();

	// this function will be call each frame
	virtual void draw() = 0;

	virtual FFLocation getLocation() = 0;

	void setName(const std::string& name);
	const std::string& getName() const;

	void setVisible(bool visible);
	bool isVisible() const;

	void addComponent(const FFDrawComponentRef& component);
 	virtual FFDrawComponent* getComponent(const char* name) const;
};

typedef std::shared_ptr<FFDrawObj> FFDrawObjRef;