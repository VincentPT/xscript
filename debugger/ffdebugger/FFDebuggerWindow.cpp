#include "FFDebuggerWindow.h"
#include "utils/ClipboardViewer.h"
#include "pretzel/PretzelGui.h"
#include "drawobjs/FFContainer.h"
#include "drawobjs/FFCompLine.h"

#include "drawobjs\FFRect.h"

#include "FFDebuggerApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

FFDebuggerWindow::FFDebuggerWindow(const std::string& title, int width, int height)
{
	ci::app::Window::Format format;

	auto mainDisplay = Display::getMainDisplay();
	auto& displays = Display::getDisplays();
	// select a default display
	auto displayRef = mainDisplay;
	for (size_t i = 0; i != displays.size(); i++) {
		displayRef = displays[i];
		if (mainDisplay.get() != displayRef.get()) {
			break;
		}
	}
	
	format.display(displayRef);
	_nativeWindow = App::get()->createWindow(format);
	_nativeWindow->setUserData(this);

	setTitle(title);
	setSize(width, height);

	setupWindow();

}

FFDebuggerWindow::FFDebuggerWindow(ci::app::WindowRef nativeWindow) : _nativeWindow(nativeWindow) {
	_nativeWindow->setUserData(this);

	ClipboardViewer* clipboardViewer = ClipboardViewer::getInstance();
	// start monitor the clipboars
	clipboardViewer->startMonitor((HWND)_nativeWindow->getNative());

	setupWindow();
}

FFDebuggerWindow::~FFDebuggerWindow()
{
	_spyClient->stopMonitorProcess();
}

void FFDebuggerWindow::setupWindow() {
	using namespace std::placeholders;
	auto size = _nativeWindow->getSize();
	_frameBuffer = gl::Fbo::create(size.x, size.y, true, true, true);

	// window settings gui
	_windowSettingsDlgRef = std::make_shared<WindowSettingsDlg>(_nativeWindow);
	_windowSettingsDlgRef->hide();

	// object settings gui
	_objectSettingsDlgRef = std::make_shared<ObjectSettingsDlg>(_nativeWindow);
	_objectSettingsDlgRef->hide();

	// add object gui
	_objectInputerDlgRef = std::make_shared<ObjectInputerDlg>(_nativeWindow);
	_objectInputerDlgRef->hide();

	_bckColor = &_windowSettingsDlgRef->getBckColor();
	_windowSettingsDlgRef->getMonitorButtonSignal() = std::bind(&FFDebuggerWindow::onStartMonitorProcess, this, _1);
	_objectInputerDlgRef->getAddObjectButtonSignal() = std::bind(&FFDebuggerWindow::onAddObjectClick, this, _1);	
	
	// setup user's event
	_nativeWindow->getSignalKeyDown().connect(std::bind(&FFDebuggerWindow::onKeyPress, this, _1));

	_nativeWindow->getSignalClose().connect(std::bind(&FFDebuggerWindow::onClose, this));
	_nativeWindow->getSignalResize().connect(std::bind(&FFDebuggerWindow::onResize, this));
	_nativeWindow->getSignalMouseWheel().connect(std::bind(&FFDebuggerWindow::onMouseWheel, this, _1));

	_rootObject = make_shared<FFContainer>();
	_rootObject->setName("Sence");

	FFRect* ffrect = new FFRect(100, 100, 200, 50);
	ffrect->setText("sample");

	addNewObject(ffrect);

	//onStartMonitorProcess(nullptr);
}

FFDebuggerWindow& FFDebuggerWindow::setTitle(const std::string& title) {
	_nativeWindow->setTitle(title);
	return *this;
}

FFDebuggerWindow& FFDebuggerWindow::setSize(int width, int height) {
	_nativeWindow->setSize(width, height);
	return *this;
}

void FFDebuggerWindow::needUpdate(bool flag) {
	_needUpdate = flag;
}

void FFDebuggerWindow::pendingUpdate() {

}

void applyObjectSettingForAll(FFDrawObj* obj, const std::function<void(FFDrawObj* obj)>& f) {
	f(obj);

	auto pContainer = dynamic_cast<FFContainer*>(obj);
	if (pContainer != nullptr) {
		auto& children = pContainer->getChildren();
		for (auto &aChildRef : children) {
			applyObjectSettingForAll(aChildRef.get(), f);
		}
	}
}

void applyLineColorForAll(FFDrawObj* obj, const ColorA& color) {
	int a, r, g, b;
	a = (int)(color.a * 255);
	r = (int)(color.r * 255);
	g = (int)(color.g * 255);
	b = (int)(color.b * 255);
	FFColor buzzColor = BUZZCOLOR(r, g, b, a);

	auto changeColor = [&buzzColor](FFDrawObj* obj) {
		auto lineComponent = obj->getComponent(LINE_COMPONENT_NAME);
		if (lineComponent) {
			lineComponent->setColor(buzzColor);
		}
	};

	applyObjectSettingForAll(obj, changeColor);
}

