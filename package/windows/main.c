#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <Msi.h>
#include <WinInet.h>
#include <string.h>

#pragma comment ( lib, "Wininet.lib" )
#pragma comment ( lib, "Msi.lib" )
#pragma comment ( lib, "Wininet.lib" )

/*don't ask where to find these guids. just feel the force*/
#define vc2010x86Code			"{196BB40D-1578-3D01-B289-BEFC77A11A1E}"
#define vc2010x64Code			"{DA5E371C-6333-3D8A-93A4-6FD5B20BCC6E}"
#define vc2010SP1x86Code	"{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}"
#define vc2010SP1x64Code	"{1D8E6291-B0D5-35EC-8441-6616F567A0F7}"
#define vc2013x86Code			"{13A4EE12-23EA-3371-91EE-EFB36DDFFF3E}" 
#define vc2013x64Code			"{ABB19BB4-838D-3082-BDA4-87C6604181A2}" 
#define vc2013x64CodeAdd	"{20C1086D-C843-36B1-B678-990089D1BD44}"
#define vc2015x86Code			"{BBF2AC74-720C-3CB3-8291-5E34039232FA}"
#define vc2015x64Code			"{EF1EC6A9-17DE-3DA9-B040-686A1E8A8B04}"
/////////////////////////////////////////////////////////////////

static const char* subutai_launcher_exe_str = "SubutaiLauncher.exe";

BOOL check_product_using_msi_query(const LPCTSTR psz_product);
BOOL download_file(const LPCTSTR url, const LPCTSTR dst_path);
BOOL run_process(char* app_path, uint32_t wait);

int
main(int argc, char** argv) {
	(void)argc;
	enum {
		COL_GUID = 0,
		COL_SHORT_NAME = 1,
		COL_URL = 2,
		COL_DST_NAME = 3,
		COL_COUNT
	};

	static const char* table_to_check[] = {
		/*vc2013x64Code, "vc2013x64", 
		"https://cdn.subut.ai:8338/kurjun/rest/raw/download?id=96b61b8e069832e6b809f24ea74567ba&token=null", "vc_redist_x64_2013.exe",*/
		
		vc2013x86Code, "vc2013x86", 
		"https://cdn.subut.ai:8338/kurjun/rest/raw/download?id=0fc525b6b7b96a87523daa7a0013c69d&token=null", "vc_redist_x86_2013.exe",
		
		vc2015x86Code, "vc2015x86", 
		"https://cdn.subut.ai:8338/kurjun/rest/raw/download?id=1a15e6606bac9647e7ad3caa543377cf&token=null", "vc_redist_x86_2015.exe",
		
		NULL, NULL, NULL, NULL
	};
	
	DWORD i, pi_buff_len; //product info buffer len
	enum { PI_BUFF_LEN = 128 };	
	char pi_buff[PI_BUFF_LEN] = { 0 };
	uint32_t start_path_len = strlen(argv[0]);
	char* dst_file_buffer = NULL;
	char* subutai_launcher_exe_buffer = NULL;

	while (argv[0][start_path_len] != '/' && argv[0][start_path_len] != '\\')
		--start_path_len;
	++start_path_len;

	for (i = 0; table_to_check[i * COL_COUNT]; ++i) {
		if (check_product_using_msi_query(table_to_check[i * COL_COUNT])) {
			pi_buff_len = PI_BUFF_LEN;
			uint32_t err = MsiGetProductInfo(table_to_check[i * COL_COUNT], INSTALLPROPERTY_INSTALLEDPRODUCTNAME, pi_buff, &pi_buff_len);
			if (err == ERROR_SUCCESS) {
				printf("Product name : %.*s\r\n", pi_buff_len, pi_buff);
			} else {
				printf("Can't get info. Err : %d, LastErr : %d\r\n", err, GetLastError());
			}
			printf("FOUND!!!!!! %s\r\n", table_to_check[i * COL_COUNT + COL_SHORT_NAME]);
			continue;
		}

		printf("Missing %s\r\n", table_to_check[i * COL_COUNT + COL_SHORT_NAME]);
		dst_file_buffer = (char*)malloc(start_path_len + strlen(table_to_check[i * COL_COUNT + COL_DST_NAME]) + 1);
		memset(dst_file_buffer, 0, start_path_len + strlen(table_to_check[i * COL_COUNT + COL_DST_NAME]) + 1);
		memcpy(dst_file_buffer, argv[0], start_path_len);
		memcpy(&dst_file_buffer[start_path_len], 
			table_to_check[i * COL_COUNT + COL_DST_NAME], 
			strlen(table_to_check[i * COL_COUNT + COL_DST_NAME]));
		
		if (download_file(table_to_check[i * COL_COUNT + COL_URL], dst_file_buffer)) 
			run_process(dst_file_buffer, TRUE);		
		
		free(dst_file_buffer);
	} 

	subutai_launcher_exe_buffer = (char*)malloc(start_path_len + strlen(subutai_launcher_exe_str) + 1);
	memset(subutai_launcher_exe_buffer, 0, start_path_len + strlen(subutai_launcher_exe_str) + 1);
	memcpy(subutai_launcher_exe_buffer, argv[0], start_path_len);
	memcpy(&subutai_launcher_exe_buffer[start_path_len], subutai_launcher_exe_str, strlen(subutai_launcher_exe_str));
	run_process(subutai_launcher_exe_buffer, FALSE);

	system("pause");//delete it
	return 0;
}
/////////////////////////////////////////////////////////////////

