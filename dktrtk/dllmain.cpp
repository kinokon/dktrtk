// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "dllmain.h"

FILE *f;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


BOOL __cdecl load(HGLOBAL h, long len){
	char* buf = new char[len + 1];
	memcpy(buf, h, len);
	buf[len] = 0;
	string param((char*)buf);
	GlobalFree(h);
	delete[] buf;

	int r;

	SetCurrentDirectory(s2w(param).c_str());
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if (engine == 0)return false;
	engine->SetEngineProperty(asEP_SCRIPT_SCANNER, 0);
	
	//add-on
	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterStdStringUtils(engine);
	RegisterScriptAny(engine);
	RegisterScriptHandle(engine);
	RegisterScriptWeakRef(engine);
	RegisterScriptDictionary(engine);
	RegisterScriptFile(engine);
	RegisterScriptFileSystem(engine);
	RegisterScriptMath(engine);
	RegisterScriptMathComplex(engine);
	RegisterScriptGrid(engine);

	r = CompileScript(engine);
	if (r < 0)
	{
		engine->Release();
		return false;
	}
	ctx = engine->CreateContext();
	if (ctx == 0)
	{
		engine->Release();
		return false;
	}

	asIScriptFunction *func = engine->GetModule(0)->GetFunctionByDecl("bool load(string)");
	if (func == 0)
	{
		return false;
	}

	r = ctx->Prepare(func);
	if (r < 0)
	{
		return false;
	}
	ctx->SetArgObject(0, &param);
	r = ctx->Execute();
	if (r != asEXECUTION_FINISHED)return false;
	bool returnValue = ctx->GetReturnByte();
	return returnValue;
}


BOOL __cdecl unload(){
	int r;
	asIScriptFunction *func = engine->GetModule(0)->GetFunctionByDecl("bool unload()");
	if (func == 0)
	{
		ctx->Release();
		engine->Release();
		return false;
	}
	r = ctx->Prepare(func);
	if (r < 0)
	{
		ctx->Release();
		engine->Release();
		return false;
	}
	r = ctx->Execute();

	if (r != asEXECUTION_FINISHED){
		ctx->Release();
		engine->Release();
		return false;
	}
	bool returnValue = ctx->GetReturnByte();
	ctx->Release();
	engine->Release();
	return returnValue;
}
HGLOBAL __cdecl request(HGLOBAL h, long *len){

	char* buf = new char[*len + 1];
	memcpy(buf, h, *len);
	buf[*len] = 0;
	string param((char*)buf);
	GlobalFree(h);
	delete[] buf;


	int r;


	asIScriptFunction *func = engine->GetModule(0)->GetFunctionByDecl("string request(string)");
	if (func == 0)
	{
		return false;
	}

	r = ctx->Prepare(func);
	if (r < 0)
	{
		return false;
	}
	ctx->SetArgObject(0, &param);
	r = ctx->Execute();
	if (r != asEXECUTION_FINISHED)return false;
	string* ret = (string*)ctx->GetReturnObject();

	*len = ret->size() + 1;
	buf = (char*)GlobalAlloc(GMEM_FIXED, *len);
	memcpy(buf, ret->c_str(), *len);
	//	((char*)buf)[*len - 1] = 0;
	return buf;
}


int CompileScript(asIScriptEngine *engine)
{
	int r;

	CScriptBuilder builder;

	r = builder.StartNewModule(engine, 0);
	if (r < 0)
	{
		return r;
	}
	r = builder.AddSectionFromFile("index.as");
	if (r < 0)
	{
		return r;
	}
	r = builder.BuildModule();
	return r;
}

wstring s2w(string in, string cs){
	UINT csint;
	if (cs == "Shift_JIS"){
		csint = 932;
	}
	else if (cs == "ISO-2022-JP"){
		csint = 50221;
	}
	else if (cs == "EUC-JP"){
		csint = 51932;
	}
	else if (cs == "UTF-8"){
		csint = 65001;
	}
	else{
		csint = CP_ACP;
	}
	const int nSize = MultiByteToWideChar(csint, 0, (LPCSTR)in.c_str(), -1, NULL, 0);
	TCHAR* buff = new TCHAR[nSize + 1];
	MultiByteToWideChar(csint, 0, (LPCSTR)in.c_str(), -1, (LPWSTR)buff, nSize);
	wstring ret(buff);
	delete[] buff;
	return ret;
}

string w2s(wstring in, string cs){
	UINT csint;
	if (cs == "Shift_JIS"){
		csint = 932;
	}
	else if (cs == "ISO-2022-JP"){
		csint = 50221;
	}
	else if (cs == "EUC-JP"){
		csint = 51932;
	}
	else if (cs == "UTF-8"){
		csint = 65001;
	}
	else{
		csint = CP_ACP;
	}
	const int nSize = WideCharToMultiByte(csint, 0, (LPCWSTR)in.c_str(), -1, NULL, 0, NULL, NULL);
	BYTE* buff = new BYTE[nSize + 1];
	WideCharToMultiByte(csint, 0, (LPCWSTR)in.c_str(), -1, (LPSTR)buff, nSize, NULL, NULL);
	string ret = string((char*)buff);
	delete[] buff;
	return ret;
}