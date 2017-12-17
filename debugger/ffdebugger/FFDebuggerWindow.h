#pragma once
#include <string>
#include <list>
#include "cinder/app/Renderer.h"
#include "cinder/gl/gl.h"
#include "dialogs/ObjectInputerDlg.h"
#include "dialogs/ObjectSettingsDlg.h"
#include "dialogs/WindowSettingsDlg.h"
#include "utils/FFSpyClient.h"
#include "drawobjs/FFContainer.h"
#include "dialogs/ObjectHierarchyDlg.h"
#include "dialogs/ObjectHierarchyDlg2.h"


class FFDebuggerWindow
{
	std::shared_ptr<FFSpyClient>			_spyClient;
	bool									_needUpdate = true;
	bool									_afterUpdate = false;
	bool									_pendingUpdate = false;
	bool									_pendingResize = false;
	double									_updateFBOIV = 0.25f; //time in seconds to update FBO after an user's continous event
	double									_lastEventOccur = 0; // time in seconds that last continous event occur
	double									_sheduleClipboardRestartAt; // time when restart clipboard action is requested
	float									_restartClipboardIV = 1.0f; //time in seconds to restart clipboard viewer
	bool									_needRestartClipboard = false;

	ci::app::WindowRef						_nativeWindow;	
	ci::ColorA*								_bckColor;
	ci::ColorA								_objectLine;
	int										_objectLineWidth = 1;
	ci::gl::Texture2dRef					_tex;
	ci::gl::FboRef							_frameBuffer;
	glm::mat4								_transform;
	glm::mat4								_texTransform;

	std::shared_ptr<WindowSettingsDlg>		_windowSettingsDlgRef;
	std::shared_ptr<ObjectSettingsDlg>		_objectSettingsDlgRef;
	std::shared_ptr<ObjectInputerDlg>		_objectInputerDlgRef;
	std::shared_ptr<FFDialog>				_activeDialog;
	std::shared_ptr<FFContainer>			_rootObject;
	std::shared_ptr<ObjectHierarchyDlg>		_hierarchyDialog;
	std::shared_ptr<ObjectHierarchyDlg2>	_hierarchyDialog2;
private:
	void onKeyPress(ci::app::KeyEvent& e);
	void onClose();
	void onResize();
	void onMouseWheel(ci::app::MouseEvent& me);
	void onStartMonitorProcess(FFDialog* sender);
	void onAddObjectClick(FFDialog* sender);
	void onObjectCheckedChanged(FFDrawObj* obj, BOOL newState);
protected:
	void setupWindow();	
	void pendingUpdate();
public:
	FFDebuggerWindow(const std::string& title, int width, int height);
	FFDebuggerWindow(ci::app::WindowRef nativeWindow);
	~FFDebuggerWindow();

	void needUpdate(bool flag = true);
	void scheduleRestartClipboard();
	bool isRestartClipboardShduled() const;
	void cancleRestartClipboard();

	FFDebuggerWindow& setTitle(const std::string& title);
	FFDebuggerWindow& setSize(int width, int height);

	void showInputerWithAddress(void* address);

	/// this function is should only should only be called by draw function of a cinder app
	void draw();
	void update();

	void addNewObject(FFDrawObj* obj);

	FFDrawObj* readObject(void* address, int type);
	FFDrawObj* readUnitList(void* desireReadObjectAddress);
	FFDrawObj* readUnitNode(void* desireReadObjectAddress);
	FFDrawObj* readProgramCommands(void* desireReadObjectAddress);
	FFDrawObj* readCommands(void* desireReadObjectAddress);
};

typedef std::shared_ptr<FFDebuggerWindow> FFDebuggerWindowRef;
