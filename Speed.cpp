#include <iostream>

#include <cstring>
#include <typeinfo>
#include <string>
#include <chrono>
#include <vector>
//#include <winbase.h> // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-readdirectorychangesw?redirectedfrom=MSDN
#include <Windows.h>

#include <thread>


// pass the pointer to where we are in memory,
// pass the pointer to where we need to process to
// pass the reference to the vector of where we need to store the values into.
int parallel(char * ptr, char * stop, std::vector<std::string> &myvector) {
	LONGLONG y;
	char arr[330];
	std::fill(arr, arr+330, '\0');
	
	char * pch = NULL;
	while ((pch = std::strchr(ptr,'\n'))){
		if (pch >= stop) break;
		std::fill(arr, arr+330, '\0'); 		// clean out array
		
        (y) = pch - ptr; // size
        
        std::memcpy(arr, ptr, y);
        myvector.push_back(arr);		
        
        // off set for '\n'
		ptr = pch+134;
    }
    return 1;
}


int main(void){    
	const char *filePath="C:\\Users\\riola\\Desktop\\Crimes_-_2001_to_Present.csv";
	//const char *filePath = "C:\\Users\\riola\\Downloads\\500000-Sales-Records\\500000 Sales Records.csv";
	
    HANDLE hFile;
    HANDLE hMap;
    LPVOID lpBasePtr;
    LARGE_INTEGER liFileSize;

    hFile = CreateFile(filePath, 
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
    ((void *)ptr);
    //LONGLONG i = liFileSize.QuadPart;
	//const LONGLONG z = (i-1);
	//LONGLONG y = 0;
	//LONGLONG ziy = 0;
	
	
	//unsigned long int x = 0;
	//char **tst = &ptr; // 
	
	//
	

    std::vector<std::string> myvector;
    // fit to specific data
    // just use i to do calcs
    unsigned int en = 0;
    
    //int max = 0;
    
    // 6992232

	LONGLONG k = 322;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
    //parallel(ptr, ptr+i, myvector);
    //std::thread nThread(parallel, (ptr), ptr+i, myvector);
    while ((pch = std::strchr(ptr,'\n'))){
		 
		std::fill(arr, arr+330, '\0'); 		// clean out array
		
        (y) = pch - ptr; // size
        
        std::memcpy(arr, ptr, y);
        myvector.push_back(arr);		
        
        // off set for '\n'
		ptr = pch+134;
    }
    //nThread.join();

    UnmapViewOfFile(lpBasePtr);
    CloseHandle(hMap);
    CloseHandle(hFile);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	std::cout << "Time difference (sec) = " <<  (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0  <<std::endl;
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mus]" << std::endl;
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

	std::cout << "Finished " << k  << ' ' << myvector.size() << ", EN: " << en << " vs " << liFileSize.QuadPart << ' ' <<  ((float)k/liFileSize.QuadPart)*100 << "% " << (float)liFileSize.QuadPart/k<< std::endl;
	
	
	return 0;
}
/*
 * while ((pch = std::strchr(ptr,'\n'))){
		 
		std::fill(arr, arr+330, '\0'); 		// clean out array
		
        (y) = pch - ptr; // size
        
        std::memcpy(arr, ptr, y);
        myvector.push_back(arr);		
        
        // off set for '\n'
		ptr = pch+134;
    }
 * */
