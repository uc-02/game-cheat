#pragma once
#include "xsgui.h"
#include"hook_call.h"
#include <stdio.h>
#include "config.h"

enum _BaseAddress
{
    _entity_1offset = 0x128,
    _entity_2offset = 0x220,//0x218
    _entityCount_2offset = 0x228,//0x220
    _entity_bone_info_1offset =0xC0,
    _entity_bone_info_2offset = 0x120,
    _entity_bone_info_3offset = 0x38,
    _entity_bone_info_count = 0x28,
    _entity_bone_info = 0x20,
    _entity_death_offset= 0x130,
    _materia_1offset = 0x128,
    _materia_2offset = 0x288,
    _materia_3offset = 0xC4,
    _materia_4offset = 0x20,
    _materiaCount_4offset = 0x28,
    _drop_materia_1offset = 0x98,
    _weapon_2offset = 0x858
};

enum _entity_data_offset
{
	_time_now = 0xBE8,//
	_time = 0x5C8 + 8,//
    _position = 0x70,
    _name = 0x440 + 8,
    _name_bool = 0x44F + 8,
    _npc_sign = 0x45c + 8,
    _UID = 0x5A0,//0x598
    _Is_enemy_call=0x2C8,//
    _experience = 0x450 + 8,
    _grade = 0x4C8 + 0xB * 4 + 8,
    _entity_mation_2offset= 0x7D8 + 8,//0x7D8
    _armor=0x254,
    _armor_top = 0x258,
    _npc_armor = 0x1C4,
    _npc_armor_top = 0x1C8,
    _blood = 0x5C,
    _blood_top = 0x60

};

enum _entity_bone_data_offset
{
    _bone_offset2 = 0x230 + 8,
    _bone_offset3 = 0x170,
    _bone_count = 0x178,
    _bone_pos_call = 0x98,
    _bone_id_call = 0x90,
	_bone_name_call = 0x88,
    _bone_count_call = 0x80,
    _bone_Init_id = 0xB6C,


};

enum _materia_data_offset
{
    _type = 0x10,
    _isPlunder = 0x268,
    _isF_Plunder = 0x260,
    _Show_Fkey = 0xD,
    _Plunder0 = 0x188,
    _Plunder1 = 0x38,
    _Plunder2 = 0x3E0,
    _Plunder3 = 0x340,
    _astigmatism = 0x6E0,
    _drop_offset1=0x48,
    _drop=0x38,
	_drop_count = 0x20,
	_drop_name_0 = 0x18,
	_drop_name_1 = 0x10,
	_drop_name_size = 0xE8,

    _drop_sign = 0x40,
    _materia_sign = 0x248,
    _materia_call_1 = 0xB8,
    _materia_call_4 = 0xD0,
    _materia_call_F = 0x60
};

enum _weapon_data_offset
{
    _mirror = 0x37C6,
    _fire = 0x37C4,
    _Shoot_bullet_count = 0x376C,
    _fire_speed = 0x44B4,
    _fire_speed_call = 0x198,

    _Cartridge_list=0x4410,
    _back_Cartridge_list2_1 = 0x20,
    _Cartridge_list2_1 = 0x30,
    _Cartridge_list2 = 0xA8,
    _Cartridge_list3_1 = 0x118,
    _Cartridge_list3_2 = 0x410,
    _Cartridge_list3 = 0x70,

    _Cartridge_list4_1 = 0xD8,
    _Cartridge_list4_2 = 0xC0,
    _Cartridge_list4_3 = 0x978,
    _Cartridge_list4_4 = 0x0,
    _Cartridge_list4_5 = 0x250,
    _Cartridge_list4 = 0x10,

	_bullet_clips_list = 0x3728,
    _gun_confg_list= 0x3748,
    _fully_auto_offset1 = 0x160,
    _fully_auto_offset2 = 0x0,
    _fully_auto_offset3 = 0x0,
    _fully_auto_addr = 0x444D098, //完成  需要定位 
    _semi_auto_addr = 0x4895E08, //完成  需要定位 

    _No_scattering0 = 0x397C,
    _No_scattering1 = 0x396C,
    _No_scattering2 = 0x3974,
    _No_scattering3 = 0x3984,
    _false_bullet=0x390A,
    _point_entity_this = 0x3790,



    _Sniper_fire_button = 0x2D0,
    _Sniper_fire_offset1 = 0x8,
    _Sniper_fire_offset2 = 0x220,
    _Sniper_fire_offset3 = 0x160,

    _Rifle_firing_button = 0x6810C78, //无用
    _Rifle_firing_offset4 = 0x1F0,

    _fire1=0x3748,
    _fire2 = 0x160,
    _firecall = 0x40, //88 97 50 3F 00 00 8B 97 5C 38 00 00 48 8B 8B 60 01 00 00 44 8B B3 FC 02 00 00 48 8B 0C D1 48 8B 11 FF 52 40


    _Decryption_rcx_0 = 0x8,
    _Decryption_rcx_1=0x6C8,
    _Decryption_rcx_2 = 0x38,

    _Decryption_rdx_0 = 0x98,
    _Decryption_rdx_2 = 0x50,
    _Decryption_rdx_BaseAddress=0x67CDB40,


    _Decryption_call=0x179FE0,
    _Decryption_call2= 0x18C380
};

struct _entity_data;
struct _materia_data;
struct _weapon_data;
struct _aim;
struct _name_list_data;

typedef struct __MMVA_offset {
	/* 0x0000 */ unsigned __int64 Offset : 12;
	/* 0x0000 */ unsigned __int64 PTE : 9;
	/* 0x0000 */ unsigned __int64 PDT : 9;
	/* 0x0000 */ unsigned __int64 PDPTE : 9;
	/* 0x0000 */ unsigned __int64 PML4E : 9;
	/* 0x0000 */ unsigned __int64 PML5E : 9;
	/* 0x0000 */ unsigned __int64 Partition : 7; //User:0x0000 System:0x7F

}__MMVA_offset;

typedef struct __MMVA {
	union {
		ULONGLONG Long;

		struct __MMVA_offset Offset;

	}u;
} __MMVA; /* size: 0x0008 */

bool is_addr(UINT64 aa);
void set_vva(UINT64 aa);
struct _Message//消息类
{
    bool message_enable;


    _Message() { message_enable = 0; }
};

struct _supplies//materia_data类 //世界materia_data
{
    //UINT32 supplies_enable_size;//开关总数
    bool supplies_tp_enable;
    bool drop_tp_enable;
    xs_uint drop_tp_conut;

    bool supplies_tp_enable_key;
    bool drop_tp_enable_key;

    
    inline void supplies_hook_Init(_materia_data& data);
    inline void set_supplies_pos(_materia_data& data);

    inline void set_drop_pos(_materia_data& data, xsFloat3* pos_0);

    void supplies_cheat(xs_DataFrame<_materia_data>* c, xsFloat3* pos_0);
    void drop_cheat(xs_DataFrame<_materia_data>* c, xsFloat3* pos_0);

	void supplies_config();
	void supplies_config_Write();
    _supplies() {
        //supplies_enable_size = 0;
        supplies_tp_enable = 0;
        drop_tp_enable = 0;
        supplies_tp_enable_key = 0;
        drop_tp_enable_key = 0;

        drop_tp_conut.max = 10;
        drop_tp_conut.min = 1;
        drop_tp_conut.value = 1;
        drop_tp_conut.speed = 1;


		supplies_config();
    }
};

struct _item//物品类 //个人物品
{


    _item() { }
};

struct _Teleport//传送类
{


    _Teleport() { }
};


struct _Model//模型类
{


    _Model() { }
};

struct _npc//人机类
{


    _npc() { }
};

struct _Player//player_data类
{

    xs_float  g_speed;//移动速度
    xs_float  speed;//移动速度
    xsFloat4* player_speed_value;


    bool shift_g_speed;//瞄准或开火状态的移动速度
	bool f_g_speed;//瞄准或开火状态的移动速度
	bool x_g_speed;//瞄准或开火状态的移动速度
	bool global_speed_bool;//总开关
    float* global_speed_value;

    void hook_speed_init();

    inline void player_speed();
    void player_cheat();

