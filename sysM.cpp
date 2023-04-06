#include <iostream>
#include <Windows.h>
#include <psapi.h>

int main() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    while (true) {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;

        if (GlobalMemoryStatusEx(&memInfo)) {
            std::cout << "Total Physical Memory: " << memInfo.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
            std::cout << "Available Physical Memory: " << memInfo.ullAvailPhys / (1024 * 1024) << " MB" << std::endl;
        }

        FILETIME idleTime, kernelTime, userTime;
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            ULARGE_INTEGER idle, kernel, user;
            idle.LowPart = idleTime.dwLowDateTime;
            idle.HighPart = idleTime.dwHighDateTime;
            kernel.LowPart = kernelTime.dwLowDateTime;
            kernel.HighPart = kernelTime.dwHighDateTime;
            user.LowPart = userTime.dwLowDateTime;
            user.HighPart = userTime.dwHighDateTime;

            ULONGLONG idleTicks = idle.QuadPart;
            ULONGLONG totalTicks = (kernel.QuadPart + user.QuadPart);

            static ULONGLONG prevTotalTicks = 0;
            static ULONGLONG prevIdleTicks = 0;
            ULONGLONG totalTicksSinceLastTime = totalTicks - prevTotalTicks;
            ULONGLONG idleTicksSinceLastTime = idleTicks - prevIdleTicks;
            double usage = 1.0 - ((double)idleTicksSinceLastTime) / ((double)totalTicksSinceLastTime);
            std::cout << "CPU Usage: " << usage * 100.0 << "%" << std::endl;

            prevTotalTicks = totalTicks;
            prevIdleTicks = idleTicks;
        }

        ULARGE_INTEGER totalSize, freeSize, availSize;
        if (GetDiskFreeSpaceEx(NULL, &availSize, &totalSize, &freeSize)) {
            std::cout << "Total Disk Space: " << totalSize.QuadPart / (1024 * 1024 * 1024) << " GB" << std::endl;
            std::cout << "Free Disk Space: " << freeSize.QuadPart / (1024 * 1024 * 1024) << " GB" << std::endl;
        }

        std::cout << std::endl;
        Sleep(1000);
    }

    return 0;
}
