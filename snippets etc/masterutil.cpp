#include "masterutil.h"
#include "base.h"

#include "glutil.h"
#include "setting.h"

extern std::shared_ptr<UI> g_UI;

void REGISTER_UI_CALLBACK(UIEventCallback callback, const char* ident)
{
	g_UI->RegisterCallback(callback, ident);
}

void REGISTER_UI_CALLBACK(UIEventCallback callback, const char* ident, void* arg)
{
	g_UI->RegisterCallback(callback, ident, arg);
}

void RUN_UI_CALLBACK(const char* ident)
{
	g_UI->RunCallback(ident);
}

unsigned int GetShader(const char* shadername) 
{
	setting<std::string> shaderdir("shaderdir");
	std::string vshader(shaderdir.GetSetting());
	std::string fshader(shaderdir.GetSetting());
	vshader += shadername;
	vshader += ".vsh";
	fshader += shadername;
	fshader += ".fsh";
	return LoadShaders(vshader.c_str(), fshader.c_str());
}

void GET_MOUSE_INFO(UIMouseInformation &info)
{
	g_UI->GetMouseInformation(info);
}

bool GET_MOUSE_BUTTON_INFO(int button)
{
	
	return g_UI->GetMouseButtonInformation(button);
}