	void Player_config();
	void Player_config_Write();
    _Player() {
        speed.value = 1.0f;
        speed.min = 1.0f;
        speed.max = 1.9f;
        speed.speed = 0.1f;

        g_speed.value = 0.0f;
        g_speed.min = 0.00000f;
        g_speed.max = 50.00f;
        g_speed.speed = 0.10f;

        shift_g_speed = 0;
		f_g_speed = 0;
		x_g_speed = 0;
		global_speed_bool = 0;

        player_speed_value = (xsFloat4*)malloc(sizeof(float[4]));
        *player_speed_value = xsFloat4{ 1.0f, 1.0f,1.0f,1.0f };


        global_speed_value = NULL;


		Player_config();
    }
    ~_Player() {
        free(player_speed_value);
    }
};


struct _Weapon//武器类
{
    //UINT32 weapon_enable_size;//开关总数
    xs_uint bullet_value;//输出子弹数量
    bool bullet_enable;//输出子弹数量 开关

    xs_uint speed_value;//射速
    bool    speed_enable;//射速


    bool weapon_read_confg_enable;//读配置
    bool weapon_write_confg_enable;//写配置

    bool deceit_bullets_enable;//卡无限备弹
    bool bullets_lock_enable;//锁子弹
    bool bullets_unlock_enable;//解锁子弹

    bool weapon_auto_enable;//全自动 武器

    bool weapon_recoil_enable;//无后坐力

    bool weapon_scatter_enable;//无散射

    bool weapon_reload_bullets_enable;//无换弹动作

    float* weapon_recoil;//


    inline void weapon_no_recoil(_weapon_data& data);
    inline void weapon_auto(_weapon_data& data);
    inline void weapon_lock_bullet(_weapon_data& data);
    inline void weapon_config(_weapon_data& data);
    inline void weapon_scatter(_weapon_data& data);

    void weapon_Init(UINT64* weapon_speed_call, UINT64* weapon_bullet_call, UINT64* weapon_deceit_bullets_call, UINT64* weapon_reload_call);
    void weapon_cheat(_weapon_data& data);
	void Weapon_config();
	void Weapon_config_Write();

    _Weapon() {
        //weapon_enable_size = 0;

        bullet_value.value = 1;
        bullet_value.max = 99;
        bullet_value.min = 1;
        bullet_value.speed = 1;
        bullet_enable = 0;

        deceit_bullets_enable = 0;
        bullets_lock_enable = 0;
        bullets_unlock_enable = 0;

        speed_value.value = 100;
        speed_value.max = 2000;
        speed_value.min = 100;
        speed_value.speed = 100;
        speed_enable = 0;


        weapon_auto_enable = 0;
        weapon_recoil_enable = 0;

        weapon_scatter_enable = 0;



        weapon_read_confg_enable = 0;
        weapon_write_confg_enable = 0;
        weapon_recoil = NULL;
		weapon_reload_bullets_enable = 0;

		Weapon_config();
    }
};

struct _esp//绘制类
{
    xsFloat2* window;
    float* matrix;

    UINT32 blacklist_box_rgba;//方框 黑name_list
    UINT32 blacklist_blood_rgba;//血量 黑name_list
    UINT32 whitelist_box_rgba;//方框 白name_list
    UINT32 whitelist_blood_rgba;//血量 白name_list

    UINT32 box_rgba;//方框
    UINT32 bone_rgba;//bone
    UINT32 name_rgba;//name
    UINT32 blood_rgba;//血量
    UINT32 blood_rgba_enemy;//血量
    UINT32 armor_rgba;//护甲
    UINT32 aim_sign_rgba;//自瞄sign

    UINT32 aiming_rgba;//准心
    bool   aiming_enable;

    //UINT32 Player_enable_size;//开关总数

    bool Player_enemy_enable;//enemy

    bool Player_box_enable;//player_data方框
    bool Player_bone_enable;//bone
    bool Player_name_enable;//name
    bool Player_blood_enable;//血量
    bool Player_armor_enable;//护甲
    bool Player_grade_enable;//grade
    bool Player_distance_enable;//distance
    bool Player_alert_enable;//警惕 或者 背后被偷袭提示
    bool Player_weapon_enable;//武器
    bool Player_aim_sign_enable;//自瞄sign
    
    //UINT32 npc_enable_size;//开关总数

    bool npc_enemy_enable;//enemy

    bool npc_box_enable;//方框
    bool npc_bone_enable;//bone
    bool npc_name_enable;//name
    bool npc_blood_enable;//血量
    bool npc_armor_enable;//护甲
    bool npc_grade_enable;//grade
    bool npc_distance_enable;//distance
    bool npc_alert_enable;//警惕 或者 背后被偷袭提示
    bool npc_weapon_enable;//武器
    bool npc_aim_sign_enable;//自瞄sign

    bool supplies_sign_enable;//materia_datasign

    //UINT32 Drop_enable_size;//开关总数
    bool Drop_sign_enable;//掉落物(drop_data)sign
    bool Drop_name_enable;//掉落物(drop_data)名

    bool aim_length_enable;
    bool aim_mouse_length_enable;


    inline bool WorldToScreen(xsFloat3* entity_position, xsFloat2* screen_position);

    inline void size(_entity_data& data);
    inline void box(_entity_data& data);
    inline void bone(_entity_data& data);
    inline void name_grade(_entity_data& data,char* a);
    inline void blood(_entity_data& data);
    inline void armor(_entity_data& data);
    inline void alert(_entity_data& data);
    inline void aim_bone_sign(_entity_data& data);

    inline void size(_materia_data& data);
    inline void supplies_sign(_materia_data& data);
    inline void drop_sign(_materia_data& data);

    inline void aim_length(_aim& data);

    
    void esp_config();
	void esp_config_Write();

    void esp_aim(_aim& data);
    void esp_player(xs_DataFrame<_entity_data>* c,UINT32 type);
    void esp_supplies(xs_DataFrame<_materia_data>* c);
    void esp_Drop(xs_DataFrame<_materia_data>* c);
   

    _esp() {
        window = &xs_Gui::GetIO().DisplaySize;

        //Player_enable_size = 0;
        Player_enemy_enable = 0;

        Player_box_enable = 0;
        Player_bone_enable = 0;
        Player_name_enable = 0;
        Player_blood_enable = 0;
        Player_armor_enable = 0;
        Player_grade_enable = 0;
        Player_distance_enable = 0;
        Player_alert_enable = 0;
        Player_weapon_enable = 0;
        Player_aim_sign_enable = 1;

        //npc_enable_size = 0;
        npc_enemy_enable = 0;

        npc_box_enable = 0;
        npc_bone_enable = 0;
        npc_name_enable = 0;
        npc_blood_enable = 0;
        npc_armor_enable = 0;
        npc_grade_enable = 0;
        npc_distance_enable = 0;
        npc_alert_enable = 0;
        npc_weapon_enable = 0;
        npc_aim_sign_enable = 1;

        supplies_sign_enable = 0;
        //Drop_enable_size = 0;
        Drop_sign_enable = 0;
        Drop_name_enable = 0;

        aim_mouse_length_enable = 0;
        aim_length_enable = 1;

        aiming_enable = 0;





        //_ui64toa(数值, temp字符串, 16)
         blacklist_box_rgba=0xFF000000;//方框 黑name_list
         blacklist_blood_rgba = 0xFF000000;//血量 黑name_list
         whitelist_box_rgba = 0xFFffffff;//方框 白name_list
         whitelist_blood_rgba = 0xFFffffff;//血量 白name_list


         box_rgba= 0xFF0050C9;//方框
         bone_rgba= 0xFFffffff;//bone
         name_rgba= 0xffffffff;//name
         blood_rgba= 0xff00ff00;//血量
         blood_rgba_enemy = 0xff0000ff;//enemy血量
         armor_rgba= 0xFFFFFF26;//护甲
         aim_sign_rgba= 0xffffffff;//自瞄sign

         aiming_rgba= 0xff00ff00;//准心

		 esp_config();
    }
};

struct _aim
{
    xsFloat2* window;
    float* matrix;

    //UINT32 aim_enable_size;
    xs_uint aim_bone[12];


    xs_float aim_length;//静默范围
    float aim_length_new;//计算 静默范围
    bool aim_length_enable;//结果 视角瞄准 开关
	bool aim_length_enable2;//结果 视角瞄准 开关

    xs_float aim_mouse_length;//鼠标范围
    float aim_mouse_length_new;//计算 鼠标范围 
    bool aim_mouse_length_enable;//结果 鼠标瞄准 开关
    bool aim_gun_enable;//结果 魔术子弹
	bool aim_san_enable;//结果 散射追踪


    xsFloat2 aim_bone_pos2;
    xsFloat3 aim_bone_pos3[80];
    xsFloat3 aim_bone_pos4[80];
	UINT32 aim_san_size;//结果 散射追踪 开关
	UINT32 aim_san_size2;//结果 散射追踪 开关

