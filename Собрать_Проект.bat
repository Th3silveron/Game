@echo off
echo ==========================================
echo    Сборка рогалик игры с системой здоровья
echo ==========================================
echo.

echo Шаг 1: Сборка библиотеки XYZEngine...
echo Это может занять некоторое время...
echo.

REM Попытка найти MSBuild
set MSBUILD_PATH=""
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
) else (
    echo ОШИБКА: MSBuild не найден!
    echo Пожалуйста, установите Visual Studio или добавьте MSBuild в PATH
    echo.
    pause
    exit /b 1
)

echo Найден MSBuild: %MSBUILD_PATH%
echo.

echo Сборка XYZEngine...
%MSBUILD_PATH% XYZEngine\XYZEngine.vcxproj /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% neq 0 (
    echo ОШИБКА при сборке XYZEngine!
    pause
    exit /b 1
)

echo.
echo Шаг 2: Сборка XYZRoguelike...
%MSBUILD_PATH% XYZRoguelike\XYZRoguelike.vcxproj /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% neq 0 (
    echo ОШИБКА при сборке XYZRoguelike!
    pause
    exit /b 1
)

echo.
echo ==========================================
echo    Сборка завершена успешно!
echo ==========================================
echo.
echo Теперь можно запустить игру:
echo XYZRoguelike\x64\Debug\XYZRoguelike.exe
echo.
echo Управление:
echo - WASD: движение
echo - ПРОБЕЛ: атака
echo - ESC: выход
echo.
pause