BOOL
check_product_using_msi_query(const LPCTSTR psz_product) {
	INSTALLSTATE ir = INSTALLSTATE_UNKNOWN;
	// Check input parameter
	if (!psz_product)
		return FALSE;

	ir = MsiQueryProductState(psz_product);
	return ir == INSTALLSTATE_DEFAULT;
}
/////////////////////////////////////////////////////////////////

BOOL 
download_file(const LPCTSTR url, const LPCTSTR dst_path) {
	enum { BUFF_SIZE = 1480 * 4 }; //MTU * 4
	uint8_t buff[BUFF_SIZE] = { 0 };
	HINTERNET h_session;
	BOOL result = TRUE;
	HANDLE dst_fd;
	

	if (!url) return FALSE;
	if (!dst_path) return FALSE;
	dst_fd = CreateFile(dst_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (dst_fd == INVALID_HANDLE_VALUE) {
		printf("Can't create destination file. Last err : %d\r\n", GetLastError());
		return FALSE;
	}
	
	h_session = InternetOpen(TEXT("launcher4launcher"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!h_session) {
		printf("Couldn't open internet session. Last err : %d\r\n", GetLastError());
		CloseHandle(dst_fd);
		return FALSE;
	}

	do {
		DWORD nrecv = 0;
		DWORD error = ERROR_SUCCESS;
		HINTERNET h_if = InternetOpenUrl(h_session, url, NULL, 0, 0, 0);
		uint32_t percent = 0;
		uint32_t total_written = 0;
		DWORD nwritten = 0;
		DWORD file_size = 0;

		if (!h_if) {
			printf("Couldn't open url. Last err : %d\r\n", GetLastError());
			result = FALSE;
			break;
		}

		file_size = InternetSetFilePointer(h_if, 0, 0, FILE_END, 0);
		printf("Got file size : %d\r\n", file_size);
		InternetSetFilePointer(h_if, 0, 0, FILE_BEGIN, 0);
		percent = (total_written * 100 / file_size);

		do {
			if (!InternetReadFile(h_if, buff, BUFF_SIZE, &nrecv)) {
				printf("\r\nRead file failed. Last err : %d\r\n", GetLastError());
				result = FALSE;
				break;
			}

			if (!WriteFile(dst_fd, buff, nrecv, &nwritten, NULL)) {
				printf("\r\nWrite to file failed. Last err : %d\r\n", GetLastError());
				result = FALSE;
				break;
			}
			total_written += nwritten;
			
			if (percent != (total_written * 100 / file_size)) {
				percent = (total_written * 100 / file_size);
				printf("Download progress : %d%%\r\n", percent);
			}
		} while ((error == ERROR_SUCCESS) && (nrecv > 0));
		InternetCloseHandle(h_if);
	} while (0);

	InternetCloseHandle(h_session);
	CloseHandle(dst_fd);
	return result;
}
/////////////////////////////////////////////////////////////////

BOOL
run_process(char* app_path, uint32_t wait) {
	PROCESS_INFORMATION proc_info = { 0 };
	STARTUPINFO startup_info = { 0 };
	BOOL sr = FALSE;
	startup_info.cb = sizeof(startup_info);
	sr = CreateProcess(NULL, app_path, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info, &proc_info);

	if (!sr) {
		printf("Can't create process. Error : %d\r\n", GetLastError());
	} else {
		if (wait) WaitForSingleObject(proc_info.hProcess, 1000 * 60 * 30);
	}
	return sr;
}