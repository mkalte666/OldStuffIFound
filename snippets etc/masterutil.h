/*
File: 
Purpose: 
Author(s): Malte Kieﬂling (mkalte666)
*/
#pragma once

#include "render\UI.h"

extern void REGISTER_UI_CALLBACK(UIEventCallback callback, const char* ident);
extern void REGISTER_UI_CALLBACK(UIEventCallback callback, const char* ident, void* arg);
extern void RUN_UI_CALLBACK(const char* ident);

extern unsigned int GetShader(const char* shadername);
extern void GET_MOUSE_INFO(UIMouseInformation &info);
extern bool GET_MOUSE_BUTTON_INFO(int button);

