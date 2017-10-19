#include "FFDialog.h"
#include "FFCustomGui.h"

FFDialog::FFDialog(const std::string& title, int width, int height, ci::app::WindowRef window) {
	_nativeDlg = std::make_shared<FFCustomGui> (title, width, height, window);

	_nativeDlg->setCloseEventHandler([this]() {
		if (_closeEventHandler) {
			_closeEventHandler(this);
		}
	});
}

FFDialog::~FFDialog() {}

FFCustomGuiRef FFDialog::getNative() const {
	return _nativeDlg;
}

void FFDialog::onClose() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(false);
		if (_closeEventHandler) {
			_closeEventHandler(this);
		}
	}
}

void FFDialog::show() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(true);
	}
}

void FFDialog::hide() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(false);
	}
}

void FFDialog::display() {
	if (_nativeDlg && _nativeDlg->isVisible()) {
		_nativeDlg->draw();
	}
}

bool FFDialog::isVisible() {
	return (_nativeDlg && _nativeDlg->isVisible());
}

void FFDialog::setCloseEventHandler(DialogCloseEventHandler&& eventHandler) {
	_closeEventHandler = eventHandler;
}