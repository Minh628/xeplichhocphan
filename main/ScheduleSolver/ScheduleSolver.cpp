#include "ScheduleSolver.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;
using namespace std;

bool ScheduleSolver::loadData(const string &inputFilePath)
{
    ifstream fin(inputFilePath);
    if (!fin)
    {
        cerr << "Khong the mo file: " << inputFilePath << endl;
        return false;
    }

    unordered_map<string, NhomLop> nhomMap;
    string ma, ten, nhom, coSo;
    int thu, batDau, soTiet;

    while (fin >> ma >> ten >> nhom >> thu >> batDau >> soTiet >> coSo)
    {
        string khoa = ma + "_" + nhom;
        Session s = {thu, batDau, soTiet, coSo};

        if (nhomMap.count(khoa) == 0)
            nhomMap[khoa] = NhomLop(ma, ten, nhom);

        nhomMap[khoa].addSession(s);
    }
    fin.close();

    allCourseGroups.clear();
    for (auto &[khoa, nhomLop] : nhomMap)
        allCourseGroups[nhomLop.maMon].push_back(nhomLop);

    coursesToSchedule.clear();
    for (auto &[maMon, danhSach] : allCourseGroups)
        coursesToSchedule.push_back(danhSach);

    return true;
}

void ScheduleSolver::setForbiddenDays(const set<int> &days)
{
    forbiddenDays = days;
}

bool ScheduleSolver::prioritizeCoSoC(const NhomLop &a, const NhomLop &b)
{
    return a.isAtCoSo("C") > b.isAtCoSo("C");
}

void ScheduleSolver::findSchedules()
{
    solutions.clear();
    Schedule currentSchedule;
    if (!coursesToSchedule.empty())
        backtrack(0, currentSchedule);
}

void ScheduleSolver::backtrack(int courseIndex, Schedule &currentSchedule)
{
    if (courseIndex == coursesToSchedule.size())
    {
        if (!currentSchedule.isOnForbiddenDays(forbiddenDays))
            solutions.push_back(currentSchedule);
        return;
    }

    vector<NhomLop> currentCourseOptions = coursesToSchedule[courseIndex];
    sort(currentCourseOptions.begin(), currentCourseOptions.end(), prioritizeCoSoC);

    for (const auto &nhom : currentCourseOptions)
    {
        if (currentSchedule.isCompatible(nhom))
        {
            currentSchedule.addGroup(nhom);
            backtrack(courseIndex + 1, currentSchedule);
            currentSchedule.removeLastGroup();
        }
    }
}

void ScheduleSolver::saveResults(const string &outputDirectory) const
{
    if (fs::exists(outputDirectory))
        fs::remove_all(outputDirectory);

    fs::create_directory(outputDirectory);

    int fileIndex = 1;
    for (const auto &schedule : solutions)
    {
        string filePath = outputDirectory + "/thoikhoabieu_" + to_string(fileIndex++) + ".csv";
        schedule.exportToCSV(filePath);
    }
}

int ScheduleSolver::getSolutionCount() const
{
    return solutions.size();
}