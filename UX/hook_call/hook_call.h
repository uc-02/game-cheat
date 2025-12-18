#pragma once
#include <windows.h>

bool getModuleInformation(const wchar_t* name, UINT64* module_addr, UINT64* module_size = NULL); //x64


UINT64 findPattern(const wchar_t* module, const char* pattern, UINT32 pattern_size);
UINT64 findPattern(const UINT64 module_addr, const UINT64 _module_size, const char* pattern, UINT32 pattern_size);
UINT64 findPattern(const wchar_t* module, const UINT64 pattern, int conut);

ULONG64 Compile_PointAddr(ULONG64 offseAddr);

void hook_up(UINT64 hook_addr, UINT64 call_addr, UINT8 type);
void hook_un(UINT64 call_addr);


void stack_hidden_call_init();
UINT64 get_jmp_addr();
UINT64 get_stack_my();

inline void stack_switch_push()
{
    
    __asm {
        call get_stack_my
        mov[rax], rsp
        lea rsp, [rax]
    }

}
inline void stack_switch_pop()
{

    __asm {
        mov rsp, [rsp]
    }

}

template <class T, class ...Args>
inline auto back_jmp(T call_, Args... args)
{
	T call_o = call_;
	UINT64 ret_rbx = 0;
	__asm {

		call end2;

	end0:
		lea rbx, ret_rbx
			mov rax, [rsp]
			mov[rbx], rax

			call get_jmp_addr
			mov[rsp], rax

			jmp call_o


			end2 :
		pop rax
			mov call_, rax
	}
    return call_(args...);
}
template <class T, class ...Args>
inline auto stack_hidden_call(T call_, Args... args)
{


    auto ret_ = back_jmp(call_, args...);

    return ret_;
}

template <class T, class ...Args>
inline void stack_hidden_call_v(T call_, Args... args)
{

     back_jmp(call_, args...);

}
