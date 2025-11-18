@echo off
echo Dang bien dich code...

g++ main/main.cpp main/ScheduleSolver/ScheduleSolver.cpp main/ScheduleSolver/Schedule/ScheduleClass.cpp main/ScheduleSolver/Schedule/NhomLop/NhomLop.cpp main/ScheduleSolver/Schedule/NhomLop/Session/Session.cpp -o xep_lich

if %errorlevel% neq 0 (
    echo.
    echo [LOI] Bien dich that bai!
    pause
    exit /b
)

echo.
echo Xong! Dang chay chuong trinh...
xep_lich.exe
pause