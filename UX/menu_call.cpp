#include "menu_call.h"
#include "VMProtectSDK.h"



enum _Language_option
{
	//主页
	__Home,
	__Ux,
	__Esp,
	__Player,
	__Npc,
	__Aim,
	__Weapon,
	__Supplies,
	__Install,

	//透视
	__Esp_Player,
	__Esp_npc,
	__enemy,
	__solid,
	__box,
	__bone, __bone1, __bone2, __bone3, __bone4, __bone5, __bone6, __bone7, __bone8, __bone9, __bone10, __bone11, __bone12,
	__grade,
	__distance,
	__name,
	__blood,
	__armor,
	__aim_sign,

	__Esp_aim,

	__supplies_sign,
	__drop_sign,

	//瞄准
	__aim_bone,
	__aim_bone_info,
	__aim_distance,
	__aim_weak,
	__aim_mouse,
	__aim_mouse_message,
	__mouse_length,
	__aim_view,
	__aim_view_message,
	__view_length,
	__aim_magic,
	__aim_magic_san,
	__aim_aoto_fire,
	__aim_aoto_list,
	__aim_aoto_list_if,

	__aim_stop_fire,
	__aim_stop_fire_message,
	__aim_stop_fire_if,
	__aim_switch_target,
	__aim_switch_target_if,

	//武器
	__weapon_read_confg,
	__weapon_read_confg_message,
	__weapon_write_confg,
	__weapon_write_confg_message,
	__deceit_bullets_amount,
	__deceit_bullets_amount_message,
	__lock_bullets_amount,
	__speed_bullets,
	__no_reload_bullets,
	__no_scatter,
	__bullet_value,
	__bullet,
	__aoto_bullets,
	__no_recoil,

	//player_data
	__speed,
	__aim_speed,
	__g_speed,
	__run_global_speed,
	__F_global_speed,
	__X_global_speed,
	__fire_global_speed,

	//materia_data
	__supplies_TP,
	__drop_TP,
	__drop_TP_count,

	//透视 col调制
	__box_col_set,
	__bone_col_set,
	__name_col_set,
	__blood_col_set,
	__enemy_blood_col_set,
	__armor_col_set,
	__aim_armor_col_set,

	__blackname_list_box_col_set,
	__blackname_list_blood_col_set,

	__whitename_list_box_col_set,
	__whitename_list_blood_col_set,

	__center_col_set,
	//设置
	__fps,
	__time_esp,
	__Aiming,
	__esp_rbga,
	__Tab_list,
		__Mouse_list,
	__CapsLock_supplies_TP,
	__Ctrl_drop_TP,
	__Save_Settings,
	__ReadSettings,

	__language
};

//char* _Language_cn[] = {
//	u8"主页"),
//u8"\x9FA1"),
//u8"透视"),
//u8"player_data"),
//u8"人机"),
//u8"瞄准"),
//u8"武器"),
//u8"materia_data"),
//u8"设置"),
//
////透视
//u8"透视_player"),
//u8"透视_人机"),
//u8"enemy"),
//u8"固体"),
//u8"方框"),
//u8"bone"),
//u8"grade"),
//u8"distance"),
//u8"name"),
//u8"生命值"),
//u8"装甲值"),
//u8"自瞄部位"),
//
//u8"透视_自瞄"),
//
//u8"箱子_sign"),
//u8"掉落物_sign"),
//
////瞄准
//u8"distance限制"),
//u8"鼠标_瞄准"),
//u8"使用鼠标\n进行辅助瞄准"),
//u8"鼠标_范围"),
//u8"静默_瞄准"),
//u8"在静默范围之内\n可以子弹穿墙"),
//u8"静默_范围"),
//u8"子弹追踪"),
//u8"顾名思义"),
//u8"识别到enemy开火"),
//u8"自动白name_list"),
//u8"白name_list条件"),
//u8"自动熄火"),
//u8"可以避免伤害丢失"),
//u8"熄火条件"),
//u8"自动切换目标"),
//u8"切换条件"),
//
////武器
//u8"读取武器配置"),
//u8"读取 当前的武器配置\n先读取武器配置\n后修改武器配置"),
//u8"修改武器配置"),
//u8"修改 当前的武器配置\n先读取武器配置\n后修改武器配置"),
//u8"卡无限子弹"),
//u8"卡无限子弹 与 修改武器配置\n不要同时使用,避免闪退"),
//u8"锁子弹"),
//u8"子弹射速"),
//u8"换弹无CD"),
//u8"无散射"),
//u8"伤害倍数"),
//u8"改伤害"),
//u8"全自动"),
//u8"无后座"),
//
////player_data
//u8"mov_speed"),
//u8"射击时mov_speed"),
//u8"全局加速"),
//u8"跑时_启用加速"),
//u8"开火_启用加速"),
//
////materia_data
//u8"吸引箱子"),
//u8"吸引掉落物"),
//
////透视col
//u8"方框"),
//u8"bone"),
//u8"name"),
//u8"生命值"),
//u8"enemy生命值"),
//u8"装甲值"),
//u8"自瞄部位"),
//
//u8"黑name_list_方框"),
//u8"黑name_list_生命值"),
//
//u8"白name_list_方框"),
//u8"白name_list_生命值"),
//u8"准心"),
////设置
//u8"显示准心"),
//u8"透视_col调制"),
//u8"Tab键_Black_Whitename_list"),
//u8"大小写键_吸materia_data"),
//u8"Ctrl键_吸drop_data"),
//u8"保存当前设置"),
//u8"读取当前设置"),
//
//u8"Language"
//};

