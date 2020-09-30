#include <Windows.h>
#include <iostream>
#include <string>
#include <nana/gui.hpp>
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/timer.hpp>

using namespace nana;

void sendPaste(std::string string);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Retrieve handle to Client's window
	HWND handle = FindWindowA(NULL, (LPCTSTR)"League of Legends");
	if (!handle) { if (MessageBox(NULL, (LPCTSTR)"League of Legends was not found!", "ERROR", MB_OK | MB_SYSTEMMODAL | MB_TOPMOST | MB_ICONERROR)); exit(-1); }

	form fm(API::make_center(350, 200), appearance(true, true, true, false, false, false, false));
	
	fm.caption("LoL LaneCaller");
	button btn(fm, nana::rectangle(70, 90, 100, 30));
	btn.edge_effects(false);
	btn.caption("(UN)REGISTER");

	label lb{ fm, rectangle{ 210, 90, 100, 100 } };
	lb.caption("Welcome!");
	lb.format(true);
	
	label lb2{ fm, rectangle{ 90, 140, 200, 100 } };
	lb2.caption("Press <bold>CAPS LOCK</> in League before <bold>Champ Select</> to <bold>activate</> spammer!");
	lb2.format(true);

	textbox txt(fm, rectangle(70, 40, 210, 30));

	RECT dim;
	nana::timer timer;
	nana::timer autoclose;
	autoclose.interval(std::chrono::minutes(1));
	autoclose.elapse([&fm] { fm.close();  exit(1); });
	timer.interval(std::chrono::milliseconds(50));
	// "Main" loop
	timer.elapse([&handle, &dim, &txt] {
		while (GetForegroundWindow() == handle && GetKeyState(VK_CAPITAL) & 1) {
			// Positioning the cursor on chat
			if (!GetWindowRect(handle, &dim)) { std::cout << "Something went wrong!"; exit(-1); }
			if (!SetCursorPos(dim.left + 50, dim.bottom - 40)) { continue; }

			sendPaste(txt.caption());
			Sleep(50);
		}
		});
	

	btn.events().click([&txt, &lb, &timer, &autoclose] {
		static bool visibility = true;
		txt.enabled(visibility = !visibility);
		if (visibility == false) {
			std::string caption = txt.caption();
			if (caption.length() <= 1) { std::cout << "invalid input, its small\n";  lb.caption("<bold color=0xff0000>Registered:</> ERROR"); }
			else {
				std::cout << "Currently registered: " << caption << std::endl;
				lb.caption("<bold color=0xff0000>Registered:</> " + caption);
				timer.start();
				if (autoclose.started() == false) { autoclose.start(); }
			}
		}
		else {
			timer.stop();
		}
		});
	fm.show();
	nana::exec();

	return 0;
}

void sendPaste(std::string string) {
	unsigned int len = string.length();
	INPUT* str_inputs = new INPUT[len];

	// Define enter key
	INPUT enterKey;
	enterKey.type = INPUT_KEYBOARD;
	enterKey.ki.wScan = 0;
	enterKey.ki.time = 0;
	enterKey.ki.dwExtraInfo = 0;
	enterKey.ki.dwFlags = 0;
	enterKey.ki.wVk = VK_RETURN;

	// Define mouse click
	INPUT mouseClick;
	mouseClick.type = INPUT_MOUSE;
	mouseClick.mi.dx = 0;
	mouseClick.mi.dy = 0;
	mouseClick.mi.mouseData = 0;
	mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	mouseClick.mi.time = 0;
	mouseClick.mi.dwExtraInfo = 0;
	SendInput(1, &mouseClick, sizeof(INPUT));
	
	for (int i = 0; i < len; i++) {
		// Arrange string as INPUT struct
		str_inputs[i].type = INPUT_KEYBOARD;
		str_inputs[i].ki.wScan = 0;
		str_inputs[i].ki.time = 0;
		str_inputs[i].ki.dwExtraInfo = 0;
		str_inputs[i].ki.dwFlags = 0;
		str_inputs[i].ki.wVk = VkKeyScanA((TCHAR)string[i]);;
	}
	
	
	mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &mouseClick, sizeof(INPUT));
	SendInput(len+1, &*str_inputs, sizeof(INPUT));
	SendInput(1, &enterKey, sizeof(INPUT));
	delete str_inputs;
	return;
	
	
	INPUT pasteKey;
	pasteKey.type = pasteKey_KEYBOARD;
	pasteKey.ki.wScan = 0;
	pasteKey.ki.time = 0;
	pasteKey.ki.dwExtraInfo = 0;
	pasteKey.ki.dwFlags = 0;

	// Press CTRL
	pasteKey.ki.wVk = VK_CONTROL;
	SendInput(1, &pasteKey, sizeof(INPUT));
	
	// Press V
	pasteKey.ki.wVk = 0x56;
	SendInput(1, &pasteKey, sizeof(INPUT));

	// Release keys
	pasteKey.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &pasteKey, sizeof(INPUT));

	pasteKey.ki.wVk = VK_CONTROL;
	SendInput(1, &pasteKey, sizeof(INPUT));
}
