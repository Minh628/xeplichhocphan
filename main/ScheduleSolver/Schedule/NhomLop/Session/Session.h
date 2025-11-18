#pragma once
#include <string>

struct Session
{
    int thu;
    int tietBatDau;
    int soTiet;
    std::string coSo;

    bool conflictsWith(const Session &other) const;
};