char* _Language_cn[] = {
(char*)VMProtectDecryptStringA("\xe4\xb8\xbb\xe9\xa1\xb5"),//主页
(char*)VMProtectDecryptStringA("\xe9\xbe\xa1"),//ux
(char*)VMProtectDecryptStringA("\xe9\x80\x8f\xe8\xa7\x86"),//透视
(char*)VMProtectDecryptStringA("\xe4\xba\xba\xe7\x89\xa9"),//人物
(char*)VMProtectDecryptStringA("\xe4\xba\xba\xe6\x9c\xba"),//人机
(char*)VMProtectDecryptStringA("\xe7\x9e\x84\xe5\x87\x86"),//瞄准
(char*)VMProtectDecryptStringA("\xe6\xad\xa6\xe5\x99\xa8"),//武器
(char*)VMProtectDecryptStringA("\xe7\x89\xa9\xe8\xb5\x84"),//物资
(char*)VMProtectDecryptStringA("\xe8\xae\xbe\xe7\xbd\xae"),//设置
(char*)VMProtectDecryptStringA("\xe9\x80\x8f\xe8\xa7\x86\x5f\xe7\x8e\xa9\xe5\xae\xb6"),//透视_玩家
(char*)VMProtectDecryptStringA("\xe9\x80\x8f\xe8\xa7\x86\x5f\xe4\xba\xba\xe6\x9c\xba"),//透视_人机
(char*)VMProtectDecryptStringA("\xe6\x95\x8c\xe4\xba\xba"),//敌人
(char*)VMProtectDecryptStringA("\xe5\x9b\xba\xe4\xbd\x93"),//固体
(char*)VMProtectDecryptStringA("\xe6\x96\xb9\xe6\xa1\x86"),//方框
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc"),//骨骼
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x31"),//骨骼1
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x32"),//骨骼2
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x33"),//骨骼3
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x34"),//骨骼4
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x35"),//骨骼5
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x36"),//骨骼6
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x37"),//骨骼7
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x38"),//骨骼8
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x39"),//骨骼9
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x31\x30"),//骨骼10
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x31\x31"),//骨骼11
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc\x31\x32"),//骨骼12
(char*)VMProtectDecryptStringA("\xe7\xad\x89\xe7\xba\xa7"),//等级
(char*)VMProtectDecryptStringA("\xe8\xb7\x9d\xe7\xa6\xbb"),//距离
(char*)VMProtectDecryptStringA("\xe5\x90\x8d\xe5\xad\x97"),//名字
(char*)VMProtectDecryptStringA("\xe7\x94\x9f\xe5\x91\xbd\xe5\x80\xbc"),//生命值
(char*)VMProtectDecryptStringA("\xe8\xa3\x85\xe7\x94\xb2\xe5\x80\xbc"),//装甲值
(char*)VMProtectDecryptStringA("\xe8\x87\xaa\xe7\x9e\x84\xe9\x83\xa8\xe4\xbd\x8d"),//自瞄部位
(char*)VMProtectDecryptStringA("\xe9\x80\x8f\xe8\xa7\x86\x5f\xe8\x87\xaa\xe7\x9e\x84"),//透视_自瞄
(char*)VMProtectDecryptStringA("\xe7\xae\xb1\xe5\xad\x90\x5f\xe6\xa0\x87\xe8\xaf\x86"),//箱子_标识
(char*)VMProtectDecryptStringA("\xe6\x8e\x89\xe8\x90\xbd\xe7\x89\xa9\x5f\xe6\xa0\x87\xe8\xaf\x86"),//掉落物_标识
(char*)VMProtectDecryptStringA("\xe7\x9e\x84\xe5\x87\x86\x5f\xe9\xaa\xa8\xe9\xaa\xbc"),//瞄准_骨骼
(char*)VMProtectDecryptStringA("\x30\x3a\xe5\xa4\xb4\xe9\x83\xa8\x20\x31\x3a\xe8\xba\xab\xe4\xbd\x93\x20\x32\x3a\xe6\x89\x8b\x20\x33\x3a\xe8\x85\xbf\xa\x34\x3a\xe5\xbc\xb1\xe7\x82\xb9\x31\x20\x35\x3a\xe5\xbc\xb1\xe7\x82\xb9\x32\x20\x36\x3a\xe5\xbc\xb1\xe7\x82\xb9\x33\x20\x37\x3a\xe5\xbc\xb1\xe7\x82\xb9\x34"),//1:头部 2:身体 3:手 4:腿5:弱点1 6 : 弱点2 7 : 弱点3 8 : 弱点4
(char*)VMProtectDecryptStringA("\xe8\xb7\x9d\xe7\xa6\xbb\xe9\x99\x90\xe5\x88\xb6"),//距离限制
(char*)VMProtectDecryptStringA("\xe4\xb8\x8d\xe7\x9e\x84\xe5\x87\x86\xe5\xbc\xb1\xe7\x82\xb9"),//不瞄准弱点

(char*)VMProtectDecryptStringA("\xe9\xbc\xa0\xe6\xa0\x87\x5f\xe7\x9e\x84\xe5\x87\x86"),//鼠标_瞄准
(char*)VMProtectDecryptStringA("\xe4\xbd\xbf\xe7\x94\xa8\xe9\xbc\xa0\xe6\xa0\x87\xa\xe8\xbf\x9b\xe8\xa1\x8c\xe8\xbe\x85\xe5\x8a\xa9\xe7\x9e\x84\xe5\x87\x86"),//使用鼠标\n进行辅助瞄准
(char*)VMProtectDecryptStringA("\xe9\xbc\xa0\xe6\xa0\x87\x5f\xe8\x8c\x83\xe5\x9b\xb4"),//鼠标_范围
(char*)VMProtectDecryptStringA("\xe9\x9d\x99\xe9\xbb\x98\x5f\xe7\x9e\x84\xe5\x87\x86"),//静默_瞄准
(char*)VMProtectDecryptStringA("\xe5\x9c\xa8\xe9\x9d\x99\xe9\xbb\x98\xe8\x8c\x83\xe5\x9b\xb4\xe4\xb9\x8b\xe5\x86\x85\xa\xe5\x8f\xaf\xe4\xbb\xa5\xe5\xad\x90\xe5\xbc\xb9\xe7\xa9\xbf\xe5\xa2\x99"),//在静默范围之内\n可以子弹穿墙
(char*)VMProtectDecryptStringA("\xe9\x9d\x99\xe9\xbb\x98\x5f\xe8\x8c\x83\xe5\x9b\xb4"),//静默_范围
(char*)VMProtectDecryptStringA("\xe5\xad\x90\xe5\xbc\xb9\xe8\xbf\xbd\xe8\xb8\xaa"),//子弹追踪
(char*)VMProtectDecryptStringA("\xe6\x95\xa3\xe5\xb0\x84\xe8\xbf\xbd\xe8\xb8\xaa"),//散射追踪
(char*)VMProtectDecryptStringA("\xe8\xaf\x86\xe5\x88\xab\xe5\x88\xb0\xe6\x95\x8c\xe4\xba\xba\xe5\xbc\x80\xe7\x81\xab"),//识别到敌人开火
(char*)VMProtectDecryptStringA("\xe8\x87\xaa\xe5\x8a\xa8\xe7\x99\xbd\xe5\x90\x8d\xe5\x8d\x95"),//自动白名单
(char*)VMProtectDecryptStringA("\xe7\x99\xbd\xe5\x90\x8d\xe5\x8d\x95\xe6\x9d\xa1\xe4\xbb\xb6"),//白名单条件
(char*)VMProtectDecryptStringA("\xe8\x87\xaa\xe5\x8a\xa8\xe7\x86\x84\xe7\x81\xab"),//自动熄火
(char*)VMProtectDecryptStringA("\xe5\x8f\xaf\xe4\xbb\xa5\xe9\x81\xbf\xe5\x85\x8d\xe4\xbc\xa4\xe5\xae\xb3\xe4\xb8\xa2\xe5\xa4\xb1"),//可以避免伤害丢失
(char*)VMProtectDecryptStringA("\xe7\x86\x84\xe7\x81\xab\xe6\x9d\xa1\xe4\xbb\xb6"),//熄火条件
(char*)VMProtectDecryptStringA("\xe8\x87\xaa\xe5\x8a\xa8\xe5\x88\x87\xe6\x8d\xa2\xe7\x9b\xae\xe6\xa0\x87"),//自动切换目标
(char*)VMProtectDecryptStringA("\xe5\x88\x87\xe6\x8d\xa2\xe6\x9d\xa1\xe4\xbb\xb6"),//切换条件
(char*)VMProtectDecryptStringA("\xe8\xaf\xbb\xe5\x8f\x96\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae"),//读取武器配置
(char*)VMProtectDecryptStringA("\xe8\xaf\xbb\xe5\x8f\x96\x20\xe5\xbd\x93\xe5\x89\x8d\xe7\x9a\x84\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae\xa\xe5\x85\x88\xe8\xaf\xbb\xe5\x8f\x96\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae\xa\xe5\x90\x8e\xe4\xbf\xae\xe6\x94\xb9\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae"),//读取 当前的武器配置\n先读取武器配置\n后修改武器配置
(char*)VMProtectDecryptStringA("\xe4\xbf\xae\xe6\x94\xb9\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae"),//修改武器配置
(char*)VMProtectDecryptStringA("\xe4\xbf\xae\xe6\x94\xb9\x20\xe5\xbd\x93\xe5\x89\x8d\xe7\x9a\x84\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae\xa\xe5\x85\x88\xe8\xaf\xbb\xe5\x8f\x96\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae\xa\xe5\x90\x8e\xe4\xbf\xae\xe6\x94\xb9\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae"),//修改 当前的武器配置\n先读取武器配置\n后修改武器配置
(char*)VMProtectDecryptStringA("\xe5\x8d\xa1\xe6\x97\xa0\xe9\x99\x90\xe5\xad\x90\xe5\xbc\xb9"),//卡无限子弹
(char*)VMProtectDecryptStringA("\xe5\x8d\xa1\xe6\x97\xa0\xe9\x99\x90\xe5\xad\x90\xe5\xbc\xb9\x20\xe4\xb8\x8e\x20\xe4\xbf\xae\xe6\x94\xb9\xe6\xad\xa6\xe5\x99\xa8\xe9\x85\x8d\xe7\xbd\xae\xa\xe4\xb8\x8d\xe8\xa6\x81\xe5\x90\x8c\xe6\x97\xb6\xe4\xbd\xbf\xe7\x94\xa8\x2c\xe9\x81\xbf\xe5\x85\x8d\xe9\x97\xaa\xe9\x80\x80"),//卡无限子弹 与 修改武器配置\n不要同时使用,避免闪退
(char*)VMProtectDecryptStringA("\xe9\x94\x81\xe5\xad\x90\xe5\xbc\xb9"),//锁子弹
(char*)VMProtectDecryptStringA("\xe5\xad\x90\xe5\xbc\xb9\xe5\xb0\x84\xe9\x80\x9f"),//子弹射速
(char*)VMProtectDecryptStringA("\xe6\x8d\xa2\xe5\xbc\xb9\xe6\x97\xa0\x43\x44"),//换弹无CD
(char*)VMProtectDecryptStringA("\xe6\x97\xa0\xe6\x95\xa3\xe5\xb0\x84"),//无散射
(char*)VMProtectDecryptStringA("\xe4\xbc\xa4\xe5\xae\xb3\xe5\x80\x8d\xe6\x95\xb0"),//伤害倍数
(char*)VMProtectDecryptStringA("\xe6\x94\xb9\xe4\xbc\xa4\xe5\xae\xb3"),//改伤害
(char*)VMProtectDecryptStringA("\xe5\x85\xa8\xe8\x87\xaa\xe5\x8a\xa8"),//全自动
(char*)VMProtectDecryptStringA("\xe6\x97\xa0\xe5\x90\x8e\xe5\xba\xa7"),//无后座
(char*)VMProtectDecryptStringA("\xe7\xa7\xbb\xe9\x80\x9f"),//移速
(char*)VMProtectDecryptStringA("\xe5\xb0\x84\xe5\x87\xbb\xe6\x97\xb6\x5f\xe7\xa7\xbb\xe9\x80\x9f"),//射击时_移速
(char*)VMProtectDecryptStringA("\xe5\x85\xa8\xe5\xb1\x80\xe5\x8a\xa0\xe9\x80\x9f"),//全局加速
(char*)VMProtectDecryptStringA("\xe8\xb7\x91\xe6\x97\xb6\x5f\xe5\x90\xaf\xe7\x94\xa8\xe5\x8a\xa0\xe9\x80\x9f"),//跑时_启用加速
(char*)VMProtectDecryptStringA("F\x5f\xe5\x90\xaf\xe7\x94\xa8\xe5\x8a\xa0\xe9\x80\x9f"),//F_启用加速
(char*)VMProtectDecryptStringA("X\x5f\xe5\x90\xaf\xe7\x94\xa8\xe5\x8a\xa0\xe9\x80\x9f"),//X_启用加速
(char*)VMProtectDecryptStringA("\xe5\xbc\x80\xe7\x81\xab\x5f\xe5\x90\xaf\xe7\x94\xa8\xe5\x8a\xa0\xe9\x80\x9f"),//开火_启用加速
(char*)VMProtectDecryptStringA("\xe5\x90\xb8\xe5\xbc\x95\xe7\xae\xb1\xe5\xad\x90"),//吸引箱子
(char*)VMProtectDecryptStringA("\xe5\x90\xb8\xe5\xbc\x95\xe6\x8e\x89\xe8\x90\xbd\xe7\x89\xa9"),//吸引掉落物
(char*)VMProtectDecryptStringA("\xe5\x90\xb8\xe5\xbc\x95\xe6\x8e\x89\xe8\x90\xbd\xe7\x89\xa9\x2d\xe4\xb8\x8a\xe9\x99\x90"),//吸引掉落物-上限
(char*)VMProtectDecryptStringA("\xe6\x96\xb9\xe6\xa1\x86"),//方框
(char*)VMProtectDecryptStringA("\xe9\xaa\xa8\xe9\xaa\xbc"),//骨骼
(char*)VMProtectDecryptStringA("\xe5\x90\x8d\xe5\xad\x97"),//名字
(char*)VMProtectDecryptStringA("\xe7\x94\x9f\xe5\x91\xbd\xe5\x80\xbc"),//生命值
(char*)VMProtectDecryptStringA("\xe6\x95\x8c\xe4\xba\xba\xe7\x94\x9f\xe5\x91\xbd\xe5\x80\xbc"),//敌人生命值
(char*)VMProtectDecryptStringA("\xe8\xa3\x85\xe7\x94\xb2\xe5\x80\xbc"),//装甲值
(char*)VMProtectDecryptStringA("\xe8\x87\xaa\xe7\x9e\x84\xe9\x83\xa8\xe4\xbd\x8d"),//自瞄部位
(char*)VMProtectDecryptStringA("\xe9\xbb\x91\xe5\x90\x8d\xe5\x8d\x95\xe6\x96\xb9\xe6\xa1\x86"),//黑名单方框
(char*)VMProtectDecryptStringA("\xe9\xbb\x91\xe5\x90\x8d\xe5\x8d\x95\xe7\x94\x9f\xe5\x91\xbd\xe5\x80\xbc"),//黑名单生命值
(char*)VMProtectDecryptStringA("\xe7\x99\xbd\xe5\x90\x8d\xe5\x8d\x95\xe6\x96\xb9\xe6\xa1\x86"),//白名单方框
(char*)VMProtectDecryptStringA("\xe7\x99\xbd\xe5\x90\x8d\xe5\x8d\x95\xe7\x94\x9f\xe5\x91\xbd\xe5\x80\xbc"),//白名单生命值
(char*)VMProtectDecryptStringA("\xe5\x87\x86\xe5\xbf\x83"),//准心

(char*)VMProtectDecryptStringA("\xe6\x98\xbe\xe7\xa4\xba\x46\x50\x53\xe5\xb8\xa7\xe7\x8e\x87"),//FPS
(char*)VMProtectDecryptStringA("sleep time"),//FPS
(char*)VMProtectDecryptStringA("\xe6\x98\xbe\xe7\xa4\xba\xe5\x87\x86\xe5\xbf\x83"),//显示准心
(char*)VMProtectDecryptStringA("\xe9\x80\x8f\xe8\xa7\x86\x5f\xe9\xa2\x9c\xe8\x89\xb2\xe8\xb0\x83\xe5\x88\xb6"),//透视_颜色调制
(char*)VMProtectDecryptStringA("Tab\xe9\x94\xae\x5f\xe5\x88\x87\xe6\x8d\xa2\xe5\x90\x8d\xe5\x8d\x95 ~\xe9\x94_out list"),//Tab键_切换名单
(char*)VMProtectDecryptStringA("\xe9\xbc\xa0\xe6\xa0\x87\xe4\xbe\xa7\xe9\x94\xae\xe5\x88\x87\xe6\x8d\xa2\x20\xe6\xbb\x9a\xe8\xbd\xae\xe6\xb6\x88\xe9\x99\xa4"),//鼠标侧键切换 滚轮消除
(char*)VMProtectDecryptStringA("\xe5\xa4\xa7\xe5\xb0\x8f\xe5\x86\x99\xe9\x94\xae\x5f\xe5\x90\xb8\xe7\x89\xa9\xe8\xb5\x84"),//大小写键_吸物资
(char*)VMProtectDecryptStringA("\\ \xe9\x94\xae\x5f\xe5\x90\xb8\xe6\x8e\x89\xe8\x90\xbd\xe7\x89\xa9"),//Ctrl键_吸掉落物
(char*)VMProtectDecryptStringA("\xe4\xbf\x9d\xe5\xad\x98\xe5\xbd\x93\xe5\x89\x8d\xe8\xae\xbe\xe7\xbd\xae"),//保存当前设置
(char*)VMProtectDecryptStringA("\xe8\xaf\xbb\xe5\x8f\x96\xe5\xbd\x93\xe5\x89\x8d\xe8\xae\xbe\xe7\xbd\xae"),//读取当前设置
(char*)VMProtectDecryptStringA("\x4c\x61\x6e\x67\x75\x61\x67\x65")//Language
};
char* _Language_en[] = {
	(char*)VMProtectDecryptStringA("Home"),
(char*)VMProtectDecryptStringA("\xe9\xbe\xa1"),
(char*)VMProtectDecryptStringA("Esp"),
(char*)VMProtectDecryptStringA("Player"),
(char*)VMProtectDecryptStringA("Npc"),
(char*)VMProtectDecryptStringA("Aim"),
(char*)VMProtectDecryptStringA("Weapon"),
(char*)VMProtectDecryptStringA("Supplies"),
(char*)VMProtectDecryptStringA("Settings"),

//透视
(char*)VMProtectDecryptStringA("Esp_Player"),
(char*)VMProtectDecryptStringA("Esp_npc"),
(char*)VMProtectDecryptStringA("enemy"),
(char*)VMProtectDecryptStringA("solid"),
(char*)VMProtectDecryptStringA("box"),
(char*)VMProtectDecryptStringA("bone"),(char*)VMProtectDecryptStringA("bone1"),(char*)VMProtectDecryptStringA("bone2"),(char*)VMProtectDecryptStringA("bone3"),(char*)VMProtectDecryptStringA("bone4"),(char*)VMProtectDecryptStringA("bone5"),(char*)VMProtectDecryptStringA("bone6"),
(char*)VMProtectDecryptStringA("bone7"),(char*)VMProtectDecryptStringA("bone8"),(char*)VMProtectDecryptStringA("bone9"),(char*)VMProtectDecryptStringA("bone10"),(char*)VMProtectDecryptStringA("bone11"),(char*)VMProtectDecryptStringA("bone12"),
(char*)VMProtectDecryptStringA("grade"),
(char*)VMProtectDecryptStringA("distance"),
(char*)VMProtectDecryptStringA("name"),
(char*)VMProtectDecryptStringA("blood"),
(char*)VMProtectDecryptStringA("armor"),
(char*)VMProtectDecryptStringA("aim_sign"),

(char*)VMProtectDecryptStringA("Esp_aim"),

(char*)VMProtectDecryptStringA("supplies_sign"),
(char*)VMProtectDecryptStringA("drop_sign"),

//瞄准
(char*)VMProtectDecryptStringA("aim_bone"),
(char*)VMProtectDecryptStringA("0:Head 1:Body 2:Hand 3:Leg\n4:WeakPoint1 5 : WeakPoint2 6 : WeakPoint3 7 : WeakPoint4"),
(char*)VMProtectDecryptStringA("distance_limit"),
(char*)VMProtectDecryptStringA("No Weak"),
(char*)VMProtectDecryptStringA("mouse"),
(char*)VMProtectDecryptStringA("Use the mouse for assisted aiming"),
(char*)VMProtectDecryptStringA("mouse range"),
(char*)VMProtectDecryptStringA("magic"),
(char*)VMProtectDecryptStringA("Bullets can go through\n walls within silent range"),
(char*)VMProtectDecryptStringA("magic range"),
(char*)VMProtectDecryptStringA("magic no range"),
(char*)VMProtectDecryptStringA("scatter magic"),
(char*)VMProtectDecryptStringA("aoto fire"),
(char*)VMProtectDecryptStringA("aoto whitelist"),
(char*)VMProtectDecryptStringA("aoto whitelist Enable"),
(char*)VMProtectDecryptStringA("aoto stop fire"),
(char*)VMProtectDecryptStringA("Injury loss can be avoided"),
(char*)VMProtectDecryptStringA("aoto stop fire Enable"),
(char*)VMProtectDecryptStringA("random target"),
(char*)VMProtectDecryptStringA("random target Enable"),

//武器
(char*)VMProtectDecryptStringA("Read weapon config"),
(char*)VMProtectDecryptStringA("Read the current weapon configuration \n first read the weapon configuration \n then modify the weapon configuration"),
(char*)VMProtectDecryptStringA("Modify weapon config"),
(char*)VMProtectDecryptStringA("Modify the current weapon configuration \n first read the weapon configuration \n then modify the weapon configuration"),
(char*)VMProtectDecryptStringA("deceit bullets amount"),//欺骗子弹数量
(char*)VMProtectDecryptStringA("weapon config And deceit bullets amount\nDo not use it simultaneously to avoid flickering back"),
(char*)VMProtectDecryptStringA("lock bullets"),
(char*)VMProtectDecryptStringA("speed firi"),//子弹射速
(char*)VMProtectDecryptStringA("no reload bullets"),//换弹无动作
(char*)VMProtectDecryptStringA("no scatter"),//无散射
(char*)VMProtectDecryptStringA("Bullet multiplier"),//倍数
(char*)VMProtectDecryptStringA("Bullet"),//伤害倍数
(char*)VMProtectDecryptStringA("full automatic"),//全自动
(char*)VMProtectDecryptStringA("no recoil"),//无后坐力

//player_data
(char*)VMProtectDecryptStringA("speed"),
(char*)VMProtectDecryptStringA("aim speed"),
(char*)VMProtectDecryptStringA("global speed"),
(char*)VMProtectDecryptStringA("run global speed"),
(char*)VMProtectDecryptStringA("F global speed"),
(char*)VMProtectDecryptStringA("X global speed"),
(char*)VMProtectDecryptStringA("fire global speed"),

//materia_data
(char*)VMProtectDecryptStringA("supplies TP"),
(char*)VMProtectDecryptStringA("drop TP"),
(char*)VMProtectDecryptStringA("drop TP count"),

//透视col
(char*)VMProtectDecryptStringA("box"),
(char*)VMProtectDecryptStringA("bone"),
(char*)VMProtectDecryptStringA("name"),
(char*)VMProtectDecryptStringA("blood"),
(char*)VMProtectDecryptStringA("enemy blood"),
(char*)VMProtectDecryptStringA("armor"),
(char*)VMProtectDecryptStringA("aim sign"),

(char*)VMProtectDecryptStringA("Blacklist box"),
(char*)VMProtectDecryptStringA("Blacklist blood"),

(char*)VMProtectDecryptStringA("Whitelist box"),
(char*)VMProtectDecryptStringA("Whitelist blood"),
(char*)VMProtectDecryptStringA("Aiming"),
//设置
(char*)VMProtectDecryptStringA("FPS"),
(char*)VMProtectDecryptStringA("sleep time"),
(char*)VMProtectDecryptStringA("Aiming"),
(char*)VMProtectDecryptStringA("Esp RBGA"),
(char*)VMProtectDecryptStringA("Tab list ~ out list"),
(char*)VMProtectDecryptStringA("Mouse side list wheel out list"),
(char*)VMProtectDecryptStringA("CapsLock supplies TP"),
(char*)VMProtectDecryptStringA("\ drop TP"),
(char*)VMProtectDecryptStringA("Save config"),
(char*)VMProtectDecryptStringA("read config"),

(char*)VMProtectDecryptStringA("language")
};

