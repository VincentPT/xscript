#include "FFCustomGui.h"
using namespace ci;
using namespace ci::app;

FFCustomGui::FFCustomGui(const std::string& title, int width, int height, ci::app::WindowRef window) : pretzel::PretzelGui(title, width, height, window) {
}
void FFCustomGui::keyDown(const char &key, const int &keyCode) {
	if (keyCode == KeyEvent::KEY_ESCAPE) {
		this->setVisible(false);
		if (_closeEventHandler) {
			_closeEventHandler();
		}
		return;
	}
	pretzel::PretzelGui::keyDown(key, keyCode);
}

void FFCustomGui::setCloseEventHandler(CloseEventHandler&& eventHandler) {
	_closeEventHandler = eventHandler;
}