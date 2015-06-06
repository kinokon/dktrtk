#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。

#define AS_USE_STLNAMES
// Windows ヘッダー ファイル:
#include <Windows.h>
#include <vector>
#include <regex>
#include <string>
#include <map>

#include <tchar.h>

#include <iostream>  // cout
#include <assert.h>  // assert()
#include <string.h>  // strstr()

#include <conio.h>   // kbhit(), getch()
#include <windows.h> // timeGetTime()

#include <angelscript.h>
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/scriptarray/scriptarray.h"
#include "add_on/scriptany/scriptany.h"
#include "add_on/scripthandle/scripthandle.h"
#include "add_on/weakref/weakref.h"
#include "add_on/scriptdictionary/scriptdictionary.h"
#include "add_on/scriptfile/scriptfile.h"
#include "add_on/scriptfile/scriptfilesystem.h"
#include "add_on/scriptmath/scriptmath.h"
#include "add_on/scriptmath/scriptmathcomplex.h"
#include "add_on/scriptgrid/scriptgrid.h"
#include "add_on/scriptbuilder/scriptbuilder.h"


using namespace std;

extern "C" __declspec(dllexport) BOOL __cdecl load(HGLOBAL h, long len);
extern "C" __declspec(dllexport) BOOL __cdecl unload();
extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len);

static string w2s(wstring in, string cs = "UTF-8");
static wstring s2w(string in, string cs = "UTF-8");

int  CompileScript(asIScriptEngine *engine);

asIScriptEngine *engine;
asIScriptContext *ctx;