char** _Language = _Language_en;
bool _Language_bool = 1;

void esp_list(xs_MenuUi& a);
void Teleport_list(xs_MenuUi& a);
void aim_list(xs_MenuUi& a);
void aim_bone_list(xs_MenuUi& a);
void weapon_list(xs_MenuUi& a);
void player_list(xs_MenuUi& a);
void npc_list(xs_MenuUi& a);
void supplies_list(xs_MenuUi& a);
void model_list(xs_MenuUi& a);
void item_list(xs_MenuUi& a);
void message_list(xs_MenuUi& a);
void install_list(xs_MenuUi& a);


void menu_home(xs_MenuUi& a)
{
	
	if (!_Language_bool)
		_Language = _Language_en;
	else
		_Language = _Language_cn;

	//a.MenuUi_PushInt_ToItme(u8"测试整数-效果"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  0.0f,  0.0f, 1.0f }), &b);
	//a.MenuUi_PushFloat_ToItme(u8"测试浮点数-效果"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  0.0f,  0.0f, 1.0f }), &c);
	//a.MenuUi_PushEnable_ToItme(u8"测试开关-效果"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), 开关);

	a.MenuUi_PushTopName(_Language[__Home]);
	a.MenuUi_PushLowName(_Language[__Ux]);
	a.MenuUi_Pushcall_ToItme(_Language[__Esp], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, esp_list);
	//a.MenuUi_Pushcall_ToItme(u8"Teleport"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), Teleport_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Player], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, player_list);
	//a.MenuUi_Pushcall_ToItme(_Language[__Npc], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), npc_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Aim], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, aim_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Weapon], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, weapon_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Supplies], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, supplies_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Install], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, install_list);
	a.MenuUi_PushEnable_ToItme(_Language[__language], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), (char*)VMProtectDecryptStringA("language"), _Language_bool);

	//_Message* c = &_cheat::get_cheat_data()->message;
	//a.MenuUi_PushEnable_ToItme((char&)"message"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), c->message_enable);

	//a.MenuUi_Pushcall_ToItme(u8"Model"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), model_list);
	//a.MenuUi_Pushcall_ToItme(u8"Item"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), item_list);
	//a.MenuUi_Pushcall_ToItme(u8"Message"), xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), message_list);

	
}

