#ifndef PROCESS_H
#define PROCESS_H
#include <Windows.h>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <time.h>
#include <iostream>
#include <tlhelp32.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <Psapi.h>
#include <pdh.h>
using namespace std;
class Process
{
private:
    DWORD processId;    //
    HANDLE hProcess;    //
    string processName; //
    bool running;       //
    string processState;
    SIZE_T pageFaults;
    SIZE_T commitSize;
    SIZE_T peakWorkingSet;
    SIZE_T workingSet;
    SIZE_T pagedPool;
    SIZE_T NPpool;
    int processExecutionTime; //
    DWORD processPriority;  //
    string Spriority;
   // time_t start;
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;
     std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  //  std::chrono::time_point start = ;

public:


    Process(const char* path) {
        createProc(path);
    }

    ~Process() {
        CloseHandle(hProcess);
    }

    void createProc(const char* path){

        if (CreateProcessA(
                NULL,
                LPSTR(path),
                NULL,
                NULL,
                FALSE,
                CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS,
                NULL,
                NULL,
                &si,
                &pi
                )) {
         //   std::thread clockThread(StartTime());
        //    clockThread.detach();

           start_time = std::chrono::high_resolution_clock::now();
            //time(&start);

            processId = pi.dwProcessId;

            hProcess = pi.hProcess;

            running = true;

            processPriority = GetPriorityClass(hProcess);

            if (hProcess != NULL) {
                char buffer[MAX_PATH];
                GetProcessImageFileNameA(hProcess, buffer, MAX_PATH);
                processName = string(buffer);
                string a = processName;
                size_t lastSlash = a.find_last_of('\\');
                if (lastSlash != string::npos) {
                    processName = processName.substr(lastSlash + 1);

                }
            }
        }
    }
    DWORD getProcessId() const {
        return processId;
    }
    bool getRunning(){
        return running;
    }
    string getProcState(){
        return (running) ? "running": "suspend";
    }
    string getProcessName() const {
        return processName;
    }
    SIZE_T getPageFaults()const{
        return pageFaults;
    }
    SIZE_T getCommitSize()const{
        return commitSize;
    }
    SIZE_T getPeakWS()const{
        return peakWorkingSet;
    }
    SIZE_T getWS()const{
        return workingSet;
    }
    SIZE_T getPagedPool()const{
        return pagedPool;
    }
    SIZE_T getNPagedPool()const{
        return NPpool;
    }
    string getProcessState() const {
        return processState;
    }

    DWORD getProcessExecutionTime() const {
        return processExecutionTime;
    }

    DWORD getProcessPriority() const {
        return processPriority;
    }
    string getSProcessPriority() const {
        return Spriority;
    }
    void setRunning(){
        running = !running;
    }

    int getTime()
    {   auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
        return elapsed_time.count();
    }

    void StartTime(){
        start_time = std::chrono::high_resolution_clock::now();
    }

    void otherData(){
         PROCESS_MEMORY_COUNTERS_EX pmc;
        if(GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))){
            workingSet = pmc.WorkingSetSize;
            peakWorkingSet = pmc.PeakWorkingSetSize;
            commitSize = pmc.PrivateUsage;
            pagedPool = pmc.QuotaPagedPoolUsage;
            NPpool = pmc.QuotaNonPagedPoolUsage;
            pageFaults = pmc.PageFaultCount;

        } else {
            cout << "HBGVSDCDFDGFNBVDCDSX " << GetLastError() << " mj,kmhngbfvdc";
        }
    }
    string beautifulShow(){
        int elapsed_time_seconds = getTime() / 1000; // Convert milliseconds to seconds
        int hours = elapsed_time_seconds / 3600;
        int minutes = (elapsed_time_seconds % 3600) / 60;
        int seconds = elapsed_time_seconds % 60;
        stringstream a;
        a <<  std::setfill('0') << std::setw(2) << hours << ":"
          << std::setfill('0') << std::setw(2) << minutes << ":" <<
            std::setfill('0') << std::setw(2) << seconds;

        return a.str();
    }

    void renewData()
    {
            processExecutionTime = getTime();
            processPriority = GetPriorityClass(hProcess);
            setStrPriority();
            std::thread mt([this](){
                otherData();
            });
            mt.join();
    }

    void setStrPriority(){
        switch(processPriority){
        case 32768: Spriority = "ABOVE_NORMAL"; break;
            case 128: Spriority = "HIGH"; break;
            case 16384: Spriority = "BELOW_NORMAL"; break;
            case 64: Spriority = "IDLE"; break;
            case 32: Spriority = "NORMAL"; break;
            case 256: Spriority = "REALTIME"; break;
            default:Spriority = "NORMAL"; break;
        }

    }
    bool SuspendProcess() {
         DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode) > 0) {

            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (snapshot == INVALID_HANDLE_VALUE) {
                return false;
            }

            THREADENTRY32 thread_entry;
            thread_entry.dwSize = sizeof(THREADENTRY32);

            if (Thread32First(snapshot, &thread_entry)) {
                do {
                    if (thread_entry.th32OwnerProcessID == processId) {
                        HANDLE threadHandle = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread_entry.th32ThreadID);
                        if (threadHandle != NULL) {
                            SuspendThread(threadHandle);
                            CloseHandle(threadHandle);
                        }
                    }
                } while (Thread32Next(snapshot, &thread_entry));
            }

            return true;
        }
        else {
            return false;
        }
    }

    void terminateProcess(){
           if (hProcess != NULL) {
                TerminateProcess(hProcess, 0);
            }
    }

    bool ResumeProcess() {
        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode) > 0) {

            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (snapshot == INVALID_HANDLE_VALUE) {
                return false;
            }

            THREADENTRY32 threadEntry;
            threadEntry.dwSize = sizeof(THREADENTRY32);

            if (Thread32First(snapshot, &threadEntry)) {
                do {
                    if (threadEntry.th32OwnerProcessID == processId) {
                        HANDLE threadHandle = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
                        if (threadHandle != NULL) {
                            ResumeThread(threadHandle);
                            CloseHandle(threadHandle);
                        }
                    }
                } while (Thread32Next(snapshot, &threadEntry));
            }

            return true;
        }
        else {
            return false;
        }
    }

    int DeathCheck(){
        DWORD exitCode;
        if(GetExitCodeProcess(hProcess, &exitCode)){
            if(exitCode != STILL_ACTIVE){
                return 1;
            }
        }
        return 0;
    }

    void setPriority(int a){
       if(SetPriorityClass(hProcess, a) == 0){
            cout << "FDDDDDDDDDDDDDDD" << GetLastError() << "KYJHGBFVDCDCSFVGBRHN";     }
       else {cout << "FVERONFVFKERFI";}
    }


    bool setAffinity(DWORD_PTR a){
       if(SetProcessAffinityMask(hProcess, a)){
            return 0;
       } else {
            return 1;
       }
    }
};
#endif // PROCESS_H
