#pragma once
#include <vector>
#include <string>
#include <set>
#include "NhomLop/NhomLop.h"

struct Schedule
{
    std::vector<NhomLop> selectedGroups;

    bool isCompatible(const NhomLop &newGroup) const;
    void addGroup(const NhomLop &newGroup);
    void removeLastGroup();
    bool isOnForbiddenDays(const std::set<int> &forbiddenDays) const;
    void exportToCSV(const std::string &filePath) const;
};