	bool aim_no_bone;

    bool Player_enable;
    bool npc_enable;


    bool aim_mouse_enable;
    bool aim_enable;
	bool aim_weak_enable;//弱点

    char fire_py;
    bool aim_auto_fire_enable;

    xs_uint aim_stop_fire;//自动熄火
    bool aim_stop_fire_enable;
    xs_uint aim_switch_target;//自动换目标
    bool aim_switch_target_enable;
    xs_uint aim_aotu_list;//自动白name_list
    bool aim_aotu_list_enable;

    xs_float aim_distance;//自动白name_list

	bool aim_list_key_enable;//名单按键

    inline bool WorldToScreen(xsFloat3* entity_position, xsFloat2* screen_position);
    inline void size(_entity_data& data);

    inline float aim_count_length(_weapon_data& data);

    inline void aim_mouse_mov();

    inline void aim_auto_fire(_weapon_data& data);
    inline UINT32 aim_switch_target_call(_weapon_data& data, UINT32 ii);
    inline void   aim_aotu_list_(_entity_data& data, _weapon_data& data2);
    inline void   aim_init_aim();
    inline float aim_init_length();
    void aim_hook_Init(UINT64 magic_call, UINT64 fire_call);
     void mouse_list(xs_DataFrame<_entity_data>* c, _entity_data& a, UINT32 type,xs_DataFrame<_name_list_data>* b);//准心 指向name_list
	 void no_mouse_list(xs_DataFrame<_entity_data>* c);//准心 指向name_list

     void aim_player(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc);
     void aim_cheat(_entity_data& data);

	 inline void aim__(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc);
	 inline void aim_gun_(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc);
	 inline void aim_san_(xs_DataFrame<_entity_data>* c, UINT32 type, _weapon_data& cc);

	 void aim_config();
	 void aim_config_Write();
     _aim()
     {
         window = &xs_Gui::GetIO().DisplaySize;
         //aim_enable_size = 0;
		 aim_weak_enable = 0;

		 aim_san_size = 0;
		 aim_san_size2 = 0;
		 aim_san_enable = 0;

         aim_bone[0].value = 11;
         aim_bone[0].speed = 1;
         aim_bone[0].min = 0;
         aim_bone[0].max = 11;

         aim_bone[1].value = 10;
         aim_bone[1].speed = 1;
         aim_bone[1].min = 0;
         aim_bone[1].max = 11;

         aim_bone[2].value = 9;
         aim_bone[2].speed = 1;
         aim_bone[2].min = 0;
         aim_bone[2].max = 11;

         aim_bone[3].value = 8;
         aim_bone[3].speed = 1;
         aim_bone[3].min = 0;
         aim_bone[3].max = 11;

         aim_bone[4].value = 7;
         aim_bone[4].speed = 1;
         aim_bone[4].min = 0;
         aim_bone[4].max = 11;

         aim_bone[5].value = 6;
         aim_bone[5].speed = 1;
         aim_bone[5].min = 0;
         aim_bone[5].max = 11;

		 aim_bone[6].value = 5;
		 aim_bone[6].speed = 1;
		 aim_bone[6].min = 0;
		 aim_bone[6].max = 11;

		 aim_bone[7].value = 4;
		 aim_bone[7].speed = 1;
		 aim_bone[7].min = 0;
		 aim_bone[7].max = 11;

		 aim_bone[8].value = 0;
		 aim_bone[8].speed = 1;
		 aim_bone[8].min = 0;
		 aim_bone[8].max = 11;

		 aim_bone[9].value = 1;
		 aim_bone[9].speed = 1;
		 aim_bone[9].min = 0;
		 aim_bone[9].max = 11;

		 aim_bone[10].value = 2;
		 aim_bone[10].speed = 1;
		 aim_bone[10].min = 0;
		 aim_bone[10].max = 11;

		 aim_bone[11].value = 3;
		 aim_bone[11].speed = 1;
		 aim_bone[11].min = 0;
		 aim_bone[11].max = 11;


         aim_length.value = 200.0f;
         aim_length.speed = 1.0f;
         aim_length.min = 4.0f;
         aim_length.max = 600.0f;
         aim_length_new = 0;
         aim_length_enable = 1;
		 aim_length_enable2 = 0;

         aim_mouse_length.value = 100.0f;
         aim_mouse_length.speed = 1.0f;
         aim_mouse_length.min = 10.0f;
         aim_mouse_length.max = 300.0f;
         aim_mouse_length_new = 0;
         aim_mouse_length_enable = 0;



         aim_bone_pos2 = { 0,0 };
         //aim_bone_pos3 = { 0,0,0 };
         //aim_bone_pos4 = { 0,0,0 };
		 aim_no_bone = 0;

         Player_enable = 0;
         npc_enable = 0;


         aim_mouse_enable = 0;
         aim_enable = 0;
         aim_gun_enable = 1;
		 

         fire_py = 0;
         aim_auto_fire_enable = 1;

         aim_stop_fire.max = 100;
         aim_stop_fire.min = 1;
         aim_stop_fire.speed = 1;
         aim_stop_fire.value = 50;
         aim_stop_fire_enable = 0;

         aim_switch_target.max = 5;
         aim_switch_target.min = 1;
         aim_switch_target.speed = 1;
         aim_switch_target.value = 1;
         aim_switch_target_enable = 1;

         aim_aotu_list.max = 50;
         aim_aotu_list.min = 10;
         aim_aotu_list.speed = 1;
         aim_aotu_list.value = 20;
         aim_aotu_list_enable = 0;

         aim_distance.max = 3000.0f;
             aim_distance.min = 10.0f;
             aim_distance.speed = 10.0f;
             aim_distance.value = 3000.0f;

			 aim_list_key_enable = 0;
             
			 aim_config();
     }
};

struct _other {

    bool fps;
	xs_uint sleep_time;
	void Other_config()
	{
		xs_config iniop;

		iniop.Read_bool_Config(VMProtectDecryptStringA("[other]"), VMProtectDecryptStringA("\nother_fps_enable="), &fps);

		iniop.Read_ulong_Config(VMProtectDecryptStringA("[other]"), VMProtectDecryptStringA("\nother_sleep_time="), &sleep_time.value);

		iniop.SaveConfig();
		iniop.OffConfig();
	}
	void Other_config_Write()
	{
		xs_config iniop;
		iniop.Write_bool_Config(VMProtectDecryptStringA("[other]"), VMProtectDecryptStringA("other_fps_enable="), fps);


		iniop.Write_ulong_Config(VMProtectDecryptStringA("[other]"), VMProtectDecryptStringA("other_sleep_time="), sleep_time.value);

		iniop.SaveConfig();
		iniop.OffConfig();
	}
    _other()
    {
        fps = 1;
		sleep_time.value = 50;
		sleep_time.speed = 1;
		sleep_time.min = 0;
		sleep_time.max = 100;

		Other_config();
    }
};


struct _weapon_data
{
    UINT64* table_this;//7F0
    UINT64* Decrypting_rdx_this;

    xsFloat3 aim;
	bool aim_bone_bool;
	xsFloat3 aim2;
	bool aim_bone2_bool;
	bool aim_no_bone;
    //bool*     fire;//开枪
    bool*     mirror;//开镜
    bool*     no_less_bullets;//不减子弹
    UINT32*   fire_bullets;//射出子弹数
    //UINT32*   bullet_speed;//射速
    UINT64*   bullet_clips_list;//弹夹链表
    UINT64*   bullet_confg_list;//配置链表

    //UINT64* bullet_clips_list3;//弹夹链表3 提前的 有BUG
    //UINT64* bullet_clips_list4;//弹夹链表4 固定的

    float* bullet_scatter0;//无散射0
    float* bullet_scatter1;//无散射1
    float* bullet_scatter2;//无散射2
    float* bullet_scatter3;//无散射3

    //UINT64* aim_call;

    xsFloat2 aim_bone_pos;
    bool aim_bone_pos_bool;



    inline void Write_table(UINT64& a) { table_this = (UINT64*)a; }
    inline void bone_pos(xsFloat2& a) { aim_bone_pos = a; aim_bone_pos_bool = 1; }
	inline void get_aim_pos(xsFloat3& a) { aim = a; aim_bone_bool = 1; }
	inline void get_aim_pos_2(xsFloat3& a) { aim2 = a; aim_bone2_bool = 1; }
	inline void init_aim_pos() { aim_bone_bool = 0; aim_bone_pos_bool = 0;}