void esp_player_list(xs_MenuUi& a)
{
	
	_esp* c = &_cheat::get_cheat_data()->esp_draw;

	a.MenuUi_PushTopName(_Language[__Esp_Player]);
	a.MenuUi_PushLowName(_Language[__Ux]);

	a.MenuUi_PushEnable_ToItme(_Language[__enemy], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_enemy_enable);

	a.MenuUi_PushEnable_ToItme(_Language[__box], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_box_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__bone], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_bone_enable);
	//a.MenuUi_PushEnable_ToItme(_Language[__grade], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_grade_enable);
	//a.MenuUi_PushEnable_ToItme(_Language[__distance], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_distance_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__name], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_name_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__blood], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_blood_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__armor], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_armor_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__aim_sign], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_aim_sign_enable);
	
}
void esp_npc_list(xs_MenuUi& a)
{
	
	_esp* c = &_cheat::get_cheat_data()->esp_draw;

	a.MenuUi_PushTopName((char*)VMProtectDecryptStringA("Esp_npc"));
	a.MenuUi_PushLowName((char*)VMProtectDecryptStringA("Ux"));

	a.MenuUi_PushEnable_ToItme(_Language[__enemy], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_enemy_enable);

	a.MenuUi_PushEnable_ToItme(_Language[__box], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_box_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__bone], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_bone_enable);
	//a.MenuUi_PushEnable_ToItme(_Language[__grade], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_grade_enable);
	//a.MenuUi_PushEnable_ToItme(_Language[__distance], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_distance_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__name], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_name_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__blood], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_blood_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__armor], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_armor_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__aim_sign], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_aim_sign_enable);
	
}
void esp_aim_list(xs_MenuUi& a)
{
	
	_esp* c = &_cheat::get_cheat_data()->esp_draw;

	a.MenuUi_PushTopName(_Language[__Esp_aim]);
	a.MenuUi_PushLowName(_Language[__Ux]);

	a.MenuUi_PushEnable_ToItme(_Language[__mouse_length], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->aim_mouse_length_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__view_length], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->aim_length_enable);
	
}
void esp_list(xs_MenuUi& a)
{
	
	_esp* c = &_cheat::get_cheat_data()->esp_draw;

	a.MenuUi_PushTopName(_Language[__Esp]);
	a.MenuUi_PushLowName(_Language[__Ux]);
	a.MenuUi_Pushcall_ToItme(_Language[__Player], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, esp_player_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Npc], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, esp_npc_list);
	a.MenuUi_Pushcall_ToItme(_Language[__Aim], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, esp_aim_list);
	//a.MenuUi_PushEnable_ToItme(_Language[__supplies_sign], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->supplies_sign_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__drop_sign], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Drop_sign_enable);


	
}

