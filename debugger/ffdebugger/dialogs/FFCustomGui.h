#pragma once
#include "pretzel/PretzelGui.h"
typedef std::function<void()> CloseEventHandler;
class FFCustomGui : public pretzel::PretzelGui {
	CloseEventHandler _closeEventHandler;
public:
	FFCustomGui(const std::string& title, int width, int height, ci::app::WindowRef window);
	void keyDown(const char &key, const int &keyCode) override;
	virtual void setCloseEventHandler(CloseEventHandler&& eventHandler);
};