    inline void get_mirror() { mirror = (bool*)(*table_this + _mirror); }
    inline void get_fire_bullet() { fire_bullets = (UINT32*)(*table_this + _Shoot_bullet_count); }
    inline void get_no_less_bullets() { no_less_bullets = (bool*)(*table_this + _false_bullet); }

    inline void get_gun_confg_list() { bullet_confg_list = (UINT64*)(*table_this + _gun_confg_list); }
	inline void get_bullet_clips_list() { bullet_clips_list = (UINT64*)(*table_this + _bullet_clips_list); }
    inline void get_bullet_scatter() { bullet_scatter0 = (float*)(*table_this + _No_scattering0); bullet_scatter1 = (float*)(*table_this + _No_scattering1); bullet_scatter2 = (float*)(*table_this + _No_scattering2);bullet_scatter3 = (float*)(*table_this + _No_scattering3);}



    inline UINT64 get_entity() { UINT64 a = *(UINT64*)(*table_this + _point_entity_this); if (a){ a = *(UINT64*)(a + 0x30); }return a; }

    inline UINT64* get_bullet_config_list(UINT64* _TISH) { 
        UINT64 a = *(UINT64*)(*_TISH + _gun_confg_list);
        if (a)
            a = *(UINT64*)(a+ _fully_auto_offset1);
        if(a)
            a = *(UINT64*)a;
        return (UINT64*)a;
       // return **(UINT64***)(*(UINT64*)(*_TISH + _hand_gun_list)+ _fully_auto_offset1); 
    }

    _weapon_data()
    {
        table_this = NULL;
        Decrypting_rdx_this = NULL;
        aim = { 0,0,0 };
		aim_bone_bool = 0;
		aim2 = { 0,0,0 };
		aim_bone2_bool = 0;
		aim_no_bone = 0;
        //fire = NULL;
        fire_bullets = NULL;
        mirror = NULL;
        no_less_bullets = NULL;
        bullet_clips_list = NULL;

        //bullet_clips_list3 = NULL;
        //bullet_clips_list4 = NULL;

        bullet_scatter0 = NULL;
        bullet_scatter1 = NULL;
        bullet_scatter2 = NULL;
        bullet_scatter3 = NULL;

        //bullet_speed = NULL;

        //aim_call = NULL;
        aim_bone_pos = { 0,0 };
        aim_bone_pos_bool = 0;

        bullet_confg_list = NULL;
    }
};

struct _entity_data//
{
    UINT64*        table_this;
    UINT64*        bone_info_this;
    UINT32         bone_info_count;
    _weapon_data   weapon;
    float*         matrix;

    float          distance;
    char           name_list;
    bool           enemy;
    bool           npc;
    UINT32         ID;
    char*          UID;
    bool           aim_pitch;//透视被 瞄准 骨骼标识
    xsFloat2       aim_pitch_position;

    xsFloat3       mov_speed;
    xsFloat3*      position;

    UINT64*        bone_this;
    UINT32         bone_count;

	UINT64*        bone2_this;
	UINT32         bone2_count;

	UINT64         wrj_aim_call;
	UINT64         wrj_aim_r8;

    char*          name;
    UINT32*        experience;
    UINT32*        grade;
    UINT32*        blood_top;
    UINT32*        blood;
    UINT32*        Armor_top;
    UINT32*        Armor;

    xsFloat2       screen_s;//屏幕box
    xsFloat2       screen_x;
    bool           screen_s_bool;
    bool           screen_x_bool;
    float          high;



    inline void write_matrix(float* a) { matrix = a; }

    inline void Write_table(UINT64& a) { table_this = (UINT64*)a; }
    inline void Write_bone_info() {
		UINT64 bone_info = *(UINT64*)(*table_this + _entity_bone_info_1offset);
		if (is_addr(bone_info))
			bone_info = *(UINT64*)(bone_info + _entity_bone_info_2offset);
		if (is_addr(bone_info))
			bone_info = *(UINT64*)(bone_info + _entity_bone_info_3offset);
		if (is_addr(bone_info))
		{
			bone_info_count = *(UINT32*)(bone_info + _entity_bone_info_count);
			bone_info_this = *(UINT64**)(bone_info + _entity_bone_info);
		}
		else
		{
			bone_info_count = 0;
			bone_info_this = 0;
		}
    }


    inline bool isenemy(UINT64 locality,UINT64 enemy) {
        return(*(bool(__stdcall*)(UINT64, UINT64))(*(UINT64*)(*(UINT64*)*table_this + _Is_enemy_call)))(locality, enemy);
    }
    inline float calc_distance(xsFloat3 a)
    {
        distance = sqrtf(powf(position->x - a.x, 2.0f) + powf(position->z - a.z, 2.0f) + powf(position->y - a.y, 2.0f));
        return distance;
    }

    inline bool isplayer() { if (*(UINT32*)(*table_this + _npc_sign) == 2) npc = 1; return npc; }
    inline void write_id(UINT32& a) { ID = a; }

    inline void getUID() { UID = (char*)(*table_this + _UID); }

    inline void getposition() { position = (xsFloat3*)(*table_this + _position); }
	inline bool gettime_v(int time_) { return ((int)(*(UINT32*)(*table_this + _time_now) - *(UINT32*)(*table_this + _time)) > time_); }
	inline void time_v() { if (*(UINT32*)(*table_this + _time) == 0) { *(UINT32*)(*table_this + _time) = *(UINT32*)(*table_this + _time_now); } }
    inline void getmov_speed(UINT64 a) { 
        UINT64 aa = *(UINT64*)(a + 0x118);
        if (aa)
            aa = *(UINT64*)(aa + 0x168);
        if (aa)
            a = *(UINT64*)(a + 0x278);
        if (aa)
            mov_speed = *(xsFloat3*)(a + 0xB0);
       }
   

	inline void getname() {
		if (*(char*)(*table_this + _name_bool) == (char)0x01)
		{
			char* aa = (char*)*(UINT64*)(*table_this + _name);
			if (is_addr((UINT64)aa))
				if (*(char*)aa == (char)0x00)
					name = (char*)VMProtectDecryptStringA("null");
				else
					name = aa;
			else
				name = (char*)VMProtectDecryptStringA("null");
		}
		else {
			char* aa = (char*)(*table_this + _name);
			if (*(char*)aa == (char)0x00)
				name = (char*)VMProtectDecryptStringA("null");
			else
				name = aa;

		}
	}
	inline void getexperience() { experience = (UINT32*)(*table_this + _experience); }
    inline void getgrade() { grade = (UINT32*)(*table_this + _grade); }

	inline void getblood() {
		UINT64* _temp_2offset = (UINT64*)(*table_this + _entity_mation_2offset);
		if (is_addr(*_temp_2offset))
		{
			blood = (UINT32*)(*_temp_2offset + _blood);
			blood_top = (UINT32*)(*_temp_2offset + _blood_top);
		}
	}
	inline void getarmor() {
		UINT64* _temp_2offset = (UINT64*)(*table_this + _entity_mation_2offset);
		if (is_addr(*_temp_2offset))
		{
			Armor = (UINT32*)(*_temp_2offset + _armor);
			Armor_top = (UINT32*)(*_temp_2offset + _armor_top);
		}
	}
	inline void getnpcarmor() {
		UINT64* _temp_2offset = (UINT64*)(*table_this + _entity_mation_2offset);
		if (is_addr(*_temp_2offset)) {
			Armor = (UINT32*)(*_temp_2offset + _npc_armor);
			Armor_top = (UINT32*)(*_temp_2offset + _npc_armor_top);
		}
	}

    inline void get_bone_this() {
        UINT64 aa = *(UINT64*)(*table_this + 0x228 + 8);
        if (is_addr(aa))
            aa = *(UINT64*)(aa + 0x28);
        if (is_addr(aa))
			aa = *(UINT64*)(aa + 0x90);
		if (is_addr(aa))
			bone_this = (UINT64*)(aa);
		else
            bone_this = 0;
    }
    inline void get_bone_count() {
        UINT64 aa = *(UINT64*)(*table_this + 0x228 + 8);
        if (is_addr(aa))
            aa = *(UINT64*)(aa + 0x28);
		if (is_addr(aa))
			bone_count = *(UINT32*)(aa + 0x98);
		else
			bone_count = 0;
    }

