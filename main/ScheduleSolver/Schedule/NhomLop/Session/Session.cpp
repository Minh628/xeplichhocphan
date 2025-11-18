#include "Session.h"

using namespace std;

bool Session::conflictsWith(const Session &b) const
{
    const Session &a = *this;

    if (a.thu != b.thu)
        return false;

    int ketThucA = a.tietBatDau + a.soTiet - 1;
    int ketThucB = b.tietBatDau + b.soTiet - 1;

    return !(ketThucA < b.tietBatDau || ketThucB < a.tietBatDau);
}