void FFDebuggerWindow::update() {
	if (_needRestartClipboard) {
		double currentTimeFromLaunch = App::get()->getElapsedSeconds();
		if (currentTimeFromLaunch - _sheduleClipboardRestartAt >= (double)_restartClipboardIV) {
			auto clipboardViewer = ClipboardViewer::getInstance();
			clipboardViewer->startMonitor((HWND)_nativeWindow->getNative());
	
			((FFDebuggerApp*)App::get())->setupCliboard();

			cout << "restarted clipboard viewer" << endl;
			_needRestartClipboard = false;
		}
	}
}

void FFDebuggerWindow::scheduleRestartClipboard() {
	_needRestartClipboard = true;
	_sheduleClipboardRestartAt = App::get()->getElapsedSeconds();
	cout << "shedule of restarting clipboard has been set!" << endl;
}

bool FFDebuggerWindow::isRestartClipboardShduled() const {
	return _needRestartClipboard;
}

void FFDebuggerWindow::cancleRestartClipboard() {
	if (_needRestartClipboard) {
		cout << "shedule of restarting clipboard has been cancelled!" << endl;
	}
	_needRestartClipboard = false;
}

void FFDebuggerWindow::draw() {
	gl::clear(*_bckColor);
	if (_pendingUpdate || _pendingResize) {
		double currentTimeFromLaunch = App::get()->getElapsedSeconds();
		if (currentTimeFromLaunch - _lastEventOccur > (double)_updateFBOIV) {
			// update the sence later
			needUpdate();

			if (_pendingResize) {
				auto size = _nativeWindow->getSize();
				_frameBuffer = gl::Fbo::create(size.x, size.y, true, true, true);
				_pendingResize = false;
			}
		}
	}

	if (_objectSettingsDlgRef->isVisible()) {
		if (_objectLine != _objectSettingsDlgRef->getObjectLineColor()) {
			_objectLine = _objectSettingsDlgRef->getObjectLineColor();
			applyLineColorForAll(_rootObject.get(), _objectLine);
			_needUpdate = true;
		}
		if (_objectLineWidth != _objectSettingsDlgRef->getLineWidth()) {
			_objectLineWidth = _objectSettingsDlgRef->getLineWidth();
			
			auto changeLineWidth = [this](FFDrawObj* obj) {
				auto lineComponent = (FFCompLine*) obj->getComponent(LINE_COMPONENT_NAME);
				if (lineComponent) {
					lineComponent->setLineWidth((float)_objectLineWidth);
				}
			};

			applyObjectSettingForAll(_rootObject.get(), changeLineWidth);

			_needUpdate = true;
		}
	}

	if(_needUpdate)
	{		
		gl::ScopedFramebuffer fbScp(_frameBuffer);
		gl::ScopedModelMatrix scopedModelMatrix;
		gl::setModelMatrix(_transform);
		gl::clear(*_bckColor);
		if (_tex) {
			gl::draw(_tex);
		}

		_rootObject->draw();

		// reset update flag
		_needUpdate = _afterUpdate;
		// reset pending update flag
		_pendingUpdate = false;
	}

	if(_pendingUpdate)
	{
		gl::ScopedModelMatrix scopedModelMatrix;
		gl::setModelMatrix(_texTransform * _transform);
		gl::draw(_frameBuffer->getColorTexture());
	}
	else {
		gl::draw(_frameBuffer->getColorTexture());
	}

	pretzel::PretzelGui::drawAll();
}

void FFDebuggerWindow::addNewObject(FFDrawObj* obj) {
	_rootObject->addObject(FFDrawObjRef(obj));
}

void FFDebuggerWindow::showInputerWithAddress(void* address) {	
	_objectInputerDlgRef->setObjectAddress(address);
	_objectInputerDlgRef->show();
}