	inline void get_bone2_this() {
		UINT64 aa = *(UINT64*)(*table_this + _bone_offset2);
		if (is_addr(aa))
			aa = *(UINT64*)(aa + _bone_offset3);
		if (is_addr(aa))
			bone2_this = (UINT64*)(aa);
		if (!is_addr(aa))
			bone2_this = 0;
	}
	inline void get_bone2_count() {
		UINT64 aa = *(UINT64*)(*table_this + _bone_offset2);
		if (is_addr(aa))
			bone2_count = *(UINT32*)(aa + _bone_count);
		else
			bone2_count = 0;
	}

    inline xsFloat3 get_bone_this_position(UINT64 a) {
        if (!bone_this || !is_addr((UINT64)bone_this[a]))
            return  xsFloat3(0,0,0);

        xsFloat3 aa = *(xsFloat3*)((UINT64)bone_this[a] + 0x60);

        return aa;
    }
	inline xsFloat3 get_bone_this_position_2(UINT64 a) {
		if (!bone_this || !is_addr((UINT64)bone_this[a]))
			return  xsFloat3(0, 0, 0);

		xsFloat3 aa = *(xsFloat3*)((UINT64)bone_this[a] + 0x60);
		float* ac = (float*)((UINT64)bone_this[a] + 0x30);
		aa.x -= ac[0] - ac[4] - ac[8];
		aa.y -= ac[1] - ac[5] - ac[9];
		aa.z -= ac[2] - ac[6] - ac[10];
		return aa;
	}
    //可否攻击
    inline bool get_bone_this_inval(UINT64 a) {
        if (bone_this || is_addr((UINT64)bone_this[a]))
            if (*(UINT16*)((UINT64)bone_this[a] + 0xED) == (UINT16)0xE9)
                return 0;
        return  1;
    }

    inline UINT64 get_bone_this_info(UINT64 a) {
        if(!bone_this || !is_addr((UINT64)bone_this[a]))
            return  0;
        UINT64 aa = *(UINT64*)(*(UINT64*)bone_this[a] + 0x30);
        UINT64* info_key= (*(UINT64 * (__stdcall*)(UINT64))(aa)) (bone_this[a]);
        for (size_t i = 0; i < bone_info_count; i++)
        {
            UINT count = i * 3;
            if (info_key[0] == bone_info_this[count] && info_key[1] == bone_info_this[count + 1])
                return bone_info_this[count + 2];
        }
        return  0;
    }

    inline UINT64 get_bone_this_info_count(UINT64 a) {
        if (!bone_this || !is_addr((UINT64)bone_this[a]))
            return  0;
        UINT64 aa = *(UINT64*)(*(UINT64*)bone_this[a] + 0x30);
        UINT64* info_key = (*(UINT64 * (__stdcall*)(UINT64))(aa)) (bone_this[a]);
        for (size_t i = 0; i < bone_info_count; i++)
        {
            UINT count = i * 3;
            if (info_key[0] == bone_info_this[count] && info_key[1] == bone_info_this[count + 1])
                return i;
        }
        return  0;
    }

	inline char* get_bone_this_info_name(UINT64 a) {
		if (!is_addr((UINT64)a))
			return (char*)VMProtectDecryptStringA("null");

		char* axa = NULL;
		if (*(char*)(a + 0x20) == (char)0x00)
		{
			axa = (char*)VMProtectDecryptStringA("null");
			return axa;
		}
		if (*(char*)(a + 0x2F) == (char)0x01)
		{
			axa = (char*)*(UINT64*)(a + 0x20);
			if (!is_addr((UINT64)axa))
				axa = (char*)VMProtectDecryptStringA("null");
		}
		else
			axa = (char*)(a + 0x20);

		return axa;
	}
    //可否攻击
    inline UCHAR get_bone_this_info_inval(UINT64 a) { 
        if (*(char*)(a + 0x3C) != 0)
            return 1;
        if (*(char*)(a + 0x3D) != 0)
            return 0;
        if (*(UINT32*)(a + 0x48) == (UINT32)5)
            return 2;
        return 0;
 }

    inline int get_bone_name_this_cmp(char* bone_name) {

        for (size_t i = 0; i < bone_count; i++)
        {
			if (get_bone_this_inval(i))
				continue;
            UINT64 info_this=get_bone_this_info(i);
            if (info_this)
                if (strcmp(bone_name, get_bone_this_info_name(info_this)) == 0)
                    //if (!get_bone_this_info_inval(info_this))
                        return i;
        }
        return -1;
    }

    inline int get_bone_name_this_str(char* bone_name) {

        for (size_t i = 0; i < bone_count; i++)
        {
            if (get_bone_this_inval(i))
                continue;
            UINT64 info_this = get_bone_this_info(i);
            if (info_this)
                if (strstr(get_bone_this_info_name(info_this), bone_name))
                    //if (!get_bone_this_info_inval(info_this))
                        return i;
        }
        return -1;
    }

	inline xsFloat3* call_wrj_aim()
	{
		xsFloat4 a;
		return (*(xsFloat3*(__stdcall*)(UINT64, UINT64, UINT64))((UINT64)(wrj_aim_call)))((UINT64)&a, *table_this, wrj_aim_r8);
	}

	inline void get_wrj_aim_call(UINT64 a)
	{
		wrj_aim_call = a;
	}
	inline void get_wrj_aim_r8(UINT64 a)
	{
		wrj_aim_r8 = a;
	}

	inline char* get_bone2_name_call(char* szOut,UINT32 idx)
	{
		UINT64 aa = *(UINT64*)(*table_this + _bone_offset2);
		UINT64 aaC = aa;
		if (is_addr(aa))
			aa = *(UINT64*)aa;
		else
			return (char*)VMProtectDecryptStringA("null");

		if (is_addr(aa))
			aa = *(UINT64*)(aa + _bone_name_call);
		else
			return (char*)VMProtectDecryptStringA("null");
		return (*(char*(__stdcall*)(UINT64, char*, UINT32))((UINT64)(aa)))(aaC, szOut, idx);
	}
	inline UINT32 get_bone2_idx_call(char* name)
	{
		UINT64 aa = *(UINT64*)(*table_this + _bone_offset2);
		UINT64 aaC = aa;
		if (is_addr(aa))
			aa = *(UINT64*)aa;
		else
			return 0;

		if (is_addr(aa))
			aa = *(UINT64*)(aa + _bone_id_call);
		else
			return 0;
		return (*(UINT32(__stdcall*)(UINT64, char*))((UINT64)(aa)))(aaC, name);
	}
	inline UINT32 get_bone2_count_call()
	{
		UINT64 aa = *(UINT64*)(*table_this + _bone_offset2);
		UINT64 aaC = aa;
		if (is_addr(aa))
			aa = *(UINT64*)aa;
		else
			return 0;

		if (is_addr(aa))
			aa = *(UINT64*)(aa + _bone_count_call);
		else
			return 0;
		return (*(UINT32(__stdcall*)(UINT64))((UINT64)(aa)))(aaC);
	}

	inline xsFloat3 get_bone2_pos_call(UINT32 idx)
	{
		UINT64 aa = *(UINT64*)(*table_this + _bone_offset2);
		UINT64 aaC = aa;
		if (is_addr(aa))
			aa = *(UINT64*)aa;
		else
			return xsFloat3(0,0,0);

		if (is_addr(aa))
			aa = *(UINT64*)(aa + _bone_pos_call);
		else
			return xsFloat3(0, 0, 0);
		return *(xsFloat3*)&(*(xsFloat4*(__stdcall*)(UINT64, UINT32))((UINT64)(aa)))(aaC, idx)[3];
	}
    inline void write_screen_s(xsFloat2 a) { screen_s = a; screen_s_bool = 1; }
    inline void write_screen_x(xsFloat2 a) { screen_x = a; screen_x_bool = 1; }


    inline bool WorldToScreen(xsFloat3* entity_position, xsFloat2* screen_position,xsFloat2* window)
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


    _entity_data() {
        table_this = NULL;
        bone_info_this = NULL;
        bone_info_count = 0;
        enemy = 0;
        npc = 0;
        ID = -1;

        distance = 0;
        mov_speed = {0,0,0};
        position = NULL;
        bone_this = NULL;
        bone_count = 0;
		bone2_this = NULL;
		bone2_count = 0;
        bone_info_this = NULL;
        bone_info_count = 0;
		wrj_aim_call = NULL;
		wrj_aim_r8 = NULL;


        name = NULL;
        experience = NULL;
        grade = NULL;
        blood_top = NULL;
        blood = NULL;
        Armor_top = NULL;
        Armor = NULL;

        screen_s = { 0,0 };
        screen_s_bool = 0;
        screen_x = { 0,0 };
        screen_x_bool = 0;
        high = 0;

        aim_pitch_position = { 0,0 };
        aim_pitch = 0;
        UID = NULL;
    }
};

