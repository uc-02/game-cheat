// freeyunDll.cpp : 定义 DLL 应用程序的导出函数。
//
// Windows 头文件: 
#pragma once
#include <windows.h>
#include <string>
#include "freeyunDll.h"
#include <Wincrypt.h>
#include "w3c.h"
#include "rc4.h"
#include <nb30.h>
#include <ctime>
#pragma comment (lib,"netapi32.lib")

using namespace std;
void 屏蔽非字母(char* 地址, ULONG 大小)
{
	char* cP = 地址;
	for (; 大小--; cP++)
	{
		if ('-' == *cP || '_' == *cP)
			continue;

		switch (*cP)
		{
		case '0':  break;
		case '1':  break;
		case '2':  break;
		case '3':  break;
		case '4':  break;
		case '5':  break;
		case '6':  break;
		case '7':  break;
		case '8':  break;
		case '9':  break;
		case 'a': case 'A':  break;
		case 'b': case 'B': break;
		case 'c': case 'C': break;
		case 'd': case 'D': break;
		case 'e': case 'E':break;
		case 'f': case 'F':  break;
		case 'g': case 'G':  break;
		case 'h': case 'H':   break;
		case 'i': case 'I':   break;
		case 'j': case 'J':   break;
		case 'k': case 'K': break;
		case 'l': case 'L':   break;
		case 'm': case 'M':   break;
		case 'n': case 'N':   break;
		case 'o': case 'O':   break;
		case 'p': case 'P':   break;
		case 'q': case 'Q':   break;
		case 'r': case 'R':   break;
		case 's': case 'S':   break;
		case 't': case 'T':  break;
		case 'u': case 'U':  break;
		case 'v': case 'V':   break;
		case 'w': case 'W':   break;
		case 'x': case 'X':   break;
		case 'y': case 'Y':  break;
		case 'z': case 'Z':   break;
		default:
			*cP = 0;
			break;
		}
		if (*cP && !*(cP - 1))
			*(cP - 1) = ' ';
	}
}
char* GetDisk(char* cmd, char* 内容)
{

	FILE* file;
	char ptr[1024] = { 0 };
	char tmp[1024] = { 0 };
	strcat_s(ptr, cmd);
	if ((file = _popen(ptr, "r")) != NULL)
	{
		int i = 0;
		fgets(tmp, 1024, file);
		while (fgets(tmp, 1024, file) != NULL)
		{


			屏蔽非字母((char*)&tmp, 20);
			if (tmp[0] != 0)
			{
				memmove((PVOID)((ULONG64)内容 + (20 * i)), tmp, 20);
			}
			i++;
		}
		_pclose(file);
	}
	return tmp;
}
BOOL GetStrMd5(string csToMD5, string &strMd5)  // 获取字符串md5值
{

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //获得CSP中一个密钥容器的句柄
    {
        return FALSE;
    }
    HCRYPTPROV hHash = NULL;
    //初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被    CryptHashData调用。
    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)
    {
        return FALSE;
    }

    if (CryptHashData(hHash, (BYTE*)csToMD5.c_str(), csToMD5.length(), 0) == FALSE)      //hash文件  
    {
        return FALSE;
    }

    BYTE *pbHash;
    DWORD dwHashLen = sizeof(DWORD);
    if (!CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0)) //我也不知道为什么要先这样调用CryptGetHashParam，这块是参照的msdn       
    {
        return FALSE;
    }
    pbHash = (BYTE*)malloc(dwHashLen);
    if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))//获得md5值 
    {
        for (DWORD i = 0; i < dwHashLen; i++)         //输出md5值 
        {
            char str[3] = { 0 };//===========================注意这个数组如果太小会出错
            string strFilePartM = ("");
            sprintf(str, ("%02x"), pbHash[i]);
            strMd5 += str;
        }
    }
    //善后工作
    if (CryptDestroyHash(hHash) == FALSE)          //销毁hash对象  
    {
        return FALSE;
    }
    if (CryptReleaseContext(hProv, 0) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}
