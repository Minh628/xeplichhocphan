#include "ScheduleClass.h"
#include <fstream>
#include <vector> // Đã thêm thư viện này để sửa lỗi

using namespace std;

bool Schedule::isCompatible(const NhomLop &newGroup) const
{
    for (const auto &existingGroup : selectedGroups)
    {
        if (existingGroup.conflictsWith(newGroup))
            return false;
    }
    return true;
}

void Schedule::addGroup(const NhomLop &newGroup)
{
    selectedGroups.push_back(newGroup);
}

void Schedule::removeLastGroup()
{
    if (!selectedGroups.empty())
        selectedGroups.pop_back();
}

bool Schedule::isOnForbiddenDays(const set<int> &forbiddenDays) const
{
    for (const auto &group : selectedGroups)
    {
        if (group.isOnForbiddenDays(forbiddenDays))
            return true;
    }
    return false;
}

void Schedule::exportToCSV(const string &filePath) const
{
    ofstream fout(filePath);
    if (!fout)
        return;

    vector<vector<string>> tkb(11, vector<string>(8, ""));

    for (const auto &g : selectedGroups)
    {
        for (const auto &s : g.cacBuoi)
        {
            for (int i = 0; i < s.soTiet; ++i)
            {
                int tiet = s.tietBatDau + i;
                if (tiet >= 1 && tiet <= 10 && s.thu >= 2 && s.thu <= 7)
                    tkb[tiet][s.thu] = g.tenMon + " (" + g.nhom + ")";
            }
        }
    }

    fout << ",Thu 2,Thu 3,Thu 4,Thu 5,Thu 6,Thu 7\n";
    for (int tiet = 1; tiet <= 10; ++tiet)
    {
        fout << "Tiet " << tiet;
        for (int thu = 2; thu <= 7; ++thu)
            fout << "," << tkb[tiet][thu];
        fout << "\n";
    }

    fout << "\nMa mon,TEN,NHOM,THU,TIET BAT DAU,SO TIET,CO SO\n";
    for (const auto &g : selectedGroups)
    {
        for (const auto &s : g.cacBuoi)
        {
            fout << g.maMon << "," << g.tenMon << "," << g.nhom << ","
                 << s.thu << "," << s.tietBatDau << "," << s.soTiet << "," << s.coSo << "\n";
        }
    }
    fout.close();
}