struct _materia_data//
{
    UINT64*  table_this;

    UINT32       ID;
    UINT64*  type;
    UINT8*   plunder;
    UINT8*   Show_Fkey;
    bool* is_cheat;

    xsFloat3* position0;
    xsFloat3* position1;
    xsFloat3* position2;
    xsFloat3* position3;
    xsFloat3* position4;
    xsFloat3* drop_position;
	UINT32* drop_count;
	UINT64* drop_name;
    //UINT32* drop_data里数;
    xsFloat2  screen;
    bool       screen_bool;

    UINT64 drop_type;
    UINT64 materia_type;

    inline void Write_table(UINT64& a) { table_this = (UINT64*)a; }
    inline void write_screen(xsFloat2& a) { screen = a; screen_bool = 1; }
    inline void write_id(UINT32& a) { ID = a; }

    inline void get_type() { type = (UINT64*)(*table_this + _type); }
    inline void get_plunder() { plunder = (UINT8*)(*table_this + _isPlunder); }
    inline bool drop_is_Fkey() {  return *(bool*)(UINT8*)(*table_this + _isF_Plunder);}
    inline void get_Show_Fkey() { Show_Fkey = (UINT8*)(*table_this + _Show_Fkey); }
    inline void get_plunder0() { position0 = (xsFloat3*)(*table_this + _Plunder0); }
    inline void get_cheat() { is_cheat = (bool*)(*table_this + _Plunder0 + 12); }
    inline void get_plunder1() { position1 = (xsFloat3*)(*table_this + _Plunder1); }
    inline void get_plunder2() { position2 = (xsFloat3*)(*table_this + _Plunder2); }
    inline void get_plunder3() { position3 = (xsFloat3*)(*table_this + _Plunder3); }
    inline void get_plunder4() { position4 = (xsFloat3*)(*table_this + _astigmatism); }
	inline void get_drop() {
		UINT64 aa = *(UINT64*)(*table_this + _drop_offset1);
		if (is_addr(aa))
			drop_position = (xsFloat3*)(aa + _drop);
		else
			drop_position = NULL;
	}
	inline void get_drop_count() {
		UINT64 aa = *(UINT64*)(*table_this + _drop_offset1);
		if (is_addr(aa))
			drop_count = (UINT32*)(aa + _drop_count);
		else
			drop_count = NULL;
	}
	inline void get_drop_name() {
		UINT64 aa = *(UINT64*)(*table_this + _drop_offset1);
		if (is_addr(aa))
			drop_name = (UINT64*)(aa + _drop_name_0);
		else
			drop_name = NULL;
	}
	inline char* get_drop_name(UINT32 i) {
		UINT64 aa = *(UINT64*)(*drop_name + 0xB0 + (_drop_name_size * i) + _drop_name_1);
		if (is_addr(aa))
		{
			if (*(char*)(aa + 0x10 + 0xF) == (char)0x01)
			{
				if (is_addr(*(UINT64*)(aa + 0x10)))
					return (char*)*(UINT64*)(aa + 0x10);
				else
					return (char*)VMProtectDecryptStringA("null");
			}
			else if (*(char*)(aa + 0x10) != (char)0x00)
			{
				return (char*)(aa + 0x10);
			}
		}

		return (char*)VMProtectDecryptStringA("null");
	}
	inline bool is_drop_player(char* a) {

		for (size_t i = 0; i < *drop_count; i++)
		{
			if (strstr(get_drop_name(i), a))
				return 1;
		}
		return 0;
	}

    inline UINT64* get_supplies_1_call() { return (UINT64*)(*(UINT64*)*table_this + _materia_call_1); }
    inline UINT64* get_supplies_4_call() { return (UINT64*)(*(UINT64*)*table_this + _materia_call_4); }
    inline UINT64* get_supplies_F_call() { return (UINT64*)(*(UINT64*)*table_this + _materia_call_F); }

    inline void get_sign(UINT64 c) { 
        if (!c)
            return;
        UINT64 a = *(UINT64*)(c + _drop_materia_1offset);
		if (!is_addr(a))
            return;
        drop_type = *(UINT64*)(a + _drop_sign);
        materia_type = *(UINT64*)(a + _materia_sign);}

    _materia_data() {
        table_this = NULL;
        ID = 0;

        type = NULL;
        plunder = NULL;

        Show_Fkey = NULL;
        position0 = NULL;
        position1 = NULL;
        position2 = NULL;
        position3 = NULL;
        position4 = NULL;
        drop_position = NULL;
		drop_count = NULL;
		drop_name = NULL;

        screen = { 0,0 };
        screen_bool = 0;
        drop_type = 0;
        materia_type = 0;
    }
};

struct _name_list_data
{
    char Black_White = 0;
    char UID[0x36] = { 0 };
    inline void set_uid(char* a) { memmove(UID, a, 0x36); }
    inline char set_list() { Black_White += 1; if (Black_White > 2) Black_White = 0; return Black_White; }
    inline char cmp_uid(char* a) { if(strstr(a, UID))return Black_White; return -1;}
};

struct _cheat_data//Data集类
{
    UINT64 table_this;
    UINT64 module_addr;
    UINT64 drop_materia_this;
    float* matrix;
	UINT64 wrj_aim_call;
	UINT64 wrj_aim_r8_this;
	UINT64 wrj_aim_r8;

    UINT64 hook_weapon_this;
    UINT64 hook_aim_this;
    UINT64 hook_materia_this;

    UINT64 weapon_table_this;
    UINT64 aim_table_this;
    UINT64 materia_table_this;

    xs_DataFrame<_entity_data>    player_data;
    xs_DataFrame<_entity_data>    npc;

    xs_DataFrame<_materia_data>    drop_data;
    xs_DataFrame<_materia_data>    materia_data;

    xs_DataFrame<_name_list_data>    name_list;

    _esp esp_draw;
    _aim aim;
    _supplies supplies;
    _Player player;
    _Weapon weapon;
    _other other;

    _Message message;

	bool config_read;
	bool config_write;

