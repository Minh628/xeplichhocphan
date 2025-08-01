#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <set>

namespace fs = std::filesystem;
using namespace std;

struct Session
{
    int thu;
    int tietBatDau;
    int soTiet;
    string coSo;
};

struct NhomLop
{
    string maMon;
    string tenMon;
    string nhom;
    vector<Session> cacBuoi;
};

// Kiểm tra trùng lịch 2 buổi
bool trungLich(const Session &a, const Session &b)
{
    if (a.thu != b.thu)
        return false;
    int ketThucA = a.tietBatDau + a.soTiet - 1;
    int ketThucB = b.tietBatDau + b.soTiet - 1;
    return !(ketThucA < b.tietBatDau || ketThucB < a.tietBatDau);
}

// Kiểm tra trùng lịch giữa 2 nhóm
bool nhomTrungLich(const NhomLop &a, const NhomLop &b)
{
    for (const auto &sa : a.cacBuoi)
    {
        for (const auto &sb : b.cacBuoi)
        {
            if (trungLich(sa, sb))
                return true;
        }
    }
    return false;
}

// Ưu tiên nhóm học ở cơ sở C
bool uuTienCoSoC(const NhomLop &a, const NhomLop &b)
{
    auto laCoSoC = [](const NhomLop &nhom)
    {
        for (const auto &s : nhom.cacBuoi)
            if (s.coSo == "C")
                return true;
        return false;
    };
    return laCoSoC(a) > laCoSoC(b);
}

// Backtracking với ưu tiên coSo C và loại các thứ không muốn học
void thuLai(int i, const vector<vector<NhomLop>> &tatCaMon,
            vector<NhomLop> &hienTai, vector<vector<NhomLop>> &ketQua,
            const set<int> &cacThuKhongMuonHoc)
{
    if (i == tatCaMon.size())
    {
        // Kiểm tra toàn bộ lịch không rơi vào ngày bị cấm
        for (const auto &nhom : hienTai)
        {
            for (const auto &s : nhom.cacBuoi)
            {
                if (cacThuKhongMuonHoc.count(s.thu))
                    return; // có tiết rơi vào ngày cấm → bỏ
            }
        }

        ketQua.push_back(hienTai);
        return;
    }

    vector<NhomLop> cacNhom = tatCaMon[i];
    sort(cacNhom.begin(), cacNhom.end(), uuTienCoSoC);

    for (const auto &nhom : cacNhom)
    {
        bool biTrung = false;
        for (const auto &daChon : hienTai)
        {
            if (nhomTrungLich(nhom, daChon))
            {
                biTrung = true;
                break;
            }
        }
        if (!biTrung)
        {
            hienTai.push_back(nhom);
            thuLai(i + 1, tatCaMon, hienTai, ketQua, cacThuKhongMuonHoc);
            hienTai.pop_back();
        }
    }
}

int main()
{
    // Nhập các thứ không muốn học
    cout << "Nhap cac thu ban khong muon hoc (vi du: 7 hoac 6 7), neu khong can loc thi 0: ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    set<int> cacThuKhongMuonHoc;
    int x;
    while (ss >> x)
    {
        cacThuKhongMuonHoc.insert(x);
    }

    ifstream fin("input.txt");
    if (!fin)
    {
        cerr << "Khong the mo file input.\n";
        system("pause");
        return 1;
    }

    unordered_map<string, NhomLop> nhomMap;
    string ma, ten, nhom, coSo;
    int thu, batDau, soTiet;

    while (fin >> ma >> ten >> nhom >> thu >> batDau >> soTiet >> coSo)
    {
        string khoa = ma + "_" + nhom;
        Session s = {thu, batDau, soTiet, coSo};

        if (nhomMap.count(khoa) == 0)
        {
            nhomMap[khoa] = {ma, ten, nhom, {s}};
        }
        else
        {
            nhomMap[khoa].cacBuoi.push_back(s);
        }
    }
    fin.close();

    unordered_map<string, vector<NhomLop>> monMap;
    for (auto &[khoa, nhom] : nhomMap)
    {
        monMap[nhom.maMon].push_back(nhom);
    }

    vector<vector<NhomLop>> tatCaMon;
    for (auto &[_, danhSach] : monMap)
    {
        tatCaMon.push_back(danhSach);
    }

    vector<NhomLop> hienTai;
    vector<vector<NhomLop>> ketQua;
    thuLai(0, tatCaMon, hienTai, ketQua, cacThuKhongMuonHoc);
    cout << "Co " << ketQua.size() << " lich hop le:\n";

    // XÓA THƯ MỤC CŨ nếu đã tồn tại
    if (fs::exists("output_TKB"))
    {
        fs::remove_all("output_TKB");
    }
    fs::create_directory("output_TKB");

    int soThuTu = 1;
    for (const auto &lich : ketQua)
    {
        string tenFile = "output_TKB/thoikhoabieu_" + to_string(soThuTu++) + ".csv";
        ofstream fout(tenFile);
        vector<vector<string>> tkb(11, vector<string>(8, ""));

        for (const auto &g : lich)
        {
            for (const auto &s : g.cacBuoi)
            {
                for (int i = 0; i < s.soTiet; ++i)
                {
                    int tiet = s.tietBatDau + i;
                    tkb[tiet][s.thu] = g.tenMon + " (" + g.nhom + ")";
                }
            }
        }

        fout << ",Thu 2,Thu 3,Thu 4,Thu 5,Thu 6,Thu 7\n";
        for (int tiet = 1; tiet <= 10; ++tiet)
        {
            fout << "Tiet " << tiet;
            for (int thu = 2; thu <= 7; ++thu)
            {
                fout << "," << tkb[tiet][thu];
            }
            fout << "\n";
        }

        fout << "\nMa mon,TEN,NHOM,THU,TIET BAT DAU,SO TIET,CO SO\n";
        for (const auto &g : lich)
        {
            for (const auto &s : g.cacBuoi)
            {
                fout << g.maMon << "," << g.tenMon << "," << g.nhom << ","
                     << s.thu << "," << s.tietBatDau << "," << s.soTiet << "," << s.coSo << "\n";
            }
        }

        fout.close();
    }
    system("pause");
    return 0;
}