int GetMac(char* mac)
{
	NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, * PASTAT;

	ASTAT Adapter;

	typedef struct _LANA_ENUM
	{
		UCHAR   length;
		UCHAR   lana[MAX_LANA];
	}LANA_ENUM;

	LANA_ENUM lana_enum;
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char*)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	uRetCode = Netbios(&ncb);

	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb);
		if (uRetCode == NRC_GOODRET)
			break;
	}

	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char*)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);

	if (uRetCode != NRC_GOODRET)
		return uRetCode;

	sprintf(mac, "%02X%02X%02X%02X%02X%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);
	return 0;
}
#include <time.h>
bool FREEYUN_API Init()
{
	char macbuf[24] = "\0";
	GetMac(macbuf);//获取硬盘ID当机器码

	SYSTEMTIME tmSys;
	GetLocalTime(&tmSys);

	time_t curtime;

	time(&curtime);

	__int64 tmDst = __int64(curtime) * 1000 + tmSys.wMilliseconds;
	string csTime;
	_i64toa(tmDst, (char *)csTime.c_str(), 10);

	string csSendData = "{\"version\":\"1.2\",\"timestamp\":";
	csSendData += csTime.c_str();
	csSendData += ",\"macCode\":\"";
	csSendData += macbuf;
	csSendData += "\",\"secretKey\":\"74A263F83C4D37A732C644D0E9100994\"}";
	int iSendDataLen = csSendData.length();

	string csRc4SendData;
	csRc4SendData = Encrypt(csSendData.c_str(), "Q2sgDa2V");//加密算法RC4

	string csSendDataToMd5, csSendDataMd5;
	csSendDataToMd5 = "1";
	csSendDataToMd5 += csTime.c_str();
	csSendDataToMd5 += "YVGPNElHf9";
	csSendDataToMd5 += "23348";
	csSendDataToMd5 += csRc4SendData.c_str();
	GetStrMd5(csSendDataToMd5, csSendDataMd5);
	//MessageBox(NULL, csSendDataMd5.c_str(), 0, 0);

	W3Client client;
	char buf[1024] = "\0";
	client.Connect("https://api.freeyun.net");
	//client.AddPostArgument("version", "1.2");
	client.AddPostArgument("appid", "23348");
	client.AddPostArgument("wtype", "1");
	client.AddPostArgument("timestamp", csTime.c_str());
	client.AddPostArgument("data", csRc4SendData.c_str());
	client.AddPostArgument("sign", csSendDataMd5.c_str());
	
	if (client.Request("/webgateway.html", W3Client::reqPost))
	{
		while (client.Response(reinterpret_cast<unsigned char*>(buf), 1024) > 0)
		{
			//OutputDebugString(buf);
		}
	}
	string _返回值 = buf;
	//MessageBox(NULL, buf, 0, 0);
	char data[1024] = { 0 };
	memmove(data,&buf[_返回值.find("\"data\":\"", 0)+8], _返回值.find("\",\"status\":", 0)- _返回值.find("\"data\":\"", 0)-8);


	string outStr = Decrypt(data, "Q2sgDa2V");
	
	if (outStr.find("\"code\":1003,", 0) < _返回值.length())
		return 1;
	else
		return 0;
}