void FFDebuggerWindow::onKeyPress(KeyEvent& e) {
	using namespace std::placeholders;

	if (!_activeDialog || _activeDialog->isVisible() == false) {
		if (e.getCode() == KeyEvent::KEY_w) {
			_needUpdate = true;
			_afterUpdate = true;
			_windowSettingsDlgRef->show();
			_activeDialog = _windowSettingsDlgRef;
			e.setHandled();
			_windowSettingsDlgRef->setCloseEventHandler([this](FFDialog*) {_afterUpdate = false; });
		}
		else if (e.getCode() == KeyEvent::KEY_e) {
			_objectSettingsDlgRef->show();
			_activeDialog = _objectSettingsDlgRef;
			e.setHandled();
		}
		else if (e.getCode() == KeyEvent::KEY_r) {
			_objectInputerDlgRef->show();
			_activeDialog = _objectInputerDlgRef;
			e.setHandled();
		}
		else if (e.getCode() == KeyEvent::KEY_t) {
			_hierarchyDialog = std::make_shared<ObjectHierarchyDlg>((HWND)_nativeWindow->getNative());
			_hierarchyDialog->setObjectRoot(_rootObject);
			_hierarchyDialog->setItemCheckChangedHandler(std::bind(&FFDebuggerWindow::onObjectCheckedChanged, this, _1, _2));
			_hierarchyDialog->show();
		}
		else if (e.getCode() == KeyEvent::KEY_y) {
			_hierarchyDialog2 = std::make_shared<ObjectHierarchyDlg2>((HWND)_nativeWindow->getNative());
			_hierarchyDialog2->setObjectRoot(_rootObject);
			_hierarchyDialog2->setItemCheckChangedHandler(std::bind(&FFDebuggerWindow::onObjectCheckedChanged, this, _1, _2));
			_hierarchyDialog2->show();
		}
	}
}

void FFDebuggerWindow::onClose() {
	auto clipboardViewer = ClipboardViewer::getInstance();

	auto checkAndSheduleRestartClipboard = [&]() {
		// start monitor clipboard on another window
		auto app = App::get();
		auto windowCount = app->getNumWindows();
		if (windowCount < 2) {
			// no window left
			return;
		}

		// find another window available
		for (int i = (int)(windowCount - 1); i >= 0; i--) {
			auto nativeWindow = app->getWindowIndex(i);
			if (nativeWindow.get() != _nativeWindow.get()) {
				nativeWindow->getUserData<FFDebuggerWindow>()->scheduleRestartClipboard();
				break;
			}
		}
	};

	// check if current viewer is going to closed
	if (clipboardViewer->getCurrentViewer() != (HWND)_nativeWindow->getNative()) {
		if (isRestartClipboardShduled()) {
			cancleRestartClipboard();
			checkAndSheduleRestartClipboard();
		}
		return;
	}
	clipboardViewer->stopMonitor();
	clipboardViewer->clearHandlers();
	checkAndSheduleRestartClipboard();
}

void FFDebuggerWindow::onResize() {
	auto size =_nativeWindow->getSize();
	if (!_frameBuffer) {
		_frameBuffer = gl::Fbo::create(size.x, size.y, true, true, true);
		// need update FBO on draw method
		needUpdate();
	}
	else {
		_lastEventOccur = App::get()->getElapsedSeconds();
		_pendingResize = true;
	}
}

void FFDebuggerWindow::onMouseWheel(MouseEvent& me) {
	float increment = me.getWheelIncrement();
	float ratio = 0.05f;
	ratio = 1.0f + increment * ratio;

	me.setHandled();

	if (_pendingUpdate == false) {
		_texTransform = glm::inverse(_transform);
	}

	_transform *= glm::scale(vec3(ratio, ratio, 1.0f));

	_lastEventOccur = App::get()->getElapsedSeconds();
	_pendingUpdate = true;
}

void FFDebuggerWindow::onStartMonitorProcess(FFDialog* sender) {
	if (!_spyClient) {
		_spyClient = std::make_shared<FFSpyClient>();
	}

	auto& currentMonitorProcessName = _spyClient->getProcessName();
	auto desireProcessName = _windowSettingsDlgRef->getProcessName();

	if (desireProcessName.empty()) {
		return;
	}

	if (desireProcessName != currentMonitorProcessName) {
		_spyClient->stopMonitorProcess();
		_spyClient->startMonitorProcess(desireProcessName.c_str());
	}
	else if (_spyClient->checkTargetAvaible() == false) {
		_spyClient->startMonitorProcess(desireProcessName.c_str());
	}
}

void FFDebuggerWindow::onAddObjectClick(FFDialog* sender) {	
	void* desireReadObjectAddress = _objectInputerDlgRef->getObjectAddress();
	int type = _objectInputerDlgRef->getSelectedTypeIndex();

	FFDrawObj* obj = readObject(desireReadObjectAddress, type);
	if (obj) {
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(void*) * 2)
			<< std::hex << desireReadObjectAddress;
		obj->setName(ss.str());
		addNewObject(obj);
	}
}

void FFDebuggerWindow::onObjectCheckedChanged(FFDrawObj* obj, BOOL newState) {
	if (newState == TRUE) {
		obj->setVisible(true);
	}
	else if (newState == FALSE) {
		obj->setVisible(false);
	}
	needUpdate();
}