    inline void updata_player() {   
		////MessageBoxA(0, VMProtectDecryptStringA("=--0"), "", 0);

		UINT64 temp_entity_this = *(UINT64*)(table_this + _entity_1offset);
		set_vva(temp_entity_this);
		if (!is_addr(temp_entity_this))
			return;

		if (!wrj_aim_r8)
		{
			UINT64 mod_dingwei = *(UINT64*)wrj_aim_r8_this;
			UINT32 mod_dingwei_count = *(UINT32*)(wrj_aim_r8_this + 8);
			////MessageBoxA(0, VMProtectDecryptStringA("=--1"), "", 0);
			for (size_t i = 0; i < mod_dingwei_count; i++)
			{
				UINT64 wrj_r8_name = (mod_dingwei + (i * 0x18));
				if (is_addr(wrj_r8_name))
					if (strstr((char*)*(UINT64*)wrj_r8_name, VMProtectDecryptStringA("hunter/ui/graphs/skills_postdeploymenttargeting.muigraph")))
					{
						wrj_aim_r8 = *(UINT64*)
							(*(UINT64*)
							(*(UINT64*)(wrj_r8_name + 0x10) + 0x30)
								+ 0xC8)
							+ 0x8A0;
						break;
					}
			}
			////MessageBoxA(0, VMProtectDecryptStringA("=--2"), "", 0);
		}


		UINT64 temp_entity_this2 = temp_entity_this;
		////MessageBoxA(0, VMProtectDecryptStringA("=--3"), "", 0);
        UINT32 entity_count = *(UINT32*)(temp_entity_this + _entityCount_2offset);
        if (!entity_count)
        {
            //weapon.deceit_bullets_enable = 0;
            //weapon.deceit_bullets_enable2 = 0;
            return;
        }
		////MessageBoxA(0, VMProtectDecryptStringA("=--4"), "", 0);
        temp_entity_this = *(UINT64*)(temp_entity_this + _entity_2offset);
		if (!is_addr(temp_entity_this))
			return;
		////MessageBoxA(0, VMProtectDecryptStringA("=--5"), "", 0);
        if (!*(UINT64*)temp_entity_this)
            return;
        //if(! *(UINT64*)(*(UINT64*)(table_this + _Cartridge_list3_1) + _Cartridge_list3_2))
        //    return;

		////MessageBoxA(0, VMProtectDecryptStringA("=--6"), "", 0);
        //UINT32 实体bool = *(UINT32*)(table_this + _entity_death_offset);
        //实体bool &= 0xFF000;
        //if (!实体bool)//10234
        //    return;

        for (size_t i = 0; i < entity_count; i++)
        {
			if (entity_count != *(UINT32*)(temp_entity_this2 + _entityCount_2offset))
			{
				player_data.clear();
				npc.clear();
				break;
			}

            //char a[256] = { 0 };
            //sprintf(a, "%d ,%d", i, entity_count);
            ////MessageBoxA(0, VMProtectDecryptStringA("--0"), "", 0);
            _entity_data v;
            v.table_this = (UINT64*)(temp_entity_this + i * 8);
			if (!is_addr(*v.table_this))
				continue;
			v.time_v();
			if (i && other.sleep_time.value && !v.gettime_v(other.sleep_time.value))//必须不然闪退
				continue;
            //v.write_matrix(matrix);

            v.ID = i;
            v.Write_bone_info();
            v.get_bone_this();
            v.get_bone_count();
			v.get_bone2_this();
			v.get_bone2_count();
			////MessageBoxA(0, VMProtectDecryptStringA("--1"), "", 0);
			v.get_wrj_aim_call(wrj_aim_call);
			v.get_wrj_aim_r8(wrj_aim_r8);

            if (!is_addr(*(UINT64*)(*v.table_this + _entity_mation_2offset)))
                continue;

            v.getposition();
            
            v.getname();
            v.getblood();
            
            v.getUID();
			////MessageBoxA(0, VMProtectDecryptStringA("--2"), "", 0);
            v.weapon.table_this = (UINT64*)(*v.table_this + _weapon_2offset);
            if (!is_addr(*v.weapon.table_this))
                continue;
           // if ((*v.weapon.table_this > 0x40000 && *v.weapon.table_this < 0x7FFFFFFF0000))
           // {    /////continue;
           //// v.weapon.Decrypting_rdx_this = (UINT64*)(module_addr + _Decryption_rdx_BaseAddress);

           // //v.weapon.get_fire();
           //     v.weapon.get_mirror();
           //     v.weapon.get_fire_bullet();
           //     v.weapon.get_gun_confg_list();
           //     v.weapon.get_bullet_scatter();
           //     v.weapon.get_no_less_bullets();
           //     //v.weapon.get_auto_bullets_tish(module_addr);
           //     //if (*v.目标 == 1)
           // }
                if (!v.isplayer())
                {
                    v.getarmor();
                    if (!i)
                    {
                        //v.getmov_speed(table_this);
                        //v.weapon.get_bullet_clips_list_s(table_this);
                        v.weapon.get_mirror();
                        v.weapon.get_fire_bullet();
                        v.weapon.get_gun_confg_list();
						//v.weapon.get_bullet_clips_list();

                        v.weapon.get_bullet_scatter();
                        v.weapon.get_no_less_bullets();

                        //v.getexperience();
                        //v.getgrade();
                        v.enemy = 0;
                    }
                    else
                    {
                        //v.getexperience();
                        //v.getgrade();
                        v.enemy = player_data.Data[0].isenemy(*player_data.Data[0].table_this, *v.table_this);
                        
                        v.calc_distance(*player_data.Data[0].position);

                        for (size_t i = 0; i < name_list.Size; i++)
                        {
                            char aa = name_list.Data[i].cmp_uid(v.UID);
                            if (aa != (char)-1)
                                v.name_list = aa;
                        }

                    }
                    player_data.push_back(v);
                }
                else
                {
                    v.getnpcarmor();
                    //v.getexperience();
                    //v.getgrade();
                    v.enemy = player_data.Data[0].isenemy(*player_data.Data[0].table_this, *v.table_this);
                    
                    v.name_list = *v.UID;
                    v.calc_distance(*player_data.Data[0].position);

                    npc.push_back(v);
                }

        }

    }

    inline void updata_materia(){

        UINT64 temp_entity_this = *(UINT64*)(table_this + _materia_1offset);
		if (!is_addr(temp_entity_this))
			return;

        temp_entity_this = *(UINT64*)(temp_entity_this + _materia_2offset);
		if (!is_addr(temp_entity_this))
			return;
        temp_entity_this = *(UINT64*)(temp_entity_this + _materia_3offset);
		if (!is_addr(temp_entity_this))
			return;
		UINT64 temp_entity_this2 = temp_entity_this;
        UINT32 entity_count = *(UINT32*)(temp_entity_this + _materiaCount_4offset);
        if (!entity_count)
            return;

        temp_entity_this = *(UINT64*)(temp_entity_this + _materia_4offset);
		if (!is_addr(temp_entity_this))
			return;
        if (!*(UINT64*)temp_entity_this)
            return;

        for (size_t i = 0; i < entity_count; i++)
        {
			if (entity_count != *(UINT32*)(temp_entity_this2 + _materiaCount_4offset))
			{
				drop_data.clear();
				break;
			}
            _materia_data v;
            v.table_this = (UINT64*)(temp_entity_this + i * 8);
			if (!is_addr(*v.table_this))
				continue;

            v.ID = i;
            v.get_type();
            //v.get_plunder();
             //v.get_Show_Fkey();
            //v.get_plunder0();
            //v.get_cheat();
             //v.get_plunder1();
            //v.get_plunder2();
            //v.get_plunder3();
            //v.get_plunder4();
             //v.get_drop();
            //v.get_drop_col();
            v.get_sign(drop_materia_this);
            //if (*v.type == v.materia_type)
            //{
            //    if (!*(UINT64*)hook_materia_this)
            //        memmove((void*)hook_materia_this, (UINT64*)((*(UINT64*)*v.table_this)- 0x500), 0x1000);
            //    if (*(UINT64*)*v.table_this != hook_materia_this + 0x500)
            //        *(UINT64*)*v.table_this = hook_materia_this + 0x500;
            //    materia_data.push_back(v);
            //}else 
			if (*v.type == v.drop_type)
			{
				v.get_Show_Fkey();
				v.get_plunder1();
				v.get_drop();
				if (v.drop_position == NULL)
					continue;
				v.get_drop_count();
				if (v.drop_count == NULL)
					continue;
				v.get_drop_name();
				if (v.drop_name == NULL)
					continue;

				drop_data.push_back(v);
			}
        }
    }




