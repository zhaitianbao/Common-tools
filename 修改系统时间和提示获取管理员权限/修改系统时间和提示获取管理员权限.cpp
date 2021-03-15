#include<windows.h>
#include<iostream>
#include<string>
#pragma warning(disable:4996)
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" )
int set_local_time(const char *time_string)
{
	SYSTEMTIME system_time = { 0 };
	char year[4 + 1] = { 0 };
	char month[2 + 1] = { 0 };
	char day[2 + 1] = { 0 };
	char hour[2 + 1] = { 0 };
	char minute[2 + 1] = { 0 };
	char second[2 + 1] = { 0 };
	int index = 0;

	strncpy(year, time_string + index, 4);
	index += 4;
	strncpy(month, time_string + index, 2);
	index += 2;
	strncpy(day, time_string + index, 2);
	index += 2;
	strncpy(hour, time_string + index, 2);
	index += 2;
	strncpy(minute, time_string + index, 2);
	index += 2;
	strncpy(second, time_string + index, 2);
	index += 2;

	GetLocalTime(&system_time);

	system_time.wYear = atoi(year);
	system_time.wMonth = atoi(month);
	system_time.wDay = atoi(day);
	system_time.wHour = atoi(hour);
	system_time.wMinute = atoi(minute);
	system_time.wSecond = atoi(second);

	if (true == SetLocalTime(&system_time))
	{
		printf("true");
		return -1;
	}
	printf("false");
	return 0;
}



void GainAdminPrivileges(const char* strApp, UINT idd) {
	std::string  strCmd = "/adminoption 1";
	SHELLEXECUTEINFO execinfo;
	memset(&execinfo, 0, sizeof(execinfo));
	execinfo.lpFile = strApp;
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = "runas";
	execinfo.fMask = SEE_MASK_NO_CONSOLE;
	execinfo.nShow = SW_SHOWDEFAULT;
	execinfo.lpParameters = "/adminoption 1";
	ShellExecuteEx(&execinfo);
}

int RunAsAdmin(LPCSTR Path, LPCSTR Param, LPCSTR Dir, int Showcmd)
{
	HINSTANCE res;
	res = ShellExecute(NULL, "runas", Path, Param, Dir, Showcmd);
	return (int)res;
}


bool IsProcessRunAsAdmin()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	BOOL b = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup);
	if (b)
	{
		CheckTokenMembership(NULL, AdministratorsGroup, &b);
		FreeSid(AdministratorsGroup);
	}
	return b == TRUE;
}
bool GetAdmin(LPCSTR Param, int Showcmd)
{
	if (IsProcessRunAsAdmin())
	{
		printf("================================已经有管理员权限！\n");
		return false; 
	}
	else
	{
		printf("================================没有管理员权限！\n");
	}
		
	TCHAR Path[MAX_PATH];
	ZeroMemory(Path, MAX_PATH);
	::GetModuleFileName(NULL, Path, MAX_PATH);           //获取程序路径
	HINSTANCE res;
	res = ShellExecute(NULL, "runas", Path, Param, NULL, SW_HIDE);
	if ((int)res > 32)
		return true;
	else
		return false;
}

int main()
{
	//GainAdminPrivileges(_pgmptr, 1);
	if (GetAdmin("",1) == true)
	{
		std::cout <<"chage successful"  << std::endl;
	}
	else
	{
		std::cout << "chage false" << std::endl;
	}
	/*set_local_time("20210312141000");

	set_local_time("20200312141000");*/
	/*system("time 14:08:20");
	system("date 2021-03-12");*/
	set_local_time("20160312131000");
	system("pause");
	return 0;
}