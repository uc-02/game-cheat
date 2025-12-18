#include "cheat_data.h"
#include <stdio.h>
#include "config.h"

inline bool _esp::WorldToScreen(xsFloat3* entity_position, xsFloat2* screen_position)
{
	float w = *(matrix + 2) * entity_position->x + *(matrix + 6) * entity_position->y + *(matrix + 10) * entity_position->z + *(matrix + 14);//Ãé×¼ÖÐ¼ä0.00//s3
	if (w < 0.001f)
		return 0;
	// 2 6 10 14
	
	screen_position->x = window->x / 2.0f;
	screen_position->y = window->y / 2.0f;

	screen_position->x *= 1.0f + ((*(matrix)*entity_position->x + *(matrix + 4) * entity_position->y + *(matrix + 8) * entity_position->z + *(matrix + 12)) / w) * *(matrix + 16);//0.00s1
	screen_position->y *= 1.0f - ((*(matrix + 1) * entity_position->x + *(matrix + 5) * entity_position->y + *(matrix + 9) * entity_position->z + *(matrix + 13)) / w) * *(matrix + 21);//1.33 //s2

	return 1;
}


inline void _esp::box(_entity_data& data)
{
	if (!data.screen_s_bool || !data.screen_x_bool )
		return;
	UINT32 rgba = 0;
	if (data.name_list == 1)
		rgba = whitelist_box_rgba;
	else if (data.name_list == 2)
		rgba = blacklist_box_rgba;
	else
		rgba = box_rgba;

	{
				xs_Gui::GetForegroundDrawList()->AddRect(data.screen_s, data.screen_x, rgba, 2.0f);
	}
}


