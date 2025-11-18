#pragma once
#include <string>
#include <vector>
#include <set>
#include "Session/Session.h"

struct NhomLop
{
    std::string maMon;
    std::string tenMon;
    std::string nhom;
    std::vector<Session> cacBuoi;

    NhomLop() = default;
    NhomLop(std::string ma, std::string ten, std::string n);

    void addSession(const Session &s);
    bool conflictsWith(const NhomLop &other) const;
    bool isAtCoSo(const std::string &coSo) const;
    bool isOnForbiddenDays(const std::set<int> &forbiddenDays) const;
};