void Teleport_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName((char*)VMProtectDecryptStringA("Teleport"));
	a.MenuUi_PushLowName((char*)VMProtectDecryptStringA("Ux"));



	
}

void aim_list(xs_MenuUi& a)
{
	
	_aim* c = &_cheat::get_cheat_data()->aim;
	a.MenuUi_PushTopName(_Language[__Aim]);
	a.MenuUi_PushLowName(_Language[__Ux]);

	a.MenuUi_PushFloat_ToItme(_Language[__aim_distance],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_distance);

	a.MenuUi_PushEnable_ToItme(_Language[__Player], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->Player_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__Npc], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->npc_enable);

	a.MenuUi_Pushcall_ToItme(_Language[__aim_bone], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, aim_bone_list);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_weak], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), (char*)VMProtectDecryptStringA("C key"), c->aim_weak_enable);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_mouse], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_mouse_message], c->aim_mouse_enable);
	a.MenuUi_PushFloat_ToItme(_Language[__mouse_length],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_mouse_length);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_view], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), (char*)VMProtectDecryptStringA("F3 key"), c->aim_enable);
	a.MenuUi_PushFloat_ToItme(_Language[__view_length], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), &c->aim_length);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_magic], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), (char*)VMProtectDecryptStringA("F1 key"), c->aim_gun_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__aim_magic_san], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), (char*)VMProtectDecryptStringA("F4 key"), c->aim_san_enable);
	a.MenuUi_PushEnable_ToItme(_Language[__aim_aoto_fire], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), (char*)VMProtectDecryptStringA("F2 key"), c->aim_auto_fire_enable);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_aoto_list], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->aim_aotu_list_enable);
	a.MenuUi_PushInt_ToItme(_Language[__aim_aoto_list_if],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_aotu_list);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_switch_target], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->aim_switch_target_enable);
	a.MenuUi_PushInt_ToItme(_Language[__aim_switch_target_if],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_switch_target);

	a.MenuUi_PushEnable_ToItme(_Language[__aim_stop_fire], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_stop_fire_message], c->aim_stop_fire_enable);
	a.MenuUi_PushInt_ToItme(_Language[__aim_stop_fire_if],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_stop_fire);
	
}