inline void _esp::bone(_entity_data& data)
{
	//if (data.bone)
	//{
	//	char a[20] = { 0 };
	//	sprintf(a, "%llx", data.table_this);
	//	xsFloat2 p2;

	//	if (WorldToScreen((xsFloat3*)data.position, &p2))
	//	{
	//		xs_Gui::GetForegroundDrawList()->AddText(NULL, 13.0f, p2, 0xffffffff, a);
	//	}
	//}
	//if (data.bone2_this)
		//for (size_t i =0; i < data.get_bone2_count_call(); i++)
		//{
		//	char a[80] = { 0 };
		//	char b[80] = { 0 };
		//	data.get_bone2_name_call(a, i);
		//	sprintf(b, u8"%d-%s", i,a);
		//	xsFloat2 p2;
		//	xsFloat3 p3 = data.get_bone2_pos_call(i);
		//	if (WorldToScreen(&p3, &p2))
		//	{
		//		xs_Gui::GetForegroundDrawList()->AddText(NULL, 13.0f, p2, 0xffffffff, b);
		//	}
		//}
		UINT32 Head = data.get_bone2_idx_call((char*)VMProtectDecryptStringA("Head"));
		UINT32 RightArm = data.get_bone2_idx_call((char*)VMProtectDecryptStringA("RightArm"));
		UINT32 LeftArm = data.get_bone2_idx_call((char*)VMProtectDecryptStringA("LeftArm"));
		UINT32 RightUpLeg = data.get_bone2_idx_call((char*)VMProtectDecryptStringA("RightUpLeg"));
		UINT32 LeftUpLeg = data.get_bone2_idx_call((char*)VMProtectDecryptStringA("LeftUpLeg"));

		if (Head && RightArm &&LeftArm&&RightUpLeg&&LeftUpLeg)
		{
			xsFloat2 Head_p2_0;
			xsFloat3 Head_p3_0 = data.get_bone2_pos_call(Head);

			xsFloat2 Head_p2_1;
			xsFloat3 Head_p3_1 = data.get_bone2_pos_call(Head - 1);

			xsFloat2 Head_p2_2;
			xsFloat3 Head_p3_2 = data.get_bone2_pos_call(Head - 2);

			xsFloat2 Head_p2_3;
			xsFloat3 Head_p3_3 = data.get_bone2_pos_call(Head - 3);

			xsFloat2 Head_p2_4;
			xsFloat3 Head_p3_4 = data.get_bone2_pos_call(Head - 4);

			xsFloat2 Head_p2_5;
			xsFloat3 Head_p3_5 = data.get_bone2_pos_call(Head - 5);


			xsFloat2 RightArm_p2_0;
			xsFloat3 RightArm_p3_0 = data.get_bone2_pos_call(RightArm);

			xsFloat2 RightArm_p2_1;
			xsFloat3 RightArm_p3_1 = data.get_bone2_pos_call(RightArm + 1);

			xsFloat2 RightArm_p2_2;
			xsFloat3 RightArm_p3_2 = data.get_bone2_pos_call(RightArm + 2);

			//xsFloat2 RightArm_p2_3;
			//xsFloat3 RightArm_p3_3 = data.get_bone2_pos_call(RightArm + 3);


			xsFloat2 LeftArm_p2_0;
			xsFloat3 LeftArm_p3_0 = data.get_bone2_pos_call(LeftArm);

			xsFloat2 LeftArm_p2_1;
			xsFloat3 LeftArm_p3_1 = data.get_bone2_pos_call(LeftArm + 1);

			xsFloat2 LeftArm_p2_2;
			xsFloat3 LeftArm_p3_2 = data.get_bone2_pos_call(LeftArm + 2);

			//xsFloat2 LeftArm_p2_3;
			//xsFloat3 LeftArm_p3_3 = data.get_bone2_pos_call(LeftArm + 3);


			xsFloat2 RightUpLeg_p2_0;
			xsFloat3 RightUpLeg_p3_0 = data.get_bone2_pos_call(RightUpLeg);

			xsFloat2 RightUpLeg_p2_1;
			xsFloat3 RightUpLeg_p3_1 = data.get_bone2_pos_call(RightUpLeg + 1);

			xsFloat2 RightUpLeg_p2_2;
			xsFloat3 RightUpLeg_p3_2 = data.get_bone2_pos_call(RightUpLeg + 2);

			//xsFloat2 RightUpLeg_p2_3;
			//xsFloat3 RightUpLeg_p3_3 = data.get_bone2_pos_call(RightUpLeg + 3);


			xsFloat2 LeftUpLeg_p2_0;
			xsFloat3 LeftUpLeg_p3_0 = data.get_bone2_pos_call(LeftUpLeg);

			xsFloat2 LeftUpLeg_p2_1;
			xsFloat3 LeftUpLeg_p3_1 = data.get_bone2_pos_call(LeftUpLeg + 1);

			xsFloat2 LeftUpLeg_p2_2;
			xsFloat3 LeftUpLeg_p3_2 = data.get_bone2_pos_call(LeftUpLeg + 2);

			//xsFloat2 LeftUpLeg_p2_3;
			//xsFloat3 LeftUpLeg_p3_3 = data.get_bone2_pos_call(LeftUpLeg + 3);

			if (WorldToScreen(&Head_p3_0, &Head_p2_0)&& WorldToScreen(&Head_p3_1, &Head_p2_1) && WorldToScreen(&Head_p3_2, &Head_p2_2) && WorldToScreen(&Head_p3_3, &Head_p2_3) && WorldToScreen(&Head_p3_4, &Head_p2_4) && WorldToScreen(&Head_p3_5, &Head_p2_5))
			{
				xs_Gui::GetForegroundDrawList()->AddLine(Head_p2_0, Head_p2_1, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(Head_p2_1, Head_p2_2, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(Head_p2_2, Head_p2_3, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(Head_p2_3, Head_p2_4, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(Head_p2_4, Head_p2_5, bone_rgba);
			}

			if (WorldToScreen(&RightArm_p3_0, &RightArm_p2_0) && WorldToScreen(&RightArm_p3_1, &RightArm_p2_1) && WorldToScreen(&RightArm_p3_2, &RightArm_p2_2) /*&& WorldToScreen(&RightArm_p3_3, &RightArm_p2_2)*/)
			{
				xs_Gui::GetForegroundDrawList()->AddLine(RightArm_p2_0, Head_p2_2, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(RightArm_p2_0, RightArm_p2_1, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(RightArm_p2_1, RightArm_p2_2, bone_rgba);
				//xs_Gui::GetForegroundDrawList()->AddLine(RightArm_p2_2, RightArm_p2_3, bone_rgba);
			}

			if (WorldToScreen(&LeftArm_p3_0, &LeftArm_p2_0) && WorldToScreen(&LeftArm_p3_1, &LeftArm_p2_1) && WorldToScreen(&LeftArm_p3_2, &LeftArm_p2_2) /*&& WorldToScreen(&LeftArm_p3_3, &LeftArm_p2_2)*/)
			{
				xs_Gui::GetForegroundDrawList()->AddLine(LeftArm_p2_0, Head_p2_2, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(LeftArm_p2_0, LeftArm_p2_1, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(LeftArm_p2_1, LeftArm_p2_2, bone_rgba);
				//xs_Gui::GetForegroundDrawList()->AddLine(LeftArm_p2_2, LeftArm_p2_3, bone_rgba);
			}

			if (WorldToScreen(&RightUpLeg_p3_0, &RightUpLeg_p2_0) && WorldToScreen(&RightUpLeg_p3_1, &RightUpLeg_p2_1) && WorldToScreen(&RightUpLeg_p3_2, &RightUpLeg_p2_2) /*&& WorldToScreen(&RightUpLeg_p3_3, &RightUpLeg_p2_2)*/)
			{
				xs_Gui::GetForegroundDrawList()->AddLine(RightUpLeg_p2_0, Head_p2_5, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(RightUpLeg_p2_0, RightUpLeg_p2_1, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(RightUpLeg_p2_1, RightUpLeg_p2_2, bone_rgba);
				//xs_Gui::GetForegroundDrawList()->AddLine(RightUpLeg_p2_2, RightUpLeg_p2_3, bone_rgba);
			}

			if (WorldToScreen(&LeftUpLeg_p3_0, &LeftUpLeg_p2_0) && WorldToScreen(&LeftUpLeg_p3_1, &LeftUpLeg_p2_1) && WorldToScreen(&LeftUpLeg_p3_2, &LeftUpLeg_p2_2)/* && WorldToScreen(&LeftUpLeg_p3_3, &LeftUpLeg_p2_2)*/)
			{
				xs_Gui::GetForegroundDrawList()->AddLine(LeftUpLeg_p2_0, Head_p2_5, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(LeftUpLeg_p2_0, LeftUpLeg_p2_1, bone_rgba);
				xs_Gui::GetForegroundDrawList()->AddLine(LeftUpLeg_p2_1, LeftUpLeg_p2_2, bone_rgba);
				//xs_Gui::GetForegroundDrawList()->AddLine(LeftUpLeg_p2_2, LeftUpLeg_p2_3, bone_rgba);
			}
		}
	//if (data.bone)
	//	for (size_t i = data.bone_id; i > data.bone_id-4; i--)
	//	{
	//		char a[20] = { 0 };
	//		sprintf(a, u8"%d", i);
	//		xsFloat2 p2;

	//		if (WorldToScreen((xsFloat3)data.get_bone_call(i), &p2))
	//		{
	//			xs_Gui::GetForegroundDrawList()->AddText(NULL, 13.0f, p2,0xff00ffff, a);
	//		}
	//	}

	//if (data.bone_count)
	//{
	//	//int bone_id = data.get_bone_name_this("Head");
	//	//if (bone_id >= 0)
	//	//{
	//	//	xsFloat3 pos = data.get_bone_this_position(bone_id);
	//	//	xsFloat2 xy;
	//	//	if (WorldToScreen(&pos, &xy))
	//	//		xs_Gui::GetForegroundDrawList()->AddText(NULL, 16.0f, xy - xsFloat2{ 0,11.0f }, 0xFFFFFFFF, "Head", 0, 1);
	//	//}
	//	for (size_t i = 0; i < data.bone_count; i++)
	//	{
	//		xsFloat3 pos = data.get_bone_this_position(i);
	//		xsFloat2 xy;
	//		if (WorldToScreen(&pos,&xy))
	//		{
	//			char a[50] = { 0 };
	//			UINT64 info = data.get_bone_this_info(i);
	//			if (info)
	//				sprintf(a, VMProtectDecryptStringA("%d %d"), i, data.get_bone_this_info_count(i));
	//			else
	//				sprintf(a, VMProtectDecryptStringA("N"));

	//			UINT32 col = 0xFFFF00FF;
	//			if (!data.get_bone_this_inval(i) && !data.get_bone_this_info_inval(info))
	//				col = 0xFFFFFFFF;
	//			xs_Gui::GetForegroundDrawList()->AddText(NULL, 16.0f, xy - xsFloat2{ 0,11.0f }, col, a, 0, 1);
	//		}
	//	}
	//}

	/*
	if (data.bone_count == 104)
	{
		xsFloat2 p1, p2, p3
			, p11, p12, p13
			, p21, p22, p23
			, p31, p32, p33
			, p41, p42, p43
			;
		//¼¹×µ
		UINT32 npc = 0;
		if (!data.npc)
			npc = 4;
		if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 7 + npc), &p1))
			if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 5 + npc), &p2))
				if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 0 + npc), &p3))
				{
					xs_Gui::GetForegroundDrawList()->AddLine(p1, p2, bone_rgba);
					xs_Gui::GetForegroundDrawList()->AddLine(p2, p3, bone_rgba);

					if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 13 + npc), &p11))
						if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 14 + npc), &p12))
							if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 15 + npc), &p13))
							{
								xs_Gui::GetForegroundDrawList()->AddLine(p2, p11, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p11, p12, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p12, p13, bone_rgba);
							}

					if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 43 + npc), &p21))
						if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 44 + npc), &p22))
							if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 45 + npc), &p23))
							{
								xs_Gui::GetForegroundDrawList()->AddLine(p2, p21, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p21, p22, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p22, p23, bone_rgba);
							}
					if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 76 + npc), &p31))
						if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 77 + npc), &p32))
							if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 78 + npc), &p33))
							{
								xs_Gui::GetForegroundDrawList()->AddLine(p3, p31, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p31, p32, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p32, p33, bone_rgba);
							}

					if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 84 + npc), &p41))
						if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 85 + npc), &p42))
							if (WorldToScreen((xsFloat3*)((xsFloat4*)data.bone + 4 * 86 + npc), &p43))
							{
								xs_Gui::GetForegroundDrawList()->AddLine(p3, p41, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p41, p42, bone_rgba);
								xs_Gui::GetForegroundDrawList()->AddLine(p42, p43, bone_rgba);
							}
				}
	}
	*/
}


inline void _esp::name_grade(_entity_data& data,char* a)
{
	if (!data.screen_s_bool || !data.screen_x_bool)
		return;
	if (!data.high)
		data.high = 5.0f;
	data.high += 18.0f;

	xs_Gui::GetForegroundDrawList()->AddText(NULL, 13.0f, data.screen_s - xsFloat2{ (data.screen_x.x - data.screen_s.x) * -0.5f,data.high } , name_rgba, 0, a, 1);
}


inline void _esp::blood(_entity_data& data)
{
	if (!data.screen_s_bool || !data.screen_x_bool)
		return;
	if (!data.high)
		data.high = 5.0f;
	data.high += 3.0f;

	xsFloat2 p1 = data.screen_s - xsFloat2{ (data.screen_x.x - data.screen_s.x) * -0.5f,data.high };
	UINT32 rgba = 0;

	if (data.name_list == 1)
		rgba = whitelist_blood_rgba;
	else if (data.name_list == 2)
		rgba = blacklist_blood_rgba;
	else if (data.enemy)
		rgba = blood_rgba_enemy;
	else
		rgba = blood_rgba;
	xs_Gui::GetForegroundDrawList()->AddLine_s(p1, 70.0f, rgba, 2.0f, *data.blood, *data.blood_top);
	//char a[256] = { 0 };

	//sprintf(a, u8"%x |%d", data.ID, data.get_bone_count());
	//xs_Gui::GetForegroundDrawList()->AddText(NULL, 13.0f, data.screen_x + xsFloat2{ 0, 1.0f }, 0xffffffff, a, 0, 1);
}


inline void _esp::armor(_entity_data& data)
{
	if (!data.screen_s_bool || !data.screen_x_bool)
		return;

	if (!data.high)
		data.high = 5.0f;
	data.high += 2.0f;

	xsFloat2 p1 = data.screen_s - xsFloat2{ (data.screen_x.x - data.screen_s.x) * -0.5f,data.high };
	xs_Gui::GetForegroundDrawList()->AddLine_s(p1, 70.0f, armor_rgba, 2.0f, *data.Armor, *data.Armor_top);
}

inline void _esp::alert(_entity_data& data)
{

}


inline void _esp::aim_bone_sign(_entity_data& data)
{
	if (!data.aim_pitch)
		return;

	//xsFloat2 pos_ ;
	//xsFloat3 pos3_;
	//pos3_ = *data.position;
	//
	//if (fabs(pos3_.y - data.weapon.aim.y) < 0.2f)
	//	pos3_.y += 10.f;
	//else
	//	pos3_.y = data.weapon.aim.y;

	//if (data.weapon.aim_no_bone)
	//	pos3_.y -= 5.f;

	//if (data.weapon.aim_bone2_bool)
	//	pos3_ = data.weapon.aim2;

	//WorldToScreen(&pos3_,&pos_);
	//xs_Gui::GetForegroundDrawList()->AddLine(data.aim_pitch_position, pos_, aim_sign_rgba, 1.0f);
	if (data.aim_pitch)
		xs_Gui::GetForegroundDrawList()->AddText(NULL, 16.0f, data.aim_pitch_position - xsFloat2{ 0,11.0f }, aim_sign_rgba, VMProtectDecryptStringA("\xc3\x97"), 0, 1);//¡Á

}



inline void _esp::size(_entity_data& data)
{
	/*
	if (data.bone_this && data.bone_count>0)
	{


		xsFloat3 xyz = data.get_bone_this_position(0);
		xsFloat2 p1, p2;

		xyz.y += 0.2f;
		if (!WorldToScreen(&xyz, &p1))
			return;
		xyz.y -= 0.4f;
		if (!WorldToScreen(&xyz, &p2))
			return;

		float temp_x = (p1.y - p2.y) * 0.5f;
		xsFloat2 p3, p4;
		p3.x = min(p1.x, p2.x) - temp_x;
		p3.y = min(p1.y, p2.y);
		p4.x = max(p1.x, p2.x) + temp_x;
		p4.y = max(p1.y, p2.y);
		data.write_screen_s(p3);
		data.write_screen_x(p4);
	}
	*/
	if (data.bone2_this && data.bone2_count > 10)
	{
		xsFloat3 xyz_p1 = data.get_bone2_pos_call(0);
		xsFloat3 xyz_p2 = xyz_p1;
		for (size_t i = 1; i < data.get_bone2_count_call(); i++)
		{
			xsFloat3 xyz = data.get_bone2_pos_call(i);
			xyz_p1.x = max(xyz_p1.x, xyz.x);
			xyz_p1.z = max(xyz_p1.z, xyz.z);

			xyz_p1.y = max(xyz_p1.y, xyz.y);


			xyz_p2.x = min(xyz_p2.x, xyz.x);
			xyz_p2.z = min(xyz_p2.z, xyz.z);

			xyz_p2.y = min(xyz_p2.y, xyz.y);
		}


		xsFloat2 p1, p2, p3, p4;

		xsFloat2 pos;
		if (!WorldToScreen(&xyz_p1, &p1))
			return;
		if (!WorldToScreen(&xyz_p2, &p2))
			return;

		float xy_yx = xyz_p1.x;
		xyz_p1.x = xyz_p2.x;
		xyz_p2.x = xy_yx;

		if (!WorldToScreen(&xyz_p1, &p3))
			return;
		if (!WorldToScreen(&xyz_p2, &p4))
			return;


		pos.x = min(min(min(p1.x, p2.x), p3.x), p4.x);
		pos.y = min(min(min(p1.y, p2.y), p3.y), p4.y);
		data.write_screen_s(pos);

		pos.x = max(max(max(p1.x, p2.x), p3.x), p4.x);
		pos.y = max(max(max(p1.y, p2.y), p3.y), p4.y);
		data.write_screen_x(pos);

	}
	else if (data.bone_this && data.bone_count > 0)
	{
		xsFloat3 xyz_p1 = data.get_bone_this_position(0);
		xsFloat3 xyz_p2 = xyz_p1;
		for (size_t i = 1; i < data.bone_count; i++)
		{
			xsFloat3 xyz = data.get_bone_this_position(i);
			xyz_p1.x = max(xyz_p1.x, xyz.x);
			xyz_p1.z = max(xyz_p1.z, xyz.z);

			xyz_p1.y = max(xyz_p1.y, xyz.y);


			xyz_p2.x = min(xyz_p2.x, xyz.x);
			xyz_p2.z = min(xyz_p2.z, xyz.z);

			xyz_p2.y = min(xyz_p2.y, xyz.y);
		}


		xsFloat2 p1, p2, p3, p4;

		xsFloat2 pos;
		if (!WorldToScreen(&xyz_p1, &p1))
			return;
		if (!WorldToScreen(&xyz_p2, &p2))
			return;

		float xy_yx = xyz_p1.x;
		xyz_p1.x = xyz_p2.x;
		xyz_p2.x = xy_yx;

		if (!WorldToScreen(&xyz_p1, &p3))
			return;
		if (!WorldToScreen(&xyz_p2, &p4))
			return;


		pos.x = min(min(min(p1.x, p2.x), p3.x), p4.x);
		pos.y = min(min(min(p1.y, p2.y), p3.y), p4.y);
		data.write_screen_s(pos);

		pos.x = max(max(max(p1.x, p2.x), p3.x), p4.x);
		pos.y = max(max(max(p1.y, p2.y), p3.y), p4.y);
		data.write_screen_x(pos);

	}
	else if(data.enemy)
	{
		xsFloat3* aa = data.call_wrj_aim();
		if (is_addr((UINT64)aa))
		{
			xsFloat3 xyz_p1 = *aa;
			xsFloat3 xyz_p2 = xyz_p1;
			xyz_p1.y += 0.2f;
			xyz_p2.y -= 0.2f;

			xsFloat2 p1, p2;
			if (WorldToScreen(&xyz_p1, &p1))
				data.write_screen_s(p1);

			if (WorldToScreen(&xyz_p2, &p2))
				data.write_screen_x(p2);
		}
	}
}


inline void _esp::size(_materia_data& data)
{
	if (!data.position1)
		return;

	xsFloat2 p1;

	xsFloat3 xyz = *data.position1;
	xyz.y -= 0.2f;
	if (WorldToScreen(&xyz, &p1))
		data.write_screen(p1);

}



inline void _esp::supplies_sign(_materia_data& data)
{
	if(!data.screen_bool)
		return;
	//char a[256] = { 0 };
	//sprintf(a, "\xe9\xbe\xa4");
	//sprintf(a, u8"%x x:%f y:%f z:%f", data.ID, data.position1->x, data.position1->y, data.position1->z);
	if (*data.Show_Fkey)
		xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0f, data.screen, 0xffFFFFFF, VMProtectDecryptStringA("\xe9\xbe\xa4"), 0, 1);
	else
		xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0f, data.screen, 0xFF0078FF, VMProtectDecryptStringA("\xe9\xbe\xa4"), 0, 1);
}


inline void _esp::drop_sign(_materia_data& data)
{
	if (!data.screen_bool)
		return;
	//char a[256] = { 0 };
	//sprintf(a, "\xe9\xbe\xa9");
	//sprintf(a, "%x", data.ID);
	if (*data.Show_Fkey)
		xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0f, data.screen, 0xffFFFFFF, VMProtectDecryptStringA("\xe9\xbe\xa9"), 0, 1);
	else
		xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0f, data.screen, 0xFFB7D223, VMProtectDecryptStringA("\xe9\xbe\xa9"), 0, 1);

}


inline void _esp::aim_length(_aim& data)
{
	xsFloat2 pos = *window;
	pos.x /= 2;
	pos.y /= 2;
	UINT32 col = 0x80ffffff;
	UINT32 col1 = 0x80ffffff;
	if (data.aim_mouse_length_enable)
		col = 0x400000ff;
	if (data.aim_length_enable2|| data.aim_san_size2)
		col1 = 0x400000ff;
	if (aim_mouse_length_enable)
		xs_Gui::GetForegroundDrawList()->AddCircle(pos, data.aim_mouse_length.value, col, 20, 2.0f);
	if (aim_length_enable)
		xs_Gui::GetForegroundDrawList()->AddCircle(pos, data.aim_length.value, col1, 10, 1.0f);

	if (aiming_enable)
		xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0F, pos - xsFloat2{ 0,8.0f }, aiming_rgba, VMProtectDecryptStringA("\xe9\xbe\xa8"), 0, 1);

}


void _esp::esp_player(xs_DataFrame<_entity_data>* c,UINT32 type)
{
	if (type)//player_data
	{
		//return;
		for (size_t i = 1; i < c->Size; i++)
		{
			if (Player_enemy_enable ? !c->Data[i].enemy : 0)
				continue;

			//if (!Player_enable_size)
			//	continue;
			if (!*c->Data[i].blood /*|| !c->Data[i].bone || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;

			size(c->Data[i]);
			if (!c->Data[i].screen_s_bool || !c->Data[i].screen_x_bool)
				continue;
			if (Player_box_enable)
				box(c->Data[i]);
			if (Player_bone_enable)
				bone(c->Data[i]);
			if (Player_armor_enable)
				armor(c->Data[i]);
			if (Player_blood_enable)
				blood(c->Data[i]);
			if (Player_aim_sign_enable)
				aim_bone_sign(c->Data[i]);
			if (Player_name_enable)
			{
				name_grade(c->Data[i], c->Data[i].name);
			}
			/*if (Player_name_enable && !Player_grade_enable && !Player_distance_enable)
			{
				name_grade(c->Data[i], c->Data[i].name);
			}
			else if (Player_name_enable || Player_grade_enable|| Player_distance_enable)
			{
				char a[256] = { 0 };
				if (Player_distance_enable)
				{
					char b[256] = { 0 };
					sprintf(b, VMProtectDecryptStringA("[%.0f]"), c->Data[i].distance);
					sprintf(a, VMProtectDecryptStringA("%s"), b);
				}
				if (Player_grade_enable)
				{
					char b[256] = { 0 };
					sprintf(b, VMProtectDecryptStringA("%s[%d]"), a, *c->Data[i].grade);
					sprintf(a, VMProtectDecryptStringA("%s"), b);
				}
				if (Player_name_enable)
				{
					char b[256] = { 0 };
					sprintf(b, VMProtectDecryptStringA("%s%s"), a, c->Data[i].name);
					sprintf(a, VMProtectDecryptStringA("%s"), b);
				}

				name_grade(c->Data[i], a);
			}*/
		}
		

	}
	else
	{
		for (size_t i = 0; i < c->Size; i++)
		{
			if (npc_enemy_enable ? !c->Data[i].enemy : 0 )
				continue;


			//if (!npc_enable_size)
			//	continue;
			if (!*c->Data[i].blood/* || !c->Data[i].bone || (*c->Data[i].bone_count == 104 ? 0 : *c->Data[i].bone_count != 42)*/)
				continue;
			size(c->Data[i]);
			if (!c->Data[i].screen_s_bool || !c->Data[i].screen_x_bool)
				continue;
			if (npc_box_enable)
				box(c->Data[i]);
			if (npc_bone_enable)
				bone(c->Data[i]);
			if (npc_armor_enable)
				armor(c->Data[i]);
			if (npc_blood_enable)
				blood(c->Data[i]);
			if (npc_aim_sign_enable)
				aim_bone_sign(c->Data[i]);

			if (npc_name_enable)
			{
				name_grade(c->Data[i], c->Data[i].name);
			}
			/*if (npc_name_enable && !npc_grade_enable && !npc_distance_enable)
			{
				name_grade(c->Data[i], c->Data[i].name);
			}
			else if (npc_name_enable || npc_grade_enable|| npc_distance_enable)
			{
				char a[256] = { 0 };
				if(npc_distance_enable)
				{
					char b[256]= { 0 };
					sprintf(b, VMProtectDecryptStringA("[%.0f]"), c->Data[i].distance);
					sprintf(a, VMProtectDecryptStringA("%s"), b);
				}
				if (npc_grade_enable)
				{
					char b[256] = { 0 };
					sprintf(b, VMProtectDecryptStringA("%s[%d]"), a,*c->Data[i].grade);
					//sprintf(b, u8"%s[%x]", a, c->Data[i].ID);
					sprintf(a, VMProtectDecryptStringA("%s"), b);
				}
				if (npc_name_enable)
				{
					char b[256] = { 0 };
					sprintf(b, VMProtectDecryptStringA("%s%s"), a, c->Data[i].name);
					sprintf(a, VMProtectDecryptStringA("%s"), b);
				}
				
				name_grade(c->Data[i], a);
			}*/
		}
		
	}

}


void _esp::esp_supplies(xs_DataFrame<_materia_data>* c)
{
	if (supplies_sign_enable)
		for (size_t i = 0; i < c->Size; i++)
		{
			size(c->Data[i]);
			if (!*c->Data[i].plunder)
				supplies_sign(c->Data[i]);
		}
}


void _esp::esp_Drop(xs_DataFrame<_materia_data>* c)
{
	if (Drop_sign_enable)
		for (size_t i = 0; i < c->Size; i++)
		{
			size(c->Data[i]);
				drop_sign(c->Data[i]);
		}
}


void _esp::esp_aim(_aim& data)
{
	aim_length(data);
}


void _esp::esp_config()
{

	{
		xs_config iniop;

		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nblacklist_box_rgba="), &blacklist_box_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nblacklist_blood_rgba="), &blacklist_blood_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nwhitelist_box_rgba="), &whitelist_box_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nwhitelist_blood_rgba="), &whitelist_blood_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nbbox_rgba="), &box_rgba);

		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nbone_rgba="), &bone_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nname_rgba="), &name_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nbblood_rgba="), &blood_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nblood_rgba_enemy="), &blood_rgba_enemy);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\narmor_rgba="), &armor_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\naim_sign_rgba="), &aim_sign_rgba);
		iniop.Read_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\naiming_rgba="), &aiming_rgba);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\naiming_enable="), &aiming_enable);


		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_enemy_enable="), &Player_enemy_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_box_enable="), &Player_box_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_bone_enable="), &Player_bone_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_name_enable="), &Player_name_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_blood_enable="), &Player_blood_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_armor_enable="), &Player_armor_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_grade_enable="), &Player_grade_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_distance_enable="), &Player_distance_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_alert_enable="), &Player_alert_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_weapon_enable="), &Player_weapon_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nPlayer_aim_sign_enable="), &Player_aim_sign_enable);


		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_enemy_enable="), &npc_enemy_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_box_enable="), &npc_box_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_bone_enable="), &npc_bone_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_name_enable="), &npc_name_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_blood_enable="), &npc_blood_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_armor_enable="), &npc_armor_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_grade_enable="), &npc_grade_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_distance_enable="), &npc_distance_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_alert_enable="), &npc_alert_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_weapon_enable="), &npc_weapon_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nnpc_aim_sign_enable="), &npc_aim_sign_enable);

		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nsupplies_sign_enable="), &supplies_sign_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nDrop_sign_enable="), &Drop_sign_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\nDrop_name_enable="), &Drop_name_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\naim_length_enable="), &aim_length_enable);
		iniop.Read_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("\naim_mouse_length_enable="), &aim_mouse_length_enable);

		iniop.SaveConfig();
		iniop.OffConfig();
	}

}

