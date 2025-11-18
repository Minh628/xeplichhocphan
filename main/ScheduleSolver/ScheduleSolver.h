#pragma once
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
// LƯU Ý: Đã đổi tên file include để tránh lỗi
#include "Schedule/ScheduleClass.h"
#include "Schedule/NhomLop/NhomLop.h"

class ScheduleSolver
{
public:
    ScheduleSolver() = default;

    bool loadData(const std::string &inputFilePath);
    void setForbiddenDays(const std::set<int> &days);
    void findSchedules();
    void saveResults(const std::string &outputDirectory) const;
    int getSolutionCount() const;

private:
    std::unordered_map<std::string, std::vector<NhomLop>> allCourseGroups;
    std::vector<std::vector<NhomLop>> coursesToSchedule;
    std::set<int> forbiddenDays;
    std::vector<Schedule> solutions;

    void backtrack(int courseIndex, Schedule &currentSchedule);
    static bool prioritizeCoSoC(const NhomLop &a, const NhomLop &b);
};