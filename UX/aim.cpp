#include "cheat_data.h"
#include "xsgui.h"
#include "hook_call.h"
#include<cmath>
#include "VMProtectSDK.h"

enum _aim_bone
{
    _head = 0,
    _neck = 1,

    _chest = 2,
    _Pelvic = 3
};
bool fire_auto = 0;

UINT64 ole_aim_call = 0;
_aim* _aim_this = NULL;
UINT64 local_aim_this;

UINT32 hook_aim_san_size = 0;

inline PVOID hook_aim_magic_call(UINT64 _rcx, UINT64 _rdx, UINT64 _r8) __attribute((__annotate__(("nosplit"))));
inline PVOID hook_aim_magic_call(UINT64 _rcx, UINT64 _rdx, UINT64 _r8) __attribute((__annotate__(("nofla"))));
inline PVOID hook_aim_magic_call(UINT64 _rcx, UINT64 _rdx, UINT64 _r8)
{
	xsFloat3* ret = (*(xsFloat3 * (__stdcall*)(UINT64, UINT64, UINT64))(ole_aim_call)) (_rcx, _rdx, _r8);
	if ((_aim_this->aim_length_enable2|| _aim_this->aim_san_size2) && local_aim_this == _rcx) {
		xsFloat3 aim_pos;
		if (!_aim_this->aim_san_size2)
		{
			*ret = _aim_this->aim_bone_pos3[0];
			ret->y += 0.04f;
			//if (!_aim_this->aim_no_bone)
			{
				 aim_pos = _aim_this->aim_bone_pos4[0];
				//aim_pos.y = _aim_this->aim_bone_pos3.y;
				__asm {
					movss xmm6, aim_pos.x
					movss xmm7, aim_pos.y
					movss xmm8, aim_pos.z
				}
				__asm {
					movss aim_pos + 0x30 - 4, xmm6
					movss aim_pos + 0x40 - 4, xmm7
					movss aim_pos + 0x50 - 4, xmm8
				}
			}
		}
		else
		{
			if (hook_aim_san_size >= _aim_this->aim_san_size2)
				hook_aim_san_size = 0;
			
			*ret = _aim_this->aim_bone_pos3[hook_aim_san_size];
			ret->y += 0.04f;
			//if (!_aim_this->aim_no_bone)
			{
				 aim_pos = _aim_this->aim_bone_pos4[hook_aim_san_size];
				//aim_pos.y = _aim_this->aim_bone_pos3.y;
				__asm {
					movss xmm6, aim_pos.x
					movss xmm7, aim_pos.y
					movss xmm8, aim_pos.z
				}
				__asm {
					movss aim_pos + 0x30 - 4, xmm6
					movss aim_pos + 0x40 - 4, xmm7
					movss aim_pos + 0x50 - 4, xmm8
				}
			}

			hook_aim_san_size++;
		}
	}
	return (PVOID)ret;
}

UINT64 ole_fire_call = 0;

//inline void aim_fire_call() __attribute((__annotate__(("nobcf"))));
//inline void aim_fire_call() __attribute((__annotate__(("nosub"))));
//inline void aim_fire_call() __attribute((__annotate__(("nosplit"))));
//inline void aim_fire_call() __attribute((__annotate__(("nofla"))));
inline void aim_fire_call()
{
	if (fire_auto)
	{
		__asm {
			mov rax, [rsp]
			add rax, 0x77
			mov[rsp], rax
			mov rax, 01
			ret
		}
	}
	__asm {
		mov al, 01
		ret
	}
}

void _aim::aim_hook_Init(UINT64 magic_call, UINT64 fire_call)
{
    if (ole_aim_call)
        return;
    _aim_this = this;
    ole_aim_call = *(UINT64*)magic_call;
    hook_up(magic_call, (UINT64)&hook_aim_magic_call, 1);

    ole_fire_call = *(UINT64*)fire_call;
    hook_up(fire_call, (UINT64)&aim_fire_call, 1);
}

inline bool _aim::WorldToScreen(xsFloat3* entity_position, xsFloat2* screen_position)
{
    float w = *(matrix + 2) * entity_position->x + *(matrix + 6) * entity_position->y + *(matrix + 10) * entity_position->z + *(matrix + 14);//瞄准中间0.00//s3
    if (w < 0.001f)
        return 0;
    // 2 6 10 14

    screen_position->x = window->x / 2.0f;
    screen_position->y = window->y / 2.0f;

    screen_position->x *= 1.0f + ((*(matrix)*entity_position->x + *(matrix + 4) * entity_position->y + *(matrix + 8) * entity_position->z + *(matrix + 12)) / w) * *(matrix + 16);//0.00s1
    screen_position->y *= 1.0f - ((*(matrix + 1) * entity_position->x + *(matrix + 5) * entity_position->y + *(matrix + 9) * entity_position->z + *(matrix + 13)) / w) * *(matrix + 21);//1.33 //s2

    return 1;
}


char* aim_bone_name[] = {
	(char*)VMProtectDecryptStringA("Head"),//0
(char*)VMProtectDecryptStringA("Body"),//1
(char*)VMProtectDecryptStringA("Hand"),//2
(char*)VMProtectDecryptStringA("Leg"),//3
(char*)VMProtectDecryptStringA("WeakPoint1"),//4
(char*)VMProtectDecryptStringA("WeakPoint2"),//5
(char*)VMProtectDecryptStringA("WeakPoint3"),//6
(char*)VMProtectDecryptStringA("WeakPoint4"),//7
(char*)VMProtectDecryptStringA("WeakPointSkillProxy1"),//8
(char*)VMProtectDecryptStringA("WeakPointSkillProxy2"),//9
(char*)VMProtectDecryptStringA("WeakPointArmor1"),//10
(char*)VMProtectDecryptStringA("WeakPointArmor2")//11
};
inline float myAbs(float x) {
	return (x < (float)0.0f) ? -x : x;
}

inline void _aim::size(_entity_data& data)
{
	//data.weapon.init_aim_pos();
	if (data.bone_count&&data.bone_this)
    {
        int bone_id = -1;
		int bone_name_id = -1;
        for (size_t i = 0; i < 12; i++)
        {
			if (aim_weak_enable)
				if (aim_bone[i].value >= 4 && aim_bone[i].value <= 11)
					continue;
            bone_id = data.get_bone_name_this_str(aim_bone_name[aim_bone[i].value]);
			bone_name_id = aim_bone[i].value;
            if (bone_id != -1)
                break;
        }
        xsFloat3 xyz;
		if (bone_id != -1)
		{

			xyz = data.get_bone_this_position(bone_id);
			if (!strcmp(data.name, VMProtectDecryptStringA("Mini Tank")) && bone_name_id == 1)
			{
				xsFloat3 xyz2 = *data.position;
				xyz2.x = xyz.x;
				xyz2.z = xyz.z;
				data.weapon.get_aim_pos_2(xyz2);
			}
			else if (!strcmp(data.name, VMProtectDecryptStringA("Armored Wagon")) && bone_name_id == 4)
			{
				xyz -= xsFloat3(2.0f, 0.0f, 2.0f);
			}
			else if (!strcmp(data.name, VMProtectDecryptStringA("*Train")))
			{
				if (bone_name_id == 4 || bone_name_id == 5 || bone_name_id == 6)
				{
					xyz += xsFloat3(0.0f, 2.6f, -2.0f);

					xsFloat3 xyz2 = xyz;
					xyz2 -= xsFloat3(0.0f, 0.0f, -3.0f);
					//xyz2.z = xyz.z;
					data.weapon.get_aim_pos_2(xyz2);
				}
				else if (bone_name_id == 7)
				{
					xyz += xsFloat3(-0.4f, 2.6f, -2.0f);

					xsFloat3 xyz2 = xyz;
					xyz2 -= xsFloat3(0.0f, 0.0f, -3.0f);
					//xyz2.z = xyz.z;
					data.weapon.get_aim_pos_2(xyz2);
				}
				else if (bone_name_id == 8)
				{
					xyz += xsFloat3(0.0f, 1.0f, 0.0f);

					xsFloat3 xyz2 = *data.position;
					xyz2.x = xyz.x;
					xyz2.z = xyz.z;
					data.weapon.get_aim_pos_2(xyz2);
				}
			}
			else if (!strcmp(data.name, VMProtectDecryptStringA("Trap")))
			{
				xsFloat3 xyz2 = xyz;
				xyz2.y -= 0.3f;
				xyz.y += 0.3f;
				data.weapon.get_aim_pos_2(xyz2);

			}
			data.weapon.get_aim_pos(xyz);

			//xsFloat3 xyz2 = data.get_bone_this_position_2(bone_id);
			//data.weapon.get_aim_pos_2(xyz2);
			//if (!aim_gun_enable && !aim_san_enable && (aim_mouse_enable || aim_enable))
			{
				xsFloat2 p1;
				if (WorldToScreen(&xyz, &p1))
					data.weapon.bone_pos(p1);
			}
		}

	}
	else if (data.enemy)
	{
		xsFloat3* aa = data.call_wrj_aim();
		if (is_addr((UINT64)aa))
		{
			xsFloat3 xyz = *aa;

			data.weapon.get_aim_pos(xyz);

			//if (!aim_gun_enable && !aim_san_enable && (aim_mouse_enable || aim_enable))
			{
				xsFloat2 p1;
				if (WorldToScreen(&xyz, &p1))
				{
					data.weapon.bone_pos(p1);
					data.weapon.aim_no_bone = 1;
				}
			}
		}
	}
}


inline float _aim::aim_count_length(_weapon_data& data)
{

    float x = fabs(data.aim_bone_pos.x - (window->x / 2.0f));
    float y = fabs(data.aim_bone_pos.y - (window->y / 2.0f));
    
    float length = sqrtf(x * x + y * y);

    return length;
}


inline void _aim::aim_mouse_mov()
{

    xsFloat2 Target;
    Target.x = (aim_bone_pos2.x - window->x / 2.0F) / 10.0F;
    Target.y = (aim_bone_pos2.y - window->y / 2.0F) / 10.0F;
    if (Target.x < 1.0f && Target.x > 0.5f) Target.x = 1.0f;
    if (Target.x > -1.0f && Target.x < -0.5f) Target.x = -1.0f;
    if (Target.y < 1.0f && Target.y > 0.5f) Target.y = 1.0f;
    if (Target.y > -1.0f && Target.y < -0.5f) Target.y = -1.0f;

    
    {//1,0右 0,1下
        stack_hidden_call_v(mouse_event,MOUSEEVENTF_MOVE, Target.x, Target.y, NULL, NULL);
    }
}

 UINT32 bullets = 0;
 UINT32 bullets_ii = 0;
 inline UINT32 _aim::aim_switch_target_call(_weapon_data& data,UINT32 ii)
{

    if(ii)
    {
        if (*data.fire_bullets > bullets)
        {
            if ((*data.fire_bullets - bullets) >= aim_switch_target.value)
            {
                bullets = *data.fire_bullets;
                bullets_ii += 1;
               
                bullets_ii = bullets_ii % ii;
                return bullets_ii;
            }
        }
        else if(!*data.fire_bullets)//if (aim_switch_target_enable)
        {
            bullets = 0;
        }
		bullets_ii = bullets_ii % ii;
        return bullets_ii;
    }
    return 0;
}

inline void _aim::aim_aotu_list_( _entity_data& data, _weapon_data& data2)
{
    static UINT32 fire_bullets_2 = 0;
    if (aim_aotu_list_enable)
        if (*data2.fire_bullets > fire_bullets_2)
        {
            data.UID[4] += 1;
            fire_bullets_2 = *data2.fire_bullets;
            if (data.UID[4] > aim_aotu_list.value)
                data.UID[0] = 1;
        }
        else if (!*data2.fire_bullets)//if (aim_switch_target_enable)
        {
            fire_bullets_2 = 0;
        }
}

inline void _aim::aim_init_aim()
{
    if (aim_length_enable)
        aim_length_enable = 0;
    if (aim_mouse_length_enable)
        aim_mouse_length_enable = 0;
}

inline float _aim::aim_init_length()
{
    float length_ = 0;
    if (aim_mouse_enable && aim_enable)
        length_ = max(aim_mouse_length.value, aim_length.value);
    else if (aim_mouse_enable)
        length_ = aim_mouse_length.value;
    else if (aim_enable)
        length_ = aim_length.value;
    return length_;
}

