#include "FFDebuggerApp.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <list>
#include "FFDebuggerWindow.h"
#include "utils/ClipboardViewer.h"

#include "utils/string_util.hpp"
#include "spy_interfaces.h"

using namespace ci;
using namespace ci::app;
using namespace std;

FFDebuggerApp::FFDebuggerApp() {
}
FFDebuggerApp::~FFDebuggerApp() {
}

void setupConsole() {
	//AllocConsole();

	//HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	//int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
	//FILE* hf_out = _fdopen(hCrt, "w");
	//setvbuf(hf_out, NULL, _IONBF, 1);
	//*stdout = *hf_out;

	//HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	//hCrt = _open_osfhandle((intptr_t)handle_in, _O_TEXT);
	//FILE* hf_in = _fdopen(hCrt, "r");
	//setvbuf(hf_in, NULL, _IONBF, 128);
	//*stdin = *hf_in;

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

void FFDebuggerApp::setup()
{
	setupConsole();

	WindowRef defaultNativeWindow = getWindow();

	auto firstWindow = new FFDebuggerWindow(defaultNativeWindow);
	firstWindow->setTitle("buzz");

	setupCliboard();
}

void FFDebuggerApp::setupCliboard() {
	using namespace std::placeholders;

	ClipboardViewer* clipboardViewer = ClipboardViewer::getInstance();
	// register ANSI text handler
	clipboardViewer->connectANSI(std::bind(&FFDebuggerApp::onClipboardTextChanged, this, _1));
}

void FFDebuggerApp::createNewWindow()
{
	auto newWindow = new FFDebuggerWindow("buzz#" + std::to_string(getNumWindows()), 960, 540);
}

void FFDebuggerApp::mouseDrag( MouseEvent event )
{
}

void FFDebuggerApp::keyDown( KeyEvent event )
{
/*	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
	else */
	if( event.getChar() == 'q' )
		createNewWindow();
}

void FFDebuggerApp::update() {
	auto windowCount = getNumWindows();
	
	// find another window available
	for (int i = 0; i < windowCount; i++) {
		auto nativeWindow = getWindowIndex(i);
		nativeWindow->getUserData<FFDebuggerWindow>()->update();
	}
}

void FFDebuggerApp::draw() {
	auto currentNativeWindow = getWindow();
	auto currentWindow = currentNativeWindow->getUserData<FFDebuggerWindow>();
	currentWindow->draw();
}

void FFDebuggerApp::onClipboardTextChanged(const std::string& text) {
	auto activeWindow = this->getForegroundWindow();
	auto buzzWindow = activeWindow->getUserData<FFDebuggerWindow>();

	cout << "process for changing text in clipboard" << std::endl;

	void* address = ObjectInputerDlg::convertToAddress(text);
	if (address != nullptr) {
		buzzWindow->showInputerWithAddress(address);
	}
	else {
		cout << "check clipboard for visual studio's copied text" << std::endl;
		// parse string copy from a row in Watch window of visual studio
		vector<string> elems;
		split(text, '\t', elems);

		if (elems.size() == 5) {
			auto name = elems[2];
			auto value = elems[3];
			auto stype = elems[4];
			if (stype.size() >= 2) {
				// remove end line characters at the end of last elemenet.
				stype.erase(stype.size() - 2, 2);
			}

			static const char* ffscriptNodeTypes[] = {
				"ffscript::ExpUnit *",
				"const ffscript::ExpUnit *",
				"ffscript::ExecutableUnit *",
				"const ffscript::ExecutableUnit *",
				"ffscript::Function *",
				"const ffscript::Function *",
				"ffscript::DynamicParamFunction *",
				"const ffscript::DynamicParamFunction *",
			};

			constexpr int ffscriptNodeTypeCount = sizeof(ffscriptNodeTypes)/sizeof(ffscriptNodeTypes[0]);

			static bool initialize = false;

			static map<string, int> supportedTypes = {
				{ "std::list<std::shared_ptr<ffscript::ExpUnit>,std::allocator<std::shared_ptr<ffscript::ExpUnit> > > *", (int)UserCommandId::READ_UNIT_LIST},
				{ "const std::list<std::shared_ptr<ffscript::ExpUnit>,std::allocator<std::shared_ptr<ffscript::ExpUnit> > > *", (int)UserCommandId::READ_UNIT_LIST },
			};

			if (initialize == false) {
				for (int i = 0; i < ffscriptNodeTypeCount; i++) {
					supportedTypes[ffscriptNodeTypes[i]] = (int)UserCommandId::READ_EXPRESSION_NODE;
				}
				initialize = true;
			}

			auto it = supportedTypes.find(stype);
			int type = -1;
			if (it == supportedTypes.end()) {
				for (int i = 0; i < ffscriptNodeTypeCount; i++) {
					auto pos = stype.find(ffscriptNodeTypes[i]);
					if (pos != string::npos) {
						type = (int)UserCommandId::READ_EXPRESSION_NODE;
						break;
					}
				}
			}

			if (type != -1) {
				auto spaceIdx = value.find(' ');
				// there is must be a space between address and object information
				if (spaceIdx == string::npos) {
					cout << "unrecognize text" << std::endl;
					return;
				}

				// get the address in value variable and convert it into address
				address = ObjectInputerDlg::convertToAddress(value.substr(0, spaceIdx));
				// check if the string is a valid pointer
				if (address == nullptr) {
					cout << "invalid pointer" << std::endl;
					return;
				}
				cout << "parse text successfully" << std::endl;

				auto newDrawingObject = buzzWindow->readObject(address, type);
				if (newDrawingObject) {
					newDrawingObject->setName(name);
					buzzWindow->addNewObject(newDrawingObject);
				}
			}
			else {
				cout << "unsupported object in clipboard" << std::endl;
			}
		}
	}
}

// This line tells Cinder to actually create the application
CINDER_APP( FFDebuggerApp, RendererGl )