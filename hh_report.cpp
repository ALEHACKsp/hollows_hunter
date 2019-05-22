#include "hh_report.h"

#include <string>
#include <sstream>

#include <iostream>
#include <iomanip>
#include <ctime>

bool is_suspicious_process(t_report report)
{
    if (report.errors) return false;
    if (report.replaced) {
        return true;
    }
    if (report.suspicious) {
        return true;
    }
    return false;
}

bool HHScanReport::appendReport(t_report &scan_report, std::string img_name)
{
    pidToReport[scan_report.pid] = scan_report;
    pidToName[scan_report.pid] = img_name;
    if (is_suspicious_process(scan_report)) {
        this->suspicious.push_back(scan_report.pid);
    }
    return true;
}

size_t HHScanReport::printSuspicious(std::stringstream &stream)
{
    std::vector<DWORD>::const_iterator itr;

    size_t printed = 0;
    size_t counter = 0;
    for (itr = this->suspicious.begin(); itr != suspicious.end(); itr++) {
        DWORD pid = *itr;
        stream << "[" << counter++ << "]:\n> PID: " << std::dec << pid << std::endl;
        stream << "> Name: " << this->pidToName[pid] << std::endl;
        printed++;
    }
    return printed;
}

std::string strtime(const time_t t)
{
    struct tm time_info;
    if (localtime_s(&time_info, &t) == 0) {
        std::stringstream str;
        str << std::dec
            << (1900  + time_info.tm_year) << "." << (1 + time_info.tm_mon) << "." << time_info.tm_mday
            << " " 
            << time_info.tm_hour << ":" << time_info.tm_min << ":" << time_info.tm_sec;
        return str.str();
    }
    return "";
}

std::string HHScanReport::toString()
{
    std::stringstream stream;
    //summary:
    stream << "--------" << std::endl;
    stream << "Scan at: " << strtime(this->startTime) << " (" << std::dec << startTime << ")\n";
    stream << "Finished scan in: " << std::dec << getScanTime() << " milliseconds\n";
    stream << "SUMMARY:\n";
    stream << "[+] Total Suspicious: " << std::dec << countSuspicious() << "\n";
    if (countSuspicious() > 0) {
        stream << "[+] List of suspicious: \n";
    }
    printSuspicious(stream);
    return stream.str();
}