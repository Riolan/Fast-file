#include <iostream>

#include <cstring>
#include <typeinfo>
#include <string>
#include <chrono>
#include <vector>
#include <winbase.h> // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-readdirectorychangesw?redirectedfrom=MSDN
#include <Windows.h>

int main(void){    
	
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	std::string filePath = "C:\\Users\\riola\\Desktop\\Crimes_-_2001_to_Present.csv";
	size_t size = filePath.length();

	TCHAR* wArr = new TCHAR[size+1];
	for (size_t i = 0; i < size; ++i)
		wArr[i] = (filePath[i]);
	wArr[size] = 0;

	TCHAR *lpFileName = TEXT(wArr);
    HANDLE hFile;
    HANDLE hMap;
    LPVOID lpBasePtr;
    LARGE_INTEGER liFileSize;

    hFile = CreateFile(lpFileName, 
        GENERIC_READ,                          // dwDesiredAccess
        0,                                     // dwShareMode
        NULL,                                  // lpSecurityAttributes
        OPEN_EXISTING,                         // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL,                 // dwFlagsAndAttributes
        0);                                    // hTemplateFile
    if (hFile == INVALID_HANDLE_VALUE) {
        return 1;
    }

    if (!GetFileSizeEx(hFile, &liFileSize)) {
        CloseHandle(hFile);
        return 1;
    }

    if (liFileSize.QuadPart == 0) {
        fprintf(stderr, "File is empty\n");
        CloseHandle(hFile);
        return 1;
    }

    hMap = CreateFileMapping(
        hFile,
        NULL,                          // Mapping attributes
        PAGE_READONLY,                 // Protection flags
        0,                             // MaximumSizeHigh
        0,                             // MaximumSizeLow
        NULL);                         // Name
    if (hMap == 0) {
        CloseHandle(hFile);
        return 1;
    }

    lpBasePtr = MapViewOfFile(
        hMap,
        FILE_MAP_READ,         // dwDesiredAccess
        0,                     // dwFileOffsetHigh
        0,                     // dwFileOffsetLow
        0);                    // dwNumberOfBytesToMap
    if (lpBasePtr == NULL) {
        CloseHandle(hMap);
        CloseHandle(hFile);
        return 1;
    }

    char *ptr = ((char *)lpBasePtr);
    LONGLONG i = liFileSize.QuadPart;

	unsigned long int x = 0;
	char **tst = &ptr; // 
	signed long int y = 0;
	//
	char arr[1000];
	//std::fill(arr, arr+1000, '\0');
    std::vector<std::string> myvector;
    while (i-- > 0) {
		if (*ptr != '\n') {
			// update value holding largest pointer
			(tst) = (&(ptr))+x;
		} else {			
			// need to ensure that memcopy is aligned, fun improvements 
			// https://www.embedded.com/optimizing-memcpy-improves-speed
			std::memcpy(arr, ptr-x+y, x-y);
			// seems wasteful (?)
			arr[x-y] = '\0';
			//
			myvector.push_back(arr);
			y=x;
		} 
		x++;
		(*ptr++);
		
    }
	
    UnmapViewOfFile(lpBasePtr);
    CloseHandle(hMap);
    CloseHandle(hFile);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	std::cout << "Time difference (sec) = " <<  (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0  <<std::endl;
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mus]" << std::endl;
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

	std::cout << "Finished " << myvector.size() << std::endl;
	
	for (auto it = myvector.begin(); it != myvector.end(); ++it) {
        std::cout << ' ' << *it << std::endl;
        std::cin.ignore();
    }
	
	
	return 0;
}
