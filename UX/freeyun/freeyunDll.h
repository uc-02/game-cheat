#pragma once
#ifndef _INTERFACELIB_H_
#define _INTERFACELIB_H_

#ifndef FREEYUN_API
#define FREEYUN_API  //_stdcall
#endif

#ifdef FREEYUN
#define FREEYUN_EXPORT    __declspec(dllexport)
#else
#define FREEYUN_EXPORT    __declspec(dllimport)
#endif


bool FREEYUN_API Init();
int FREEYUN_API UserLogin(char* user);

#endif