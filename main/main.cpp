#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include "ScheduleSolver/ScheduleSolver.h"

using namespace std;

int main()
{
    cout << "Nhap cac thu ban khong muon hoc (vi du: 7 hoac 6 7), neu khong can loc thi 0: ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    set<int> cacThuKhongMuonHoc;
    int x;
    while (ss >> x)
    {
        if (x != 0)
            cacThuKhongMuonHoc.insert(x);
    }

    ScheduleSolver solver;

    if (!solver.loadData("input.txt"))
    {
        cerr << "Loi: Khong tim thay file input.txt!\n";
        system("pause");
        return 1;
    }

    solver.setForbiddenDays(cacThuKhongMuonHoc);

    cout << "Dang tim kiem lich...\n";
    solver.findSchedules();

    int count = solver.getSolutionCount();
    cout << "Co " << count << " lich hop le.\n";

    if (count > 0)
    {
        solver.saveResults("output_TKB");
        cout << "Da xuat file vao thu muc 'output_TKB'.\n";
    }

    system("pause");
    return 0;
}