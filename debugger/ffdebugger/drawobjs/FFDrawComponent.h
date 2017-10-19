#pragma once
#include "FFDrawInterfaces.h"

#include <memory>

class FFDrawComponent {
protected:
	FFColor _rgba;
public:
	FFDrawComponent();
	virtual ~FFDrawComponent();

	void setColor(const FFColor& rgba);
	const FFColor& getColor() const;

	virtual const char* getName() const;
};

typedef std::shared_ptr<FFDrawComponent> FFDrawComponentRef;
#define BASE_COMPONENT_NAME "FFBaseComponent"
#define POINT_COMPONENT_NAME BASE_COMPONENT_NAME