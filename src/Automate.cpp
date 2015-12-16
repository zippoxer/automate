#include <iostream>
#include <lua.hpp>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <boost/assign.hpp>
#include "Shlwapi.h"
using namespace std;
using namespace boost::assign;

map<string, int> keyCodes = map_list_of("lbutton", VK_LBUTTON)
	("rbutton", VK_RBUTTON)
	("cancel", VK_CANCEL)
	("mbutton", VK_MBUTTON)
	("back", VK_BACK)
	("tab", VK_TAB)
	("clear", VK_CLEAR)
	("return", VK_RETURN)
	("shift", VK_SHIFT)
	("control", VK_CONTROL)
	("menu", VK_MENU)
	("pause", VK_PAUSE)
	("capital", VK_CAPITAL)
	("kana", VK_KANA)
	("hangeul", VK_HANGEUL)
	("hangul", VK_HANGUL)
	("junja", VK_JUNJA)
	("final", VK_FINAL)
	("hanja", VK_HANJA)
	("kanji", VK_KANJI)
	("escape", VK_ESCAPE)
	("convert", VK_CONVERT)
	("nonconvert", VK_NONCONVERT)
	("accept", VK_ACCEPT)
	("modechange", VK_MODECHANGE)
	("space", VK_SPACE)
	("prior", VK_PRIOR)
	("next", VK_NEXT)
	("end", VK_END)
	("home", VK_HOME)
	("left", VK_LEFT)
	("up", VK_UP)
	("right", VK_RIGHT)
	("down", VK_DOWN)
	("select", VK_SELECT)
	("print", VK_PRINT)
	("execute", VK_EXECUTE)
	("snapshot", VK_SNAPSHOT)
	("insert", VK_INSERT)
	("delete", VK_DELETE)
	("help", VK_HELP)
	("lwin", VK_LWIN)
	("rwin", VK_RWIN)
	("apps", VK_APPS)
	("sleep", VK_SLEEP)
	("numpad0", VK_NUMPAD0)
	("numpad1", VK_NUMPAD1)
	("numpad2", VK_NUMPAD2)
	("numpad3", VK_NUMPAD3)
	("numpad4", VK_NUMPAD4)
	("numpad5", VK_NUMPAD5)
	("numpad6", VK_NUMPAD6)
	("numpad7", VK_NUMPAD7)
	("numpad8", VK_NUMPAD8)
	("numpad9", VK_NUMPAD9)
	("multiply", VK_MULTIPLY)
	("add", VK_ADD)
	("separator", VK_SEPARATOR)
	("subtract", VK_SUBTRACT)
	("decimal", VK_DECIMAL)
	("divide", VK_DIVIDE)
	("f1", VK_F1)
	("f2", VK_F2)
	("f3", VK_F3)
	("f4", VK_F4)
	("f5", VK_F5)
	("f6", VK_F6)
	("f7", VK_F7)
	("f8", VK_F8)
	("f9", VK_F9)
	("f10", VK_F10)
	("f11", VK_F11)
	("f12", VK_F12)
	("f13", VK_F13)
	("f14", VK_F14)
	("f15", VK_F15)
	("f16", VK_F16)
	("f17", VK_F17)
	("f18", VK_F18)
	("f19", VK_F19)
	("f20", VK_F20)
	("f21", VK_F21)
	("f22", VK_F22)
	("f23", VK_F23)
	("f24", VK_F24)
	("numlock", VK_NUMLOCK)
	("scroll", VK_SCROLL)
	("lshift", VK_LSHIFT)
	("rshift", VK_RSHIFT)
	("lcontrol", VK_LCONTROL)
	("rcontrol", VK_RCONTROL)
	("lmenu", VK_LMENU)
	("rmenu", VK_RMENU)
	("oem_1", VK_OEM_1)
	("oem_2", VK_OEM_2)
	("oem_3", VK_OEM_3)
	("oem_4", VK_OEM_4)
	("oem_5", VK_OEM_5)
	("oem_6", VK_OEM_6)
	("oem_7", VK_OEM_7)
	("oem_8", VK_OEM_8)
	("processkey", VK_PROCESSKEY)
	("attn", VK_ATTN)
	("crsel", VK_CRSEL)
	("exsel", VK_EXSEL)
	("ereof", VK_EREOF)
	("play", VK_PLAY)
	("zoom", VK_ZOOM)
	("noname", VK_NONAME)
	("pa1", VK_PA1)
	("oem_clear", VK_OEM_CLEAR)
	("0", 0x30)
	("1", 0x31)
	("2", 0x32)
	("3", 0x33)
	("4", 0x34)
	("5", 0x35)
	("6", 0x36)
	("7", 0x37)
	("8", 0x38)
	("9", 0x39);

