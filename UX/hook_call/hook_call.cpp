#include "hook_call.h"
#include "xsgui.h"


 bool getModuleInformation(const wchar_t* name, UINT64* module_addr, UINT64* module_size) //x64
{
    UINT64 _PEB = __readgsqword(0x60);
    UINT64 _Ldr = *(UINT64*)(_PEB + 0x18);
    UINT64 _module_list = *(UINT64*)(_Ldr + 0x10);
    UINT64 _temp_module_list = _module_list;

    do
    {
        if(*(UINT64*)(_temp_module_list + 0x60))
            if (!wcscmp((wchar_t*)name, (wchar_t*)*(UINT64*)(_temp_module_list + 0x60)))
            {
                *module_addr = *(UINT64*)(_temp_module_list + 0x30);
                if (module_size)
                    *module_size = *(UINT64*)(_temp_module_list + 0x40);
                return 1;
            }

        _temp_module_list = *(UINT64*)_temp_module_list;
    } while (_module_list != _temp_module_list);

    return 0;
}

 UINT64 findPattern(const wchar_t* module, const char* pattern, UINT32 pattern_size)
{

    UINT64 module_addr, _module_size;
    if (!getModuleInformation(module, &module_addr, &_module_size))
        return 0;

    if (module_addr && _module_size) {
        auto start = (const char*)(module_addr);
        const auto end = start + _module_size;

        auto first = start;
        auto second = pattern;
        auto tail = pattern + pattern_size;

        while (first < end && (second != tail)) {
            if (*first == *second || *second == '?') {
                ++first;
                ++second;
            }
            else {
                first = ++start;
                second = pattern;
            }
        }

        if (second == tail)
            return (UINT64)start;
    }

    return 0;
}

 UINT64 findPattern(const UINT64 module_addr, const UINT64 _module_size,const char* pattern, UINT32 pattern_size)
 {


	 if (module_addr && _module_size) {
		 auto start = (const char*)(module_addr);
		 const auto end = start + _module_size;

		 auto first = start;
		 auto second = pattern;
		 auto tail = pattern + pattern_size;

		 while (first < end && (second != tail)) {
			 if (*first == *second || *second == '?') {
				 ++first;
				 ++second;
			 }
			 else {
				 first = ++start;
				 second = pattern;
			 }
		 }

		 if (second == tail)
			 return (UINT64)start;
	 }

	 return 0;
 }

 UINT64 findPattern(const wchar_t* module, const UINT64 pattern,int conut)
 {

     UINT64 module_addr, _module_size;
     if (!getModuleInformation(module, &module_addr, &_module_size))
         return 0;

     if (module_addr && _module_size) {
         auto start = (const UINT64*)(module_addr);
         const auto end = (UINT64*)(module_addr + _module_size);
         auto second = pattern;

         int conut_ = 0;
         auto start2 = start;

     go_while:;
         
         while (start < end && (*start != second)) {//俩种情况 1.扫完了 2.扫到了
             ++start;
         }
         if (start < end)
             if (*start == second)
             {
                 ++conut_; start2 = start;
             }

         if (conut_ == conut)
             return (UINT64)start2;
         if (!(start < end) && conut_)
             return (UINT64)start2;
         else if (start < end)
         {
             ++start;
             goto go_while;
         }
     }

     return 0;
 }


 ULONG64 Compile_PointAddr(ULONG64 offseAddr)
 {
     return (offseAddr & 0xFFFFFFFF00000000) + (ULONG32)((ULONG32)(offseAddr & 0x00000000FFFFFFFF) + *(ULONG32*)offseAddr + (ULONG32)4);
 }

 UINT64 jmp_addr = 0;
 UINT64 stack_my = 0;

 void stack_hidden_call_init()
 {
     if (jmp_addr)
         return;
     char bcd[50];
     int len2 = xs_Gui::char_to_value(VMProtectDecryptStringA("ff 23 c3\n"), bcd);
     UINT64 module_1 = findPattern(VMProtectDecryptStringW(L"KERNELBASE.dll"), bcd, len2);
     if (module_1)
         goto end;
     module_1 = findPattern(VMProtectDecryptStringW(L"gdi32full.dll"), bcd, len2);
     if (module_1)
         goto end;
     module_1 = findPattern(VMProtectDecryptStringW(L"D3DCOMPILER_47.dll"), bcd, len2);
     if (module_1)
         goto end;
     module_1 = findPattern(VMProtectDecryptStringW(L"WINTRUST.dll"), bcd, len2);
     if (module_1)
         goto end;

     len2 = xs_Gui::char_to_value(VMProtectDecryptStringA("ff 23\n"), bcd);
     module_1 = findPattern(VMProtectDecryptStringW(L"ntdll.dll"), bcd, len2);
     if (module_1)
         goto end;

 end:
     if (module_1)
     {
         
         jmp_addr = module_1;
     }
 }



 UINT64 get_jmp_addr()
 {
     return jmp_addr;
 }
 UINT64 get_stack_my()
 {
     return (UINT64) & stack_my;
 }

 struct _hook//
 {
     UINT64 call_addr;
     UINT64 hook_addr;
     UINT8  type;

     char   ole_pattern[30];
     UINT32 ole_pattern_size;
     _hook()
     {
         call_addr = 0;
         hook_addr = 0;
         type = 0;
         ole_pattern_size = 0;
         memset(ole_pattern,0,sizeof(ole_pattern));
     }
 };
 xs_DataFrame<_hook> hk;
 void hook_up(UINT64 hook_addr,UINT64 call_addr,UINT8 type)
 {
     _hook temp_hk;
     temp_hk.hook_addr = hook_addr;
     temp_hk.call_addr = call_addr;
     temp_hk.type = type;
     if (type == 1)
     {
         temp_hk.ole_pattern_size = 8;
         memmove(temp_hk.ole_pattern, (void*)hook_addr, temp_hk.ole_pattern_size);  

         DWORD oldProtect, oldProtect2;
         stack_hidden_call(VirtualProtect,(void*)hook_addr, temp_hk.ole_pattern_size, PAGE_EXECUTE_READWRITE, &oldProtect);
         *(UINT64*)hook_addr = call_addr;
         stack_hidden_call(VirtualProtect,(void*)hook_addr, temp_hk.ole_pattern_size, oldProtect, &oldProtect2);
     }
     hk.push_back(temp_hk);
 }

 void hook_un(UINT64 call_addr)
 {
     int conut = -1;
     for (size_t i = 0; i < hk.Size; i++)
     {
         if (hk[i].call_addr == call_addr)
         {
             conut = i; break;
         }
     }

     if (conut == -1)
         return;

     
     if (hk[conut].type == 1)
     {
         DWORD oldProtect, oldProtect2;
         stack_hidden_call(VirtualProtect,(void*)hk[conut].hook_addr, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
         memmove((void*)hk[conut].hook_addr, hk[conut].ole_pattern, hk[conut].ole_pattern_size);
         stack_hidden_call(VirtualProtect,(void*)hk[conut].hook_addr, 8, oldProtect, &oldProtect2);
     }
     hk.pop_back(conut);
 }


