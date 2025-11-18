#include "NhomLop.h"

using namespace std;

NhomLop::NhomLop(string ma, string ten, string n)
    : maMon(ma), tenMon(ten), nhom(n) {}

void NhomLop::addSession(const Session &s)
{
    cacBuoi.push_back(s);
}

bool NhomLop::conflictsWith(const NhomLop &other) const
{
    for (const auto &sa : cacBuoi)
    {
        for (const auto &sb : other.cacBuoi)
        {
            if (sa.conflictsWith(sb))
                return true;
        }
    }
    return false;
}

bool NhomLop::isAtCoSo(const string &coSo) const
{
    for (const auto &s : cacBuoi)
    {
        if (s.coSo == coSo)
            return true;
    }
    return false;
}

bool NhomLop::isOnForbiddenDays(const set<int> &forbiddenDays) const
{
    for (const auto &s : cacBuoi)
    {
        if (forbiddenDays.count(s.thu))
            return true;
    }
    return false;
}