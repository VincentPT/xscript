#pragma once
#include "cinder/app/app.h"
#include <functional>

class FFDialog;
typedef std::function<void(FFDialog* sender)> FFGeneralEventHandler;
typedef FFGeneralEventHandler ButtonClickEventHandler;
typedef FFGeneralEventHandler DialogCloseEventHandler;

class FFCustomGui;
typedef std::shared_ptr<FFCustomGui> FFCustomGuiRef;

class FFDialog {
	FFCustomGuiRef		_nativeDlg;
	DialogCloseEventHandler _closeEventHandler;
protected:
	FFCustomGuiRef getNative() const;
public:
	FFDialog(const std::string& title, int width, int height, ci::app::WindowRef window);
	virtual ~FFDialog();

	void onClose();
	virtual void show();
	virtual void hide();
	virtual void display();
	virtual bool isVisible();
	virtual void setCloseEventHandler(DialogCloseEventHandler&& eventHandler);
};