#pragma once
#include "xsgui.h"
#include <stdio.h>      // FILE*, sscanf
#include "VMProtectSDK.h"
struct xs_config
{
	bool InitConfig();
	bool SaveConfig();
	void OffConfig();

	FILE* ConfigHandle;
	char* ConfigPath;
	void* Data;
	size_t size;


	char* Read_ulong_Config(const char* itme, const char* subitem, UINT32* value);
	char* Write_ulong_Config(const char* itme, const char* subitem, UINT32 value);

	char* Read_float_Config(const char* itme, const char* subitem, float* value)
	{
		return Read_ulong_Config(itme, subitem, (UINT32*)value);
	}
	char* Write_float_Config(const char* itme, const char* subitem, float value)
	{
		UINT32 value_ = *(UINT32*)&value;
		return Write_ulong_Config(itme, subitem, value_);
	}

	char* Read_bool_Config(const char* itme, const char* subitem, bool* value)
	{
		UINT32 value_ = 0;
		char*ret= Read_ulong_Config(itme, subitem, (UINT32*)&value_);
		*value = value_;
		return ret;
	}
	char* Write_bool_Config(const char* itme, const char* subitem, bool value)
	{
		UINT32 value_ = value;
		return Write_ulong_Config(itme, subitem, value_);
	}

	__int64 TextTovalue(char* Text)
	{

		char* end;
		__int64 axa = stack_hidden_call(strtoul, Text, &end, 16);

		return axa;
	}
	char* valueToText(int value, char* Text)
	{

		char* axa = stack_hidden_call(_ultoa, value, Text, 16);

		return axa;
	}

	char* insert_text(char* text_begin, char* insert_pos, char* text_end, char* text)
	{
		size_t end_len = strlen(text_begin);

		size_t text_len = strlen(text);
		size_t text_end_len = 0;


		size_t new_size = text_len + end_len + +0x20;

		char* new_data = (char*)stack_hidden_call(malloc, new_size);
		memset(new_data, 0, new_size);
		if (insert_pos)
		{
			char* new_data_begin = (char*)stack_hidden_call(malloc, new_size);
			memset(new_data_begin, 0, new_size);
			size_t new_data_begin_len = 0;
			if (insert_pos >= text_begin)
				new_data_begin_len = insert_pos - text_begin;
			memmove(new_data_begin, text_begin, new_data_begin_len);

			stack_hidden_call(sprintf, new_data,VMProtectDecryptStringA("%s%s%s\0"), new_data_begin, text, text_end);
			memset(new_data_begin, 0, new_size);
			stack_hidden_call_v(free, new_data_begin);


		}
		else
		{

			stack_hidden_call(sprintf, new_data,VMProtectDecryptStringA("%s%s\0"), text_begin, text);

		}
		return new_data;
	}


	xs_config()
	{
		ConfigHandle = NULL;
		Data = NULL;
		size = 0;

		ConfigPath = (char*)VMProtectDecryptStringA("ux.ini");
		InitConfig();
	}
	~xs_config()
	{
		OffConfig();
	}
};