void aim_bone_list(xs_MenuUi& a)
{
	
	_aim* c = &_cheat::get_cheat_data()->aim;
	a.MenuUi_PushTopName(_Language[__aim_bone]);
	a.MenuUi_PushLowName(_Language[__Ux]);
	a.MenuUi_PushInt_ToItme(_Language[__bone1],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[0]);

	a.MenuUi_PushInt_ToItme(_Language[__bone2],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[1]);

	a.MenuUi_PushInt_ToItme(_Language[__bone3],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[2]);

	a.MenuUi_PushInt_ToItme(_Language[__bone4],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[3]);

	a.MenuUi_PushInt_ToItme(_Language[__bone5],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[4]);

	a.MenuUi_PushInt_ToItme(_Language[__bone6],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[5]);

	a.MenuUi_PushInt_ToItme(_Language[__bone7],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[6]);

	a.MenuUi_PushInt_ToItme(_Language[__bone8],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[7]);

	a.MenuUi_PushInt_ToItme(_Language[__bone9],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[8]);

	a.MenuUi_PushInt_ToItme(_Language[__bone10],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[9]);

	a.MenuUi_PushInt_ToItme(_Language[__bone11],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[10]);

	a.MenuUi_PushInt_ToItme(_Language[__bone12],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__aim_bone_info],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->aim_bone[11]);


}