int parseKey(string text) {
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	return keyCodes[text];
}

bool compare(string s1, string s2, int type) {
	return (type == 0 && boost::equals(s1, s2)) ||
			(type == 1 && boost::starts_with(s1, s2)) ||
			(type == 2 && boost::ends_with(s1, s2)) ||
			(type == 3 && boost::contains(s1, s2));
}

HANDLE getProcessHandle(string name, int cmptype) {
	DWORD procs[1024];
	EnumProcesses(procs, sizeof(procs), NULL);
	for(int i = 0; i < sizeof(procs); i++) {
		HANDLE h = OpenProcess(PROCESS_VM_READ, false, procs[i]);
		char n[1024];
		GetModuleBaseName(h, NULL, n, sizeof(n));
		if(compare(n, name, cmptype)) {
			return h;
		}
		CloseHandle(h);
	}
	return NULL;
}


const char* checkWindow_title;
int checkWindow_cmptype;
HWND checkWindow_result;
BOOL CALLBACK checkWindow(HWND h, LPARAM lParam) {
    char title[1024];
    GetWindowText(h, title, sizeof(title));
    if(compare(title, checkWindow_title, checkWindow_cmptype)) {
    	checkWindow_result = h;
        return false;
    }
    return true;
}

HWND getWindowHandle(string title, int cmptype) {
	checkWindow_title = title.c_str();
	checkWindow_cmptype = cmptype;
	EnumWindows(checkWindow, 0);
	return checkWindow_result;
}

