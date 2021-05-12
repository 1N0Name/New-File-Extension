#include <algorithm>
#include <iostream>
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

int main(int argc, char* argv[])
{
	std::string fileDestination	= argv[0];
	std::string rootDirectory	= fileDestination.substr(0, fileDestination.find_last_of("\\"));
	const std::string GUID		= "{01b25495-d2f0-4568-a708-911d380db1be}";

	/// AddFileType
	SetKeyValue(CreateNewKey(HKCR, ".abc"), "", "abcfile");
	SetKeyValue(CreateNewKey(HKCR, ".abc\\ShellNew"), "FileName", 
		rootDirectory + "\\..\\resources\\BadCloud.png");

	/// AddCLSID
	CreateNewKey(HKCR, "CLSID\\{01b25495-d2f0-4568-a708-911d380db1be}");

	/// AddFileInfo
	SetKeyValue(CreateNewKey(HKCR, "abcfile"), "", "ABC File Type");
	SetKeyValue(CreateNewKey(HKCR, "abcfile\\DefaultIcon"), "",
		rootDirectory + "\\..\\resources\\cloud.ico");
	SetKeyValue(CreateNewKey(HKCR, "abcfile\\shell\\open\\command"), "",
		"\"C:\\Windows\\system32\\mspaint.exe\" \"%1\"");
	SetKeyValue(CreateNewKey(HKCR, "abcfile\\shellex\\ContextMenuHandlers"), "",
		"{01b25495-d2f0-4568-a708-911d380db1be}");

	/// AddContextMenu
	SetKeyValue(CreateNewKey(HKCR, "*\\shellex\\ContextMenuHandlers\\abc"), "", GUID);

	std::cin.get();
	return 0;
}

HKEY CreateNewKey(HKEY root, const std::string& subDirectory) {
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

void SetKeyValue(HKEY hRegistryKey, const std::string& valueName , const std::string& data) {
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
		std::cout << "[Success] Value: \"" << valueName << "" << data << "\" was successfully set." << std::endl;
	else
		std::cout << "[Error] Error appeared while setting value." << std::endl;
#endif
}