void weapon_list(xs_MenuUi& a)
{
	
	_Weapon* c = &_cheat::get_cheat_data()->weapon;
	a.MenuUi_PushTopName(_Language[__Weapon]);
	a.MenuUi_PushLowName(_Language[__Ux]);


	a.MenuUi_PushInt_ToItme(_Language[__speed_bullets],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->speed_value);
	a.MenuUi_PushEnable_ToItme(_Language[__speed_bullets], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->speed_enable);

	a.MenuUi_PushInt_ToItme(_Language[__bullet_value],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->bullet_value);
	a.MenuUi_PushEnable_ToItme(_Language[__bullet], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->bullet_enable);


	a.MenuUi_PushText_ToItme(_Language[__aoto_bullets], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->weapon_auto_enable, (char*)VMProtectDecryptStringA("Write Weapon auto"));
	a.MenuUi_PushText_ToItme(_Language[__weapon_read_confg], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__weapon_read_confg_message], c->weapon_read_confg_enable, (char*)VMProtectDecryptStringA("read Successfully"));
	a.MenuUi_PushText_ToItme(_Language[__weapon_write_confg], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__weapon_write_confg_message], c->weapon_write_confg_enable, (char*)VMProtectDecryptStringA("write successfully"));


	a.MenuUi_PushEnable_ToItme(_Language[__deceit_bullets_amount], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), _Language[__deceit_bullets_amount_message], c->deceit_bullets_enable);


	
	a.MenuUi_PushEnable_ToItme(_Language[__lock_bullets_amount], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->bullets_lock_enable);
	
	//a.MenuUi_PushEnable_ToItme(_Language[__no_reload_bullets], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->weapon_reload_bullets_enable);
	//a.MenuUi_PushEnable_ToItme(_Language[__no_scatter], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->weapon_scatter_enable);


	a.MenuUi_PushText_ToItme(_Language[__no_recoil], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->weapon_recoil_enable, (char*)VMProtectDecryptStringA("Write Weapon recoil"));
	
}

void player_list(xs_MenuUi& a)
{
	
	_Player* c = &_cheat::get_cheat_data()->player;
	a.MenuUi_PushTopName(_Language[__Player]);
	a.MenuUi_PushLowName(_Language[__Ux]);
	a.MenuUi_PushFloat_ToItme(_Language[__speed], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), &c->speed);
	a.MenuUi_PushFloat_ToItme(_Language[__g_speed], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), &c->g_speed);

	a.MenuUi_PushEnable_ToItme(_Language[__g_speed], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->global_speed_bool);
	a.MenuUi_PushEnable_ToItme(_Language[__run_global_speed], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->shift_g_speed);
	a.MenuUi_PushEnable_ToItme(_Language[__F_global_speed], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->f_g_speed);
	a.MenuUi_PushEnable_ToItme(_Language[__X_global_speed], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->x_g_speed);
	
}