int setLuaPath( lua_State* L, const char* path )
{
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( L, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
    cur_path.append( path );
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, cur_path.c_str() ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}

class Engine {
public:
	static HANDLE defaultHandle;
	static HWND defaultHwnd;
	static bool defaultHandleDecided, defaultHwndDecided;

	static int findProcess(lua_State *L) {
		HANDLE h;
		if(lua_isstring(L, 1)) {
			string name = lua_tostring(L, 1);
			int cmptype = lua_tonumber(L, 2);
			h = getProcessHandle(name, cmptype);
		} else {
			HWND hwnd = defaultHwnd;
			if(lua_isuserdata(L, 1)) {
				hwnd = (HWND)lua_touserdata(L, 1);
			}
			DWORD pid;
			GetWindowThreadProcessId(hwnd, &pid);
			h = OpenProcess(0x1F0FFF, false, pid);
		}
		lua_pushlightuserdata(L, h);
		if(!defaultHandleDecided) {
			defaultHandle = h;
		}
		return 1;
	}

	static int findWindow(lua_State *L) {
		string title = lua_tostring(L, 1);
		int cmptype = lua_tonumber(L, 2);
		HWND h = getWindowHandle(title, cmptype);
		lua_pushlightuserdata(L, h);
		if(!defaultHwndDecided) {
			defaultHwnd = h;
		}
		return 1;
	}

	static int typewrite(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		string text = lua_tostring(L, ++n);
		for(int i = 0; i < text.length(); i++) {
			PostMessage(h, WM_CHAR, text[i], 0);
		}
		return 0;
	}

	static int keyDown(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		const char* key = lua_tostring(L, ++n);
		PostMessage(h, WM_KEYDOWN, parseKey(key), 0);
		return 0;
	}

	static int keyUp(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		const char* key = lua_tostring(L, ++n);
		PostMessage(h, WM_KEYUP, parseKey(key), 0);
		return 0;
	}

	static int keyPress(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		const char* key = lua_tostring(L, ++n);
		int code = parseKey(key);
		PostMessage(h, WM_KEYDOWN, code, 0);
		PostMessage(h, WM_KEYUP, code, 0);
		return 0;
	}

	static int rightClick(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		POINT p;
		if(lua_gettop(L) == n + 2) {
			p.x = lua_tonumber(L, ++n);
			p.y = lua_tonumber(L, ++n);
		} else {
			GetCursorPos(&p);
		}
		LPARAM lParam = MAKELPARAM(p.x, p.y);
		PostMessage(h, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
		PostMessage(h, WM_RBUTTONUP, MK_RBUTTON, lParam);
		return 0;
	}

	static int leftClick(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		POINT p;
		if(lua_gettop(L) == n + 2) {
			p.x = lua_tonumber(L, ++n);
			p.y = lua_tonumber(L, ++n);
		} else {
			GetCursorPos(&p);
		}
		LPARAM lParam = MAKELPARAM(p.x, p.y);
		PostMessage(h, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		PostMessage(h, WM_LBUTTONUP, MK_LBUTTON, lParam);
		return 0;
	}

	static int move(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		LPARAM lParam = MAKELPARAM(lua_tonumber(L, ++n), lua_tonumber(L, ++n));
		PostMessage(h, WM_MOUSEMOVE, 0, lParam);
		return 0;
	}

	static int leftDown(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		POINT p;
		if(lua_gettop(L) == n + 2) {
			p.x = lua_tonumber(L, ++n);
			p.y = lua_tonumber(L, ++n);
		} else {
			GetCursorPos(&p);
		}
		LPARAM lParam = MAKELPARAM(p.x, p.y);
		PostMessage(h, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		return 0;
	}

	static int leftUp(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		POINT p;
		if(lua_gettop(L) == n + 2) {
			p.x = lua_tonumber(L, ++n);
			p.y = lua_tonumber(L, ++n);
		} else {
			GetCursorPos(&p);
		}
		LPARAM lParam = MAKELPARAM(p.x, p.y);
		PostMessage(h, WM_LBUTTONUP, MK_LBUTTON, lParam);
		return 0;
	}

	static int rightDown(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		POINT p;
		if(lua_gettop(L) == n + 2) {
			p.x = lua_tonumber(L, ++n);
			p.y = lua_tonumber(L, ++n);
		} else {
			GetCursorPos(&p);
		}
		LPARAM lParam = MAKELPARAM(p.x, p.y);
		PostMessage(h, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
		return 0;
	}

	static int rightUp(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		POINT p;
		if(lua_gettop(L) == n + 2) {
			p.x = lua_tonumber(L, ++n);
			p.y = lua_tonumber(L, ++n);
		} else {
			GetCursorPos(&p);
		}
		LPARAM lParam = MAKELPARAM(p.x, p.y);
		PostMessage(h, WM_RBUTTONUP, MK_RBUTTON, lParam);
		return 0;
	}

	static int drag(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		int fx = lua_tonumber(L, ++n),
			fy = lua_tonumber(L, ++n),
			tx = lua_tonumber(L, ++n),
			ty = lua_tonumber(L, ++n);
		int delay = 50;
		if(lua_gettop(L) == n + 1) {
			delay = lua_tonumber(L, ++n);
		}
		LPARAM lParam = MAKELPARAM(fx, fy);
		PostMessage(h, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		Sleep(delay);
		POINT p;
		GetCursorPos(&p);
		SetCursorPos(p.x + 1, p.y);
		Sleep(delay);
		SetCursorPos(p.x, p.y);
		lParam = MAKELPARAM(tx, ty);
		PostMessage(h, WM_LBUTTONUP, MK_LBUTTON, lParam);
		return 0;
	}

	static int read(lua_State *L) {
		int n = 0;
		HANDLE h = defaultHandle;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		int address = lua_tonumber(L, ++n);
		string what = lua_tostring(L, ++n);
		int count;
		char lowerWhat = tolower(what[0]);
		if(lowerWhat == 's') {
			count = atoi(what.substr(2).c_str());
		} else if(lowerWhat == 'i') {
			count = 4;
		} else if(lowerWhat == 'h') {
			count = 2;
		} else if(lowerWhat == 'l') {
			count = 8;
		} else {
			count = 1;
		}
		byte buffer[count];
		ReadProcessMemory(h, (LPVOID)address, (LPVOID)&buffer, count, NULL);
		if(what[0] == 's') {
			lua_pushstring(L, (char*)buffer);
		} else if(what == "i") {
			lua_pushnumber(L, *(int*)&buffer);
		} else if(what == "I") {
			lua_pushnumber(L, *(unsigned int*)&buffer);
		} else if(what == "h") {
			lua_pushnumber(L, *(short*)&buffer);
		} else if(what == "H") {
			lua_pushnumber(L, *(unsigned short*)&buffer);
		} else if(what == "l") {
			lua_pushnumber(L, *(int64_t*)&buffer);
		} else if(what == "L") {
			lua_pushnumber(L, *(uint64_t*)&buffer);
		} else if(what == "b") {
			lua_pushboolean(L, buffer[0]);
		} else if (what == "c") {
			lua_pushnumber(L, (char)buffer[0]);
		} else {
			lua_pushnumber(L, buffer[0]);
		}
		return 1;
	}

	static int write(lua_State *L) {
		return 0;
	}

	static int wait(lua_State *L) {
		int ms = lua_tonumber(L, 1);
		Sleep(ms);
		return 0;
	}

	static int include(lua_State *L) {
		const char* filename = lua_tostring(L, 1);
		char* path = strdup(filename);
		PathRemoveFileSpec(path);
		setLuaPath(L, (string(path) + string("\\?.lua")).c_str());
		if(fopen(filename,"r")==0) {
			filename = (string(filename) + ".lua").c_str();
		}
		luaL_dofile(L, filename);
		return 0;
	}

	static int windowSize(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		RECT rect;
		if(GetWindowRect(h, &rect))
		{
		  int width = rect.right - rect.left;
		  int height = rect.bottom - rect.top;
		  lua_pushnumber(L, width);
		  lua_pushnumber(L, height);
		  return 2;
		}
		return 0;
	}

	static int isDown(lua_State *L) {
		const char* key = lua_tostring(L, 1);
		int state = GetKeyState(parseKey(key));
		lua_pushboolean(L, (state < 0));
		return 1;
	}

	static int focus(lua_State *L) {
		int n = 0;
		HWND h = defaultHwnd;
		if(lua_isuserdata(L, 1)) {
			h = (HWND)lua_touserdata(L, ++n);
		}
		PostMessage(h, WM_SETFOCUS, 0, 0);
		return 0;
	}
};

HANDLE Engine::defaultHandle;
HWND Engine::defaultHwnd;
bool Engine::defaultHandleDecided;
bool Engine::defaultHwndDecided;

bool executeScript(char* path, int argc, char* argv[]) {
	lua_State *L = lua_open();
	luaL_openlibs(L);

	// Assigning functions:
	lua_register(L, "findProcess", Engine::findProcess);
	lua_register(L, "findWindow", Engine::findWindow);
	lua_register(L, "read", Engine::read);
	lua_register(L, "write", Engine::write);
	lua_register(L, "wait", Engine::wait);
	lua_register(L, "typewrite", Engine::typewrite);
	lua_register(L, "keyDown", Engine::keyDown);
	lua_register(L, "keyUp", Engine::keyUp);
	lua_register(L, "keyPress", Engine::keyPress);
	lua_register(L, "leftClick", Engine::leftClick);
	lua_register(L, "rightClick", Engine::rightClick);
	lua_register(L, "leftDown", Engine::leftDown);
	lua_register(L, "leftUp", Engine::leftUp);
	lua_register(L, "move", Engine::move);
	lua_register(L, "drag", Engine::drag);
	lua_register(L, "windowSize", Engine::windowSize);
	lua_register(L, "include", Engine::include);
	lua_register(L, "isDown", Engine::isDown);
	lua_register(L, "focus", Engine::focus);

	// Assigning variables:
	lua_newtable(L);
	for(int i = 0; i < argc; i++) {
		lua_pushnumber(L, i + 1);
		lua_pushstring(L, argv[i]);
		lua_rawset(L, -3);
	}
	lua_setglobal(L, "args");

	luaL_dofile(L, path);
	if(lua_isstring(L, 1)) {
		cerr << lua_tostring(L, 1) << endl;
	}
	return true;
}

int main(int argc, char* argv[]) {
	const char* filename = argv[1];
	if(fopen(filename,"r")==0) {
		filename = (string(filename) + ".lua").c_str();
	}
	executeScript((char*)filename, argc, argv);
	return 0;
}
