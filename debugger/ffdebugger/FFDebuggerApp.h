#pragma once
#include "cinder/app/App.h"
#include "FFDebuggerWindow.h"

#include <list>

// We'll create a new Cinder Application by deriving from the App class
class FFDebuggerApp : public ci::app::App {
public:
	FFDebuggerApp();
	~FFDebuggerApp();
	void setup();
	void setupCliboard();

	void mouseDrag(ci::app::MouseEvent event);
	void keyDown(ci::app::KeyEvent event);
	void draw();
	void update();
	void createNewWindow();
	void onClipboardTextChanged(const std::string& text);
};
