#define  _WIN32_WINNT  0x0600

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <windows.h>

#define DEBUG 1

#define HKCR    HKEY_CLASSES_ROOT
#define HKCU    HKEY_CURRENT_USER
#define HKLM    HKEY_LOCAL_MACHINE
#define HKU     HKEY_USERS
#define HKCC    HKEY_CURRENT_CONFIG

HKEY CreateNewKey(HKEY root, const std::string& subDirectory);
void SetKeyValue(HKEY hRegistryKey, const std::string& valueName, const std::string& data);
void DeleteKey(HKEY root, const std::string& subDirectory);
void DeleteTree(HKEY root, const std::string& subDirectory);

int main(int argc, char* argv[])
{
	DeleteKey(HKCR, "*\\shellex\\ContextMenuHandlers\\abc");
    DeleteKey(HKCR, "CLSID\\{01b25495-d2f0-4568-a708-911d380db1be}");
    DeleteTree(HKCR, "abcfile");
    DeleteTree(HKCR, ".abc");

	std::cin.get();
	return 0;
}

HKEY CreateNewKey(HKEY root, const std::string& subDirectory)
{
	HKEY typeLocation = {};
	DWORD status = {};

	RegCreateKeyExA(
		root,
		const_cast<char *>(subDirectory.c_str()),
		NULL,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&typeLocation,
		&status
	);

#if DEBUG == 1
	std::cout << std::endl;
	if (status == REG_CREATED_NEW_KEY)
		std::cout << "[Warn] Location \"" << subDirectory << "\" successfully created." << std::endl;
	else
		std::cout << "[Info] Location \"" << subDirectory << "\" successfully opened." << std::endl;
#endif

	return typeLocation;
}

void SetKeyValue(HKEY hRegistryKey, const std::string& valueName , const std::string& data)
{
	auto status = RegSetValueExA(
		hRegistryKey,
		const_cast<char *>(valueName.c_str()),
		NULL,
		REG_SZ,
		(LPBYTE)const_cast<char *>(data.c_str()),
		data.size() + 1
	);

#if DEBUG == 1
	if (status == ERROR_SUCCESS)
		std::cout << "[Success] Value \"" << valueName << ": " << data << "\" was successfully set." << std::endl;
	else
		std::cout << "[Error] Error appeared while setting value." << std::endl;
#endif
}

void DeleteKey(HKEY root, const std::string& subDirectory)
{
	auto status = RegDeleteKeyExA(
		root,
		const_cast<char *>(subDirectory.c_str()),
		KEY_WOW64_64KEY,
		NULL
	);

#if DEBUG == 1
	if (status == ERROR_SUCCESS)
		std::cout << "[Success] Key \"" << subDirectory << "\" was successfully deleted." << std::endl;
	else
		std::cout << "[Error] Error appeared while deleting key." << std::endl;
#endif
}

void DeleteTree(HKEY root, const std::string& subDirectory)
{
	auto status = RegDeleteTreeA(
		root,
		const_cast<char *>(subDirectory.c_str())
	);

#if DEBUG == 1
	if (status == ERROR_SUCCESS)
		std::cout << "[Success] Key \"" << subDirectory << "\" was successfully deleted." << std::endl;
	else
		std::cout << "[Error] Error appeared while deleting key." << std::endl;
#endif
}