    _cheat_data() {
		config_read = 0;
		config_write = 0;

		const WCHAR* TheDivision2_wchar =VMProtectDecryptStringW(L"TheDivision2.exe");
        getModuleInformation(TheDivision2_wchar, &module_addr);

		//MessageBoxA(0, VMProtectDecryptStringA("1"), 0, 0);
		const char* pattern= VMProtectDecryptStringA("\xE8????\x48\x8B\x0D????\x44\x8B\xC3\x48\x8B\xD7\xE8");//E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 44 8B C3 48 8B D7 E8


        UINT64 find_addr = findPattern(TheDivision2_wchar, pattern,19);
		VMProtectFreeString(pattern);
        find_addr += 0x8;
        find_addr = Compile_PointAddr(find_addr);
        table_this = *(UINT64*)find_addr;

		//MessageBoxA(0, VMProtectDecryptStringA("2"), 0, 0);
		pattern = VMProtectDecryptStringA("\x5D\xC3\x4C\x8D\x05????\x48\x8D\x0D????\xE8");//5D C3 4C 8D 05 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? E8
        find_addr = findPattern(TheDivision2_wchar, pattern,17);
		VMProtectFreeString(pattern);
        find_addr += 12;
        find_addr = Compile_PointAddr(find_addr);
        matrix = (float*)(find_addr + 0x690);

		//无人机瞄准call r8参数
		//MessageBoxA(0, VMProtectDecryptStringA("2-1"), 0, 0);
		pattern = VMProtectDecryptStringA("\x74\x2D\x49\x8B\x57\x30\x48\x85\xD2\x74\x24\x49\xC1\xE0\x04\x48\x8D\x4C\x24?\x4D\x03\xC4\xE8????\x8B\x08");//74 2D 49 8B 57 30 48 85 D2 74 24 49 C1 E0 04 48 8D 4C 24 ?? 4D 03 C4 E8 ?? ?? ?? ?? 8B 08
		find_addr = findPattern(TheDivision2_wchar, pattern, 30);
		VMProtectFreeString(pattern);
		find_addr += 0x18;
		find_addr = Compile_PointAddr(find_addr);
		wrj_aim_call = find_addr;
		//MessageBoxA(0, VMProtectDecryptStringA("2-2"), 0, 0);
		pattern = VMProtectDecryptStringA("\x48\x63\xC8?\x8D\x34\x49\x48\x8B\x0D?????\x8D\x76\x02?\x8D\x34\xF1");//48 63 C8 ?? 8D 34 49 48 8B 0D ?? ?? ?? ?? ?? 8D 76 02 ?? 8D 34 F1
		find_addr = findPattern(TheDivision2_wchar, pattern, 22);
		VMProtectFreeString(pattern);
		find_addr += 0xA;
		find_addr = Compile_PointAddr(find_addr);
		wrj_aim_r8_this = find_addr;
		wrj_aim_r8 = 0;

        //子弹追踪
		//MessageBoxA(0, VMProtectDecryptStringA("3"), 0, 0);
		pattern = VMProtectDecryptStringA("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x60\x48\x8B\xB1");//48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B B1

        find_addr = findPattern(TheDivision2_wchar, pattern, 23);
		VMProtectFreeString(pattern);

        find_addr = findPattern(TheDivision2_wchar, find_addr, -1);

        //开火
		//MessageBoxA(0, VMProtectDecryptStringA("4"), 0, 0);
		pattern = VMProtectDecryptStringA("\x80\xB9?????\x0F\x94\xC0\xC3\xCC\xCC\xCC\xCC\xCC\xB0\x01\xC3");// 80 B9 ?? ?? ?? ?? ?? 0F 94 C0 C3 CC CC CC CC CC B0 01 C3

        UINT64 find_addr2= findPattern(TheDivision2_wchar, pattern, 19);
		VMProtectFreeString(pattern);


		find_addr2 += 0x10;
        find_addr2 = findPattern(TheDivision2_wchar, find_addr2, -1);

        aim.aim_hook_Init(find_addr, find_addr2);
       

        //射速
		//MessageBoxA(0, VMProtectDecryptStringA("5"), 0, 0);
        UINT64 weapon_speed_call[2];
		pattern = VMProtectDecryptStringA("\x80\xB9?????\x0F\x97\xC0\xC3");//80 B9 ?? ?? ?? ?? 00 0F97 C0 C3

        weapon_speed_call[0] = findPattern(TheDivision2_wchar, pattern,11 );
		VMProtectFreeString(pattern);



        weapon_speed_call[0] = findPattern(TheDivision2_wchar, weapon_speed_call[0], 2);

		//MessageBoxA(0, VMProtectDecryptStringA("6"), 0, 0);
		pattern = VMProtectDecryptStringA("\x41\xB8????\x84\xC0\x74\x0A\x8B\x8B");//41 B8 ?? ?? ?? ?? 84 C0 74 0A 8B 8B

        weapon_speed_call[1]= findPattern(TheDivision2_wchar, pattern, 12);
		VMProtectFreeString(pattern);

        //子弹数量
		////MessageBoxA(0, VMProtectDecryptStringA("7"), 0, 0);
        UINT64 weapon_bullet_call[2];
		pattern = VMProtectDecryptStringA("\x40\x55\x41\x56\x48\x8B\xEC\x48\x83\xEC?\x4C\x8B\xF1\x84\xD2");//40 55 41 56 48 8B EC 48 83 EC ?? 4C 8B F1 84 D2
        weapon_bullet_call[0] = findPattern(TheDivision2_wchar, pattern,16 );
		VMProtectFreeString(pattern);

        weapon_bullet_call[0] = findPattern(TheDivision2_wchar, weapon_bullet_call[0], -1);

		////MessageBoxA(0, VMProtectDecryptStringA("8"), 0, 0);
		pattern = VMProtectDecryptStringA("\x84\xC0\x75?\x49\x8B\x06\x33\xD2\x49\x8B\xCE\x44\x8D\x42\xFF");//84 C0 75 22 49 8B 06 33 D2 49 8B CE 44 8D 42 FF
        weapon_bullet_call[1] = findPattern(TheDivision2_wchar, pattern, 16);
		VMProtectFreeString(pattern);

        //无限备弹
		//MessageBoxA(0, VMProtectDecryptStringA("9"), 0, 0);
        UINT64 weapon_back_bullet_call[3];
		pattern = VMProtectDecryptStringA("\x48\x89\x41\x40\xC7\x41\x48????\x48\x83\xC1");//48 89 41 40 C7 41 48 ?? ?? ?? ?? 48 83 C1

        weapon_back_bullet_call[0] = findPattern(TheDivision2_wchar, pattern, 14);
		VMProtectFreeString(pattern);


        weapon_back_bullet_call[0] -= 0x47;
        weapon_back_bullet_call[0] = findPattern(TheDivision2_wchar, weapon_back_bullet_call[0], -1);

		pattern = VMProtectDecryptStringA("\xF0\xFF\x43\x08\x4C\x8B?\x48\x8B?\x48\x8B?\xE8??????????????????\xE8");//F0 FF 43 08 4C 8B ?? 48 8B ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? E8
        weapon_back_bullet_call[1] = findPattern(TheDivision2_wchar, pattern,33 );
		VMProtectFreeString(pattern);

		weapon_back_bullet_call[2] = Compile_PointAddr(weapon_back_bullet_call[1] + 0xE);

        //无视换弹动作
		//MessageBoxA(0, VMProtectDecryptStringA("10"), 0, 0);
        UINT64 weapon_reload_bullet_call[2];
		/*pattern = VMProtectDecryptStringA("\x48\x8B\x92????\x48\x85\xD2\x74\x0B");

        weapon_reload_bullet_call[0] = findPattern(TheDivision2_wchar, pattern, 12);
		VMProtectFreeString(pattern);


		////MessageBoxA(0, VMProtectDecryptStringA("11"), 0, 0);
        weapon_reload_bullet_call[0] = findPattern(TheDivision2_wchar, weapon_reload_bullet_call[0], 2);
        
		pattern = VMProtectDecryptStringA("\x44\x8B\xF8\xEB\x03\x44\x8B\xFF\x48\x8B\xCB");
		weapon_reload_bullet_call[1]=findPattern(TheDivision2_wchar, pattern, 11);
		VMProtectFreeString(pattern); */

		//MessageBoxA(0, VMProtectDecryptStringA("12"), 0, 0);
        weapon.weapon_Init(weapon_speed_call, weapon_bullet_call, weapon_back_bullet_call, weapon_reload_bullet_call);

		//MessageBoxA(0, VMProtectDecryptStringA("13"), 0, 0);
        player.hook_speed_init();

        pattern = VMProtectDecryptStringA("\x89\x4B\x08\x48\x89\x03\x66\x89\x4B\x0C\x48\x89\x4B\x10\x48\x89\x4B\x18\x48\x89\x4B\x20\xC7\x43\x28????\x89\x4B\x2C\xC7\x43\x30\xFF\xFF\xFF\xFF\x48\x8B\x05");//89 4B 08 48 89 03 66 89 4B 0C 48 89 4B 10 48 89 4B 18 48 89 4B 20 C7 43 28 ?? ?? ?? ?? 89 4B 2C C7 43 30 FF FF FF FF 48 8B 05
        find_addr = findPattern(TheDivision2_wchar, pattern, 42);
        VMProtectFreeString(pattern);

        find_addr += 0x2A;
        find_addr = Compile_PointAddr(find_addr);
        drop_materia_this = *(UINT64*)(find_addr);
        //MessageBoxA(0, VMProtectDecryptStringA("14"), 0, 0);
        //aim.fire_py= *(char*)(module_addr + _Rifle_firing_button);

       /* hook_weapon_this = (UINT64)malloc(0x3000);
        memset((void*)hook_weapon_this, 0, 0x3000);

        hook_aim_this = (UINT64)malloc(0x1000);
        memset((void*)hook_aim_this, 0, 0x1000);
        */
        hook_materia_this = (UINT64)stack_hidden_call(malloc,0x1000);
        memset((void*)hook_materia_this, 0, 0x1000);
        

        weapon_table_this = 0;
        aim_table_this = 0;
        materia_table_this = 0;

        esp_draw.matrix = matrix;
        aim.matrix = matrix;
        name_list.clear();

		VMProtectFreeString(TheDivision2_wchar);
        //player.hook_speed_init_2(module_addr);
    }
};



namespace _cheat //功能集
{
    _cheat_data* CreateContext();
    void    DestroyContext(_cheat_data* ctx);


    _cheat_data* get_cheat_data();

    void up_cheat_data();
    void free_cheat_data();

    void run_cheat();


}