void npc_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName((char*)VMProtectDecryptStringA("Npc"));
	a.MenuUi_PushLowName((char*)VMProtectDecryptStringA("Ux"));



	
}

//世界-materia_data
void supplies_list(xs_MenuUi& a)
{
	
	_supplies* c = &_cheat::get_cheat_data()->supplies;
	a.MenuUi_PushTopName(_Language[__Supplies]);
	a.MenuUi_PushLowName(_Language[__Ux]);


	//a.MenuUi_PushEnable_ToItme(_Language[__supplies_TP], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->supplies_tp_enable, &c->supplies_enable_size);
	a.MenuUi_PushEnable_ToItme(_Language[__drop_TP], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->drop_tp_enable);
	
	a.MenuUi_PushInt_ToItme(_Language[__drop_TP_count],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&c->drop_tp_conut);
}

void model_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName((char*)VMProtectDecryptStringA("Model"));
	a.MenuUi_PushLowName((char*)VMProtectDecryptStringA("Ux"));

	


}

//个人-物品
void item_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName((char*)VMProtectDecryptStringA("Item"));
	a.MenuUi_PushLowName((char*)VMProtectDecryptStringA("Ux"));


	

}

void message_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName((char*)VMProtectDecryptStringA("Message"));
	a.MenuUi_PushLowName((char*)VMProtectDecryptStringA("Ux"));



	
}

void rgba_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName(_Language[__esp_rbga]);
	a.MenuUi_PushLowName(_Language[__Ux]);
	_esp* c = &_cheat::get_cheat_data()->esp_draw;

	a.MenuUi_Pushcolcall_ToItme(_Language[__box_col_set], c->box_rgba, NULL, &c->box_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__bone_col_set], c->bone_rgba, NULL, &c->bone_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__name_col_set], c->name_rgba, NULL, &c->name_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__blood_col_set], c->blood_rgba, NULL, &c->blood_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__enemy_blood_col_set], c->blood_rgba_enemy, NULL, &c->blood_rgba_enemy);
	a.MenuUi_Pushcolcall_ToItme(_Language[__armor_col_set], c->armor_rgba, NULL, &c->armor_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__aim_armor_col_set], c->aim_sign_rgba, NULL, &c->aim_sign_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__center_col_set], c->aiming_rgba, NULL, &c->aiming_rgba);

	a.MenuUi_Pushcolcall_ToItme(_Language[__blackname_list_box_col_set], c->blacklist_box_rgba, NULL, &c->blacklist_box_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__blackname_list_blood_col_set], c->blacklist_blood_rgba, NULL, &c->blacklist_blood_rgba);

	a.MenuUi_Pushcolcall_ToItme(_Language[__whitename_list_box_col_set], c->whitelist_box_rgba, NULL, &c->whitelist_box_rgba);
	a.MenuUi_Pushcolcall_ToItme(_Language[__whitename_list_blood_col_set], c->whitelist_blood_rgba, NULL, &c->whitelist_blood_rgba);
	
}

void install_list(xs_MenuUi& a)
{
	
	a.MenuUi_PushTopName(_Language[__Install]);
	a.MenuUi_PushLowName(_Language[__Ux]);

	a.MenuUi_Pushcall_ToItme(_Language[__esp_rbga], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, rgba_list);
	_other* cc = &_cheat::get_cheat_data()->other;
	a.MenuUi_PushEnable_ToItme(_Language[__fps], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, cc->fps);
	
	a.MenuUi_PushInt_ToItme(_Language[__time_esp],
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL,
		xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }),
		&cc->sleep_time);

	_esp* c = &_cheat::get_cheat_data()->esp_draw;
	a.MenuUi_PushEnable_ToItme(_Language[__Aiming], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, c->aiming_enable);
	a.MenuUi_PushText_ToItme(_Language[__Tab_list], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL);
	_aim* ccc = &_cheat::get_cheat_data()->aim;
	a.MenuUi_PushEnable_ToItme(_Language[__Mouse_list], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, ccc->aim_list_key_enable);
	_supplies* d = &_cheat::get_cheat_data()->supplies;
	//a.MenuUi_PushEnable_ToItme(_Language[__CapsLock_supplies_TP], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, d->supplies_tp_enable_key, &d->supplies_enable_size);
	a.MenuUi_PushEnable_ToItme(_Language[__Ctrl_drop_TP], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, d->drop_tp_enable_key);

	a.MenuUi_PushText_ToItme(_Language[__Save_Settings], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, _cheat::get_cheat_data()->config_write, (char*)VMProtectDecryptStringA("Save Ux Configuration"));
	a.MenuUi_PushText_ToItme(_Language[__ReadSettings], xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 1.0f }), NULL, _cheat::get_cheat_data()->config_read, (char*)VMProtectDecryptStringA("Read Ux Configuration"));
	
}

void window_call(xs_windowUI& a)
{
	
	//设置window -- pos col
	xsGuiIO& io = xs_Gui::GetIO();

	a.pos = xsFloat4(0, 0, io.DisplaySize.x - 100.0F, 700.0F);
	a.col = xs_Gui::col_FloatToUint32_RGBA({ 1.0f,  1.0f,  1.0f, 0.0f });
	a.display_SetkeyValue(0);
	//提组件
	//a.提组件_文本(13.0f,xsFloat2( 0.0f,0.0f), xs_Gui::col_FloatToUint32_RGBA({ 0.0f,  0.0f,  1.0f, 0.8f }), u8"测试文本内容");
	a.pushCompon_Menu(menu_home, xsFloat4(io.DisplaySize.x - 500.0F, 100.0F, io.DisplaySize.x - 100.0F, 700.0F), xs_Gui::col_FloatToUint32_RGBA({ 0.0f,  0.0f,  0.0f, 0.0f }));
	
}