static float length_ = 0;
inline void _aim::aim__(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc)
{
	
	if (type)
	{
		length_ = aim_init_length();
		aim_init_aim();
		if (!Player_enable)
			return;
		int b = -1;
		int b2 = -1;

		for (size_t i = 1, iic = 0; i < c->Size; i++)
		{
			if (!c->Data[i].bone_this || !c->Data[i].bone_count || c->Data[i].name_list == 1 || !c->Data[i].enemy || !*c->Data[i].blood || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;


				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;

				float length_2 = aim_count_length(c->Data[i].weapon);
				if (length_ > length_2)
				{
					length_ = length_2;
					b2 = i;
				}
			

				iic += 1;

		}
		if (b2 != -1)
			b = b2;

		if (b != -1)
		{
			c->Data[b].aim_pitch = c->Data[b].weapon.aim_bone_pos_bool;
			aim_bone_pos2 = c->Data[b].weapon.aim_bone_pos;

			c->Data[b].aim_pitch_position = aim_bone_pos2;
			if (c->Data[b].weapon.aim_no_bone)
				aim_no_bone = 1;
			else
				aim_no_bone = 0;
			aim_bone_pos4[0] = *c->Data[b].position;
			aim_bone_pos3[0] = c->Data[b].weapon.aim;
			//aim_bone_pos3.y += 0.02f;
			if (aim_mouse_enable&&c->Data[b].weapon.aim_bone_pos_bool)//
			{
				aim_mouse_length_enable = 1;
			}
			if (aim_enable)
			{
				aim_length_enable = 1;
			}

			aim_aotu_list_(c->Data[b], cc);
		}

	}
	else
	{
		if (!npc_enable)
		{
			if (aim_length_enable)
				aim_length_enable2 = 1;
			else
				aim_length_enable2 = 0;

			return;
		}
		int b = -1;
		int b2 = -1;


		for (size_t i = 0, iic = 0; i < c->Size; i++)
		{
			if (c->Data[i].name_list == 1 || !c->Data[i].enemy || *c->Data[i].blood <= 1 || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;

				float length_2 = aim_count_length(c->Data[i].weapon);
				if (length_ > length_2)
				{
					length_ = length_2;
					b2 = i;
				}


			if (b == -1)
			{

					iic += 1;
			}

		}

		if (b2 != -1)
			b = b2;

		if (b != -1)
		{
			c->Data[b].aim_pitch = c->Data[b].weapon.aim_bone_pos_bool;
			aim_bone_pos2 = c->Data[b].weapon.aim_bone_pos;

			c->Data[b].aim_pitch_position = aim_bone_pos2;
			if (c->Data[b].weapon.aim_no_bone)
				aim_no_bone = 1;
			else
				aim_no_bone = 0;

			aim_bone_pos4[0] = *c->Data[b].position;
			aim_bone_pos3[0] = c->Data[b].weapon.aim;
			if (fabs(aim_bone_pos4[0].y - aim_bone_pos3[0].y) < 0.2f)
				aim_bone_pos4[0].y += 10.f;
			else
				aim_bone_pos4[0].y = aim_bone_pos3[0].y;
			if (aim_no_bone)
				aim_bone_pos4[0].y -= 5.f;

			if (c->Data[b].weapon.aim_bone2_bool)
				aim_bone_pos4[0] = c->Data[b].weapon.aim2;

			//aim_bone_pos3.y += 0.02f;
			if (aim_mouse_enable&&c->Data[b].weapon.aim_bone_pos_bool)//
			{
				aim_mouse_length_enable = 1;
			}
			if (aim_enable)
			{
				aim_length_enable = 1;
			}

			aim_aotu_list_(c->Data[b], cc);
		}
		if (aim_length_enable)
			aim_length_enable2 = 1;
		else
			aim_length_enable2 = 0;

	}
}
inline void _aim::aim_gun_(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc)
{
	if (type)
	{
		aim_init_aim();

		if (!Player_enable)
			return;
		int b = -1;

		UINT32 ii = 0;
		if (aim_switch_target_enable)
		{
			for (size_t i = 1; i < c->Size; i++)
			{
				if (!c->Data[i].bone_this || !c->Data[i].bone_count || c->Data[i].name_list == 1 || !c->Data[i].enemy || !*c->Data[i].blood || c->Data[i].distance > aim_distance.value /*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
					continue;
				size(c->Data[i]);
				if (!c->Data[i].weapon.aim_bone_bool)
					continue;
				if (c->Data[i].name_list == 2)
				{
					ii += 1;
					break;
				}
				ii += 1;
			}
			if (c->Data[ii].name_list != 2)
				ii = aim_switch_target_call(cc, ii);
			if (ii == 0)
				return;
		}
		for (size_t i = ii, iic = ii; i < c->Size; i++)
		{
			if (!c->Data[i].bone_this || !c->Data[i].bone_count || c->Data[i].name_list == 1 || !c->Data[i].enemy || !*c->Data[i].blood || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

			if (!aim_switch_target_enable)
				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;


			if (aim_gun_enable)
			{
				b = i;
				break;
			}
			else
				iic += 1;

		}


		if (b != -1)
		{
			c->Data[b].aim_pitch = c->Data[b].weapon.aim_bone_pos_bool;
			aim_bone_pos2 = c->Data[b].weapon.aim_bone_pos;

			if (c->Data[b].weapon.aim_no_bone)
				aim_no_bone = 1;
			else
				aim_no_bone = 0;
			aim_bone_pos4[0] = *c->Data[b].position;
			aim_bone_pos3[0] = c->Data[b].weapon.aim;
			//aim_bone_pos3.y += 0.02f;

			if (aim_gun_enable)//魔术子弹
			{
				aim_length_enable = 1;
			}

			aim_aotu_list_(c->Data[b], cc);
		}
	}
	else
	{
		if (!npc_enable)
		{
			if (aim_length_enable)
				aim_length_enable2 = 1;
			else
				aim_length_enable2 = 0;

			return;
		}
		if (aim_length_enable)
		{
			aim_length_enable2 = 1;
			return;
		}
		else
			aim_length_enable2 = 0;
		int b = -1;


		UINT32 ii = 0;
		if (aim_switch_target_enable)
		{
			for (size_t i = 0; i < c->Size; i++)
			{
				if (c->Data[i].name_list == 1 || !c->Data[i].enemy || *c->Data[i].blood <= 1 || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
					continue;
				size(c->Data[i]);
				if (!c->Data[i].weapon.aim_bone_bool)
					continue;
				if (c->Data[i].name_list == 2)
				{
					ii += 1;
					break;
				}
				ii += 1;
			}
			if (c->Data[ii].name_list != 2)
				ii = aim_switch_target_call(cc, ii);

		}
		for (size_t i = ii, iic = ii; i < c->Size; i++)
		{
			if (c->Data[i].name_list == 1 || !c->Data[i].enemy || *c->Data[i].blood <= 1 || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

			if (!c->Data[i].weapon.aim_bone_pos_bool)
				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;



				if (aim_gun_enable )
				{
					b = i;
					break;
				}
				else
					iic += 1;
			

		}


		if (b != -1)
		{
			c->Data[b].aim_pitch = c->Data[b].weapon.aim_bone_pos_bool;
			aim_bone_pos2 = c->Data[b].weapon.aim_bone_pos;

			c->Data[b].aim_pitch_position = aim_bone_pos2;
			if (c->Data[b].weapon.aim_no_bone)
				aim_no_bone = 1;
			else
				aim_no_bone = 0;

			aim_bone_pos4[0] = *c->Data[b].position;
			aim_bone_pos3[0] = c->Data[b].weapon.aim;
			if (fabs(aim_bone_pos4[0].y - aim_bone_pos3[0].y) < 0.2f)
				aim_bone_pos4[0].y += 10.f;
			else
				aim_bone_pos4[0].y = aim_bone_pos3[0].y;
			if (aim_no_bone)
				aim_bone_pos4[0].y -= 5.f;

			if (c->Data[b].weapon.aim_bone2_bool)
				aim_bone_pos4[0] = c->Data[b].weapon.aim2;

			//aim_bone_pos3.y += 0.02f;

			if (aim_gun_enable)//魔术子弹
			{
				aim_length_enable = 1;
			}

			aim_aotu_list_(c->Data[b], cc);
		}
		if (aim_length_enable)
			aim_length_enable2 = 1;
		else
			aim_length_enable2 = 0;

	}
}
inline void _aim::aim_san_(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc)
{
	if (type)
	{
		//初始化

		aim_san_size = 0;
		//return;
		if (!Player_enable)
			return;


		for (size_t i = 1; i < c->Size; i++)
		{
			if (!c->Data[i].bone_this || !c->Data[i].bone_count || c->Data[i].name_list == 1 || !c->Data[i].enemy || !*c->Data[i].blood || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

			if (!aim_switch_target_enable)
				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;
			if (aim_san_enable)
			{
				c->Data[i].aim_pitch = c->Data[i].weapon.aim_bone_pos_bool;
				aim_bone_pos2 = c->Data[i].weapon.aim_bone_pos;

				c->Data[i].aim_pitch_position = aim_bone_pos2;
				if (c->Data[i].weapon.aim_no_bone)
					aim_no_bone = 1;
				else
					aim_no_bone = 0;
				aim_bone_pos4[aim_san_size] = *c->Data[i].position;
				aim_bone_pos3[aim_san_size] = c->Data[i].weapon.aim;

				aim_san_size++;
				continue;
			}


		}


	}
	else
	{
		if (!npc_enable)
		{

			if (aim_san_size)
				aim_san_size2 = aim_san_size;
			else
				aim_san_size2 = 0;
			return;
		}
		if (aim_san_size)
		{
			aim_san_size2 = aim_san_size;
			return;
		}



		for (size_t i = 0; i < c->Size; i++)
		{
			if (c->Data[i].name_list == 1 || !c->Data[i].enemy || *c->Data[i].blood <= 1 || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

			if (!aim_switch_target_enable)
				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;

			if (aim_san_enable)
			{
				c->Data[i].aim_pitch = c->Data[i].weapon.aim_bone_pos_bool;
				aim_bone_pos2 = c->Data[i].weapon.aim_bone_pos;

				c->Data[i].aim_pitch_position = aim_bone_pos2;
				if (c->Data[i].weapon.aim_no_bone)
					aim_no_bone = 1;
				else
					aim_no_bone = 0;

				aim_bone_pos4[aim_san_size] = *c->Data[i].position;
				aim_bone_pos3[aim_san_size] = c->Data[i].weapon.aim;
				if (fabs(aim_bone_pos4[aim_san_size].y - aim_bone_pos3[aim_san_size].y) < 0.2f)
					aim_bone_pos4[aim_san_size].y += 10.f;
				else
					aim_bone_pos4[aim_san_size].y = aim_bone_pos3[aim_san_size].y;
				if (aim_no_bone)
					aim_bone_pos4[aim_san_size].y -= 5.f;

				if (c->Data[i].weapon.aim_bone2_bool)
					aim_bone_pos4[aim_san_size] = c->Data[i].weapon.aim2;


				aim_san_size++;
				continue;
			}

		}


		if (aim_san_size)
			aim_san_size2 = aim_san_size;
		else
			aim_san_size2 = 0;
	}
}
void _aim::aim_player(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc)
{
	static float length_ = 0;
	if (type)
	{
		//初始化
		length_ = aim_init_length();
		aim_init_aim();
		aim_san_size = 0;

		//return;
		if (!Player_enable)
			return;

		int b = -1;
		int b2 = -1;
		UINT32 ii = 0;
		if (aim_switch_target_enable)
		{
			for (size_t i = 1; i < c->Size; i++)
			{
				if (!c->Data[i].bone_this || !c->Data[i].bone_count || c->Data[i].name_list == 1 || !c->Data[i].enemy || !*c->Data[i].blood || c->Data[i].distance > aim_distance.value /*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
					continue;
				size(c->Data[i]);
				if (!c->Data[i].weapon.aim_bone_bool)
					continue;
				ii += 1;
			}
			ii = aim_switch_target_call(cc, ii);
		}
		for (size_t i = 1, iic = 0; i < c->Size; i++)
		{
			if (!c->Data[i].bone_this || !c->Data[i].bone_count || c->Data[i].name_list == 1 || !c->Data[i].enemy || !*c->Data[i].blood || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;
			//if (!c->Data[i].weapon.aim_bone_pos_bool)
			if (!c->Data[i].weapon.aim_bone_bool)
				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;
			if (aim_san_enable)
			{
				c->Data[i].aim_pitch = c->Data[i].weapon.aim_bone_pos_bool;
				aim_bone_pos2 = c->Data[i].weapon.aim_bone_pos;

				c->Data[i].aim_pitch_position = aim_bone_pos2;
				if (c->Data[i].weapon.aim_no_bone)
					aim_no_bone = 1;
				else
					aim_no_bone = 0;
				aim_bone_pos4[aim_san_size] = *c->Data[i].position;
				aim_bone_pos3[aim_san_size] = c->Data[i].weapon.aim;

				aim_san_size++;
				continue;
			}

			float length_2 = aim_count_length(c->Data[i].weapon);
			if (length_ > length_2)
			{
				length_ = length_2;
				b2 = i;
			}

			if (c->Data[i].name_list == 2)
			{
				b = i;
				//goto heim1;
			}
			if (aim_gun_enable && !aim_switch_target_enable)
			{
				b = i;
				//goto heim1;
			}
			if (aim_gun_enable && aim_switch_target_enable && iic == ii)
			{
				b = i;
				//goto heim1;
			}
			else
				iic += 1;

		}
		if (b2 != -1)
			b = b2;

		if (b != -1)
		{
		heim1:
			c->Data[b].aim_pitch = c->Data[b].weapon.aim_bone_pos_bool;
			aim_bone_pos2 = c->Data[b].weapon.aim_bone_pos;
			c->Data[b].aim_pitch_position = aim_bone_pos2;
			if (c->Data[b].weapon.aim_no_bone)
				aim_no_bone = 1;
			else
				aim_no_bone = 0;
			aim_bone_pos4[0] = *c->Data[b].position;
			aim_bone_pos3[0] = c->Data[b].weapon.aim;
			//aim_bone_pos3.y += 0.02f;
			if (aim_mouse_enable&&c->Data[b].weapon.aim_bone_pos_bool)//
			{
				aim_mouse_length_enable = 1;
			}
			if (aim_enable)
			{
				aim_length_enable = 1;
			}
			if (aim_gun_enable)//魔术子弹
			{
				aim_length_enable = 1;
			}

			aim_aotu_list_(c->Data[b], cc);
		}
	}
	else
	{
		if (!npc_enable)
		{
			if (aim_length_enable)
				aim_length_enable2 = 1;
			else
				aim_length_enable2 = 0;

			if (aim_san_size)
				aim_san_size2 = aim_san_size;
			else
				aim_san_size2 = 0;
			return;
		}
		if (aim_san_size)
		{
			aim_san_size2 = aim_san_size;
			return;
		}

		int b = -1;
		int b2 = -1;

		UINT32 ii = 0;
		if (aim_switch_target_enable)
		{
			for (size_t i = 0; i < c->Size; i++)
			{
				if (c->Data[i].name_list == 1 || !c->Data[i].enemy || *c->Data[i].blood <= 1 || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
					continue;
				size(c->Data[i]);
				if (!c->Data[i].weapon.aim_bone_bool)
					continue;
				ii += 1;
			}
			ii = aim_switch_target_call(cc, ii);
		}
		for (size_t i = 0, iic = 0; i < c->Size; i++)
		{
			if (c->Data[i].name_list == 1 || !c->Data[i].enemy || *c->Data[i].blood <= 1 || c->Data[i].distance > aim_distance.value/*|| !c->Data[i].bone  || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

			if (!c->Data[i].weapon.aim_bone_bool)
				size(c->Data[i]);
			if (!c->Data[i].weapon.aim_bone_bool)
				continue;
			if (aim_san_enable)
			{
				c->Data[i].aim_pitch = c->Data[i].weapon.aim_bone_pos_bool;
				aim_bone_pos2 = c->Data[i].weapon.aim_bone_pos;

				c->Data[i].aim_pitch_position = aim_bone_pos2;
				if (c->Data[i].weapon.aim_no_bone)
					aim_no_bone = 1;
				else
					aim_no_bone = 0;

				aim_bone_pos4[aim_san_size] = *c->Data[i].position;
				aim_bone_pos3[aim_san_size] = c->Data[i].weapon.aim;
				if (fabs(aim_bone_pos4[aim_san_size].y - aim_bone_pos3[aim_san_size].y) < 0.2f)
					aim_bone_pos4[aim_san_size].y += 10.f;
				else
					aim_bone_pos4[aim_san_size].y = aim_bone_pos3[aim_san_size].y;
				if (aim_no_bone)
					aim_bone_pos4[aim_san_size].y -= 5.f;

				if (c->Data[i].weapon.aim_bone2_bool)
					aim_bone_pos4[aim_san_size] = c->Data[i].weapon.aim2;

				
				aim_san_size++;
				continue;
			}

			float length_2 = aim_count_length(c->Data[i].weapon);
			if (length_ > length_2)
			{
				length_ = length_2;
				b2 = i;
			}

			if (b == -1)
			{
				if (c->Data[i].name_list == 2)
				{
					b = i;
					//goto heim2;
				}
				if (aim_gun_enable && !aim_switch_target_enable)
				{
					b = i;
					//goto heim2;
				}

				if (aim_gun_enable && aim_switch_target_enable && iic == ii)
				{
					b = i;
					//goto heim2;
				}
				else
					iic += 1;
			}

		}

		if (b2 != -1)
			b = b2;

		if (b != -1)
		{
		heim2:
			c->Data[b].aim_pitch = c->Data[b].weapon.aim_bone_pos_bool;
			aim_bone_pos2 = c->Data[b].weapon.aim_bone_pos;
			c->Data[b].aim_pitch_position = aim_bone_pos2;
			if (c->Data[b].weapon.aim_no_bone)
				aim_no_bone = 1;
			else
				aim_no_bone = 0;

			aim_bone_pos4[0] = *c->Data[b].position;
			aim_bone_pos3[0] = c->Data[b].weapon.aim;
			if (fabs(aim_bone_pos4[0].y - aim_bone_pos3[0].y) < 0.2f)
				aim_bone_pos4[0].y += 10.f;
			else
				aim_bone_pos4[0].y = aim_bone_pos3[0].y;
			if (aim_no_bone)
				aim_bone_pos4[0].y -= 5.f;

			if (c->Data[b].weapon.aim_bone2_bool)
				aim_bone_pos4[0] = c->Data[b].weapon.aim2;

			//aim_bone_pos3.y += 0.02f;
			if (aim_mouse_enable&&c->Data[b].weapon.aim_bone_pos_bool)//
			{
				aim_mouse_length_enable = 1;
			}
			if (aim_enable)
			{
				aim_length_enable = 1;
			}
			if (aim_gun_enable)//魔术子弹
			{
				aim_length_enable = 1;
			}

			aim_aotu_list_(c->Data[b], cc);
		}
		if (aim_length_enable)
			aim_length_enable2 = 1;
		else
			aim_length_enable2 = 0;

		if (aim_san_size)
			aim_san_size2 = aim_san_size;
		else
			aim_san_size2 = 0;
	}

}

 inline void _aim::aim_auto_fire(_weapon_data& data)
 {

     if (aim_length_enable2|| aim_san_size2)
         fire_auto = 1;
     else
         fire_auto = 0;
     if(aim_stop_fire_enable&& *data.fire_bullets> aim_stop_fire.value)
         fire_auto = 0;

 }

  void _aim::aim_cheat(_entity_data& data)
 {
	  if (is_addr((UINT64)data.weapon.table_this))
		  if (is_addr(*data.weapon.table_this))
			  local_aim_this = *data.weapon.table_this;

     if (aim_mouse_enable && aim_mouse_length_enable && (*data.weapon.mirror|| *data.weapon.fire_bullets))
         aim_mouse_mov();
     if (aim_auto_fire_enable)
         aim_auto_fire(data.weapon);
     else
         fire_auto = 0;
 }

  void _aim::mouse_list(xs_DataFrame<_entity_data>* c, _entity_data& a, UINT32 type, xs_DataFrame<_name_list_data>* b)//name_list系统
 {
     if (type)
     {
         UINT64 aa = a.weapon.get_entity();
             for (size_t i = 1; i < c->Size; i++)
             {
                 if (*c->Data[i].table_this == aa)
                 {
                     for (size_t e = 0; e < b->Size; e++)
                     {
                         if (b->Data[e].cmp_uid(c->Data[i].UID) != (char)-1)
                         {
                             if (!b->Data[e].set_list())
                                 b->pop_back(e);
                             goto G_ret;
                         }
                     }
                     _name_list_data v;
                     v.set_uid(c->Data[i].UID);
                     v.set_list();
                     b->push_back(v);
                     
                 }
             }
     }
     else
     {
         UINT64 aa = a.weapon.get_entity();
         for (size_t i = 0; i < c->Size; i++)
         {

			 if (*c->Data[i].table_this == aa)
			 {
				 *c->Data[i].UID += 1;
				 if (*c->Data[i].UID > 2)
					 *c->Data[i].UID = 0;
				 goto G_ret;
			 }
			 else
			 {
				 if (c->Data[i].aim_pitch)
				 {
					 *c->Data[i].UID += 1;
					 if (*c->Data[i].UID > 2)
						 *c->Data[i].UID = 0;
					 goto G_ret;
				 }
			 }
         }
     }
 G_ret:
     return;
 }

  void _aim::no_mouse_list(xs_DataFrame<_entity_data>* c)//name_list系统
  {
	  {

		  for (size_t i = 0; i < c->Size; i++)
		  {
			  if (*c->Data[i].UID != 0)
			  {
				  *c->Data[i].UID = 0;
			  }
		  }
	  }
	  return;
  }

  void _aim::aim_config()
  {
	  xs_config iniop;

	  iniop.Read_float_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_distance="), &aim_distance.value);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\nPlayer_enable="), &Player_enable);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\nnpc_enable="), &npc_enable);


	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_weak_enable="), &aim_weak_enable);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_mouse_enable="), &aim_mouse_enable);
	  iniop.Read_float_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_mouse_length="), &aim_mouse_length.value);

	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_enable="), &aim_enable);
	  iniop.Read_float_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_length="), &aim_length.value);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_gun_enable="), &aim_gun_enable);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_auto_fire_enable="), &aim_auto_fire_enable);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_aotu_list_enable="), &aim_aotu_list_enable);

	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_aotu_list="), &aim_aotu_list.value);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_switch_target_enable="), &aim_switch_target_enable);


	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_switch_target="), &aim_switch_target.value);
	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_stop_fire_enable="), &aim_stop_fire_enable);

	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_stop_fire="), &aim_stop_fire.value);
	 

	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone0="), &aim_bone[0].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone1="), &aim_bone[1].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone2="), &aim_bone[2].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone3="), &aim_bone[3].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone4="), &aim_bone[4].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone5="), &aim_bone[5].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone6="), &aim_bone[6].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone7="), &aim_bone[7].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone8="), &aim_bone[8].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone9="), &aim_bone[9].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone10="), &aim_bone[10].value);
	  iniop.Read_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_bone11="), &aim_bone[11].value);

	  iniop.Read_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("\naim_list_key_enable="), &aim_list_key_enable);

	  iniop.SaveConfig();
	  iniop.OffConfig();
  }

  void _aim::aim_config_Write()
  {
	  xs_config iniop;

	  iniop.Write_float_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_distance="), aim_distance.value);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("Player_enable="), Player_enable);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("npc_enable="), npc_enable);


	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_weak_enable="), aim_weak_enable);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_mouse_enable="), aim_mouse_enable);
	  iniop.Write_float_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_mouse_length="), aim_mouse_length.value);

	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_enable="), aim_enable);
	  iniop.Write_float_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_length="), aim_length.value);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_gun_enable="), aim_gun_enable);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_auto_fire_enable="), aim_auto_fire_enable);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_aotu_list_enable="), aim_aotu_list_enable);

	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_aotu_list="), aim_aotu_list.value);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_switch_target_enable="), aim_switch_target_enable);




	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_switch_target="), aim_switch_target.value);
	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_stop_fire_enable="), aim_stop_fire_enable);

	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_stop_fire="), aim_stop_fire.value);


	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone0="), aim_bone[0].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone1="), aim_bone[1].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone2="), aim_bone[2].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone3="), aim_bone[3].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone4="), aim_bone[4].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone5="), aim_bone[5].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone6="), aim_bone[6].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone7="), aim_bone[7].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone8="), aim_bone[8].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone9="), aim_bone[9].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone10="), aim_bone[10].value);
	  iniop.Write_ulong_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_bone11="), aim_bone[11].value);

	  iniop.Write_bool_Config(VMProtectDecryptStringA("[aim]"), VMProtectDecryptStringA("aim_list_key_enable="), aim_list_key_enable);

	  iniop.SaveConfig();
	  iniop.OffConfig();
  }