void _esp::esp_config_Write()
{

	{
		xs_config iniop;

		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("blacklist_box_rgba="), blacklist_box_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("blacklist_blood_rgba="), blacklist_blood_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("whitelist_box_rgba="), whitelist_box_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("whitelist_blood_rgba="), whitelist_blood_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("bbox_rgba="), box_rgba);

		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("bone_rgba="), bone_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("name_rgba="), name_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("bblood_rgba="), blood_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("blood_rgba_enemy="), blood_rgba_enemy);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("armor_rgba="), armor_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("aim_sign_rgba="), aim_sign_rgba);
		iniop.Write_ulong_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("aiming_rgba="), aiming_rgba);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("aiming_enable="), aiming_enable);


		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_enemy_enable="), Player_enemy_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_box_enable="), Player_box_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_bone_enable="), Player_bone_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_name_enable="), Player_name_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_blood_enable="), Player_blood_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_armor_enable="), Player_armor_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_grade_enable="), Player_grade_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_distance_enable="), Player_distance_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_alert_enable="), Player_alert_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_weapon_enable="), Player_weapon_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Player_aim_sign_enable="), Player_aim_sign_enable);


		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_enemy_enable="), npc_enemy_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_box_enable="), npc_box_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_bone_enable="), npc_bone_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_name_enable="), npc_name_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_blood_enable="), npc_blood_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_armor_enable="), npc_armor_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_grade_enable="), npc_grade_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_distance_enable="), npc_distance_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_alert_enable="), npc_alert_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_weapon_enable="), npc_weapon_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("npc_aim_sign_enable="), npc_aim_sign_enable);

		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("supplies_sign_enable="), supplies_sign_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Drop_sign_enable="), Drop_sign_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("Drop_name_enable="), Drop_name_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("aim_length_enable="), aim_length_enable);
		iniop.Write_bool_Config(VMProtectDecryptStringA("[esp]"), VMProtectDecryptStringA("aim_mouse_length_enable="), aim_mouse_length_enable);

		iniop.SaveConfig();
		iniop.OffConfig();
	}

}