int FREEYUN_API UserLogin(char* user1)//单吗登陆
{
	char macbuf[24] = "\0";
	GetDisk((char*)"wmic diskdrive get serialnumber", macbuf);//获取硬盘ID当机器码

	SYSTEMTIME tmSys;
	GetLocalTime(&tmSys);
	time_t curtime;

	time(&curtime);

	__int64 tmDst = __int64(curtime) * 1000 + tmSys.wMilliseconds;
	string csTime;
	_i64toa(tmDst, (char *)csTime.c_str(), 10);

	string user = user1;
	string exeMd5 = "1.2";//

	string exeMd5oUT = "";
	GetStrMd5(exeMd5, exeMd5oUT);

	string csSendData = "{\"account\":\"";//卡密
	csSendData += user.c_str();
	csSendData += "\",\"version\":\"1.2\",";//版本号
	csSendData += "\"md5\":\"";//软件的MD5
	csSendData += exeMd5oUT.c_str();

	csSendData += "\",\"macCode\":\"";//机器码
	csSendData += macbuf;
	csSendData += "\",\"timestamp\":\"";//客户端时间戳
	csSendData += csTime.c_str();
	csSendData += "\",\"secretKey\":\"74A263F83C4D37A732C644D0E9100994\"";//软件密钥


	csSendData += "}";
	int iSendDataLen = csSendData.length();

	char *buf2;
	buf2 = Encrypt(csSendData.c_str(), "Q2sgDa2V");//加密算法RC4
	char *buf3 = new char[iSendDataLen * 2 + 1];
	memset(buf3, 0, iSendDataLen * 2 + 1);
	memcpy(buf3, buf2, iSendDataLen * 2 + 1);
	string csRc4SendData(buf2);
	int length = csRc4SendData.length();


	string csSendDataToMd5, csSendDataMd5 = "";
	csSendDataToMd5 = "19"; //业务类型
	csSendDataToMd5 += csTime.c_str(); //时间戳
	csSendDataToMd5 += "YVGPNElHf9";//签名算法
	csSendDataToMd5 += "23348";
	csSendDataToMd5 += csRc4SendData.c_str();//Data 加密后的密文

	//sign请求封包签名（算法为：Md5(wtype + timestamp + 私盐 + appid + data)）
	GetStrMd5(csSendDataToMd5, csSendDataMd5);//这里计算


	//软件ID：8288
	//软件密钥：B1CFCE311C7C1D76901141BE5B3F25D5
	//	加密算法：RC4
	//	加密Key：Bjh7DbMe
	//	签名盐：Rr4FjqK2gy
	W3Client client;
	char buf[1024] = "\0";
	client.Connect("https://api.freeyun.net");
	client.AddPostArgument("appid", "23348");//软件编号
	client.AddPostArgument("wtype", "19");//业务类型
	client.AddPostArgument("timestamp", csTime.c_str());//时间戳
	client.AddPostArgument("data", csRc4SendData.c_str());//加密后的密文
	client.AddPostArgument("sign", csSendDataMd5.c_str());//签名算法

	if (client.Request("/webgateway.html", W3Client::reqPost))
	{
		while (client.Response(reinterpret_cast<unsigned char*>(buf), 1024) > 0)
		{
		}
	}

	string _返回值 = buf;
	char data[1024] = { 0 };
	memmove(data, &buf[_返回值.find("\"data\":\"", 0) + 8], _返回值.find("\",\"status\":", 0) - _返回值.find("\"data\":\"", 0) - 8);

	string outStr = Decrypt(data, "Q2sgDa2V");


	//-108、表示data加密前的参数校验有误
	//	- 109、表示服务器异常或繁忙、可重新获取一次
	//	以下为可自定义的值
	//	1001、软件不存在或密钥错误
	//	1010、机器码有误或在非绑定电脑登陆
	//	1007、软件已经关闭验证
	//	1027、充值卡不存在
	//	1040、用户被锁定
	//	1014、登陆成功
	//	1011、账号过期
	//	1012、账号点数不足
	//	1013、软件使用人数已达多开上限
	int 回值 = 0;
	if (outStr.find("\"code\":1014,", 0) < outStr.length())
		回值 = 1014;
	else if (outStr.find("\"code\":1010,", 0) < outStr.length())
		回值 = 1010;
	else if (outStr.find("\"code\":-109,", 0) < outStr.length())
		回值 = -109;
	else if (outStr.find("\"code\":1027,", 0) < outStr.length())
		回值 = 1027;
	else if (outStr.find("\"code\":1011,", 0) < outStr.length())
		回值 = 1011;
	else if (outStr.find("\"code\":1040,", 0) < outStr.length())
		回值 = 1040;
	return 回值;
}
