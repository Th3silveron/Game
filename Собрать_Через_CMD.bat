@echo off
echo ==========================================
echo    Сборка через Developer Command Prompt
echo ==========================================
echo.

echo Поиск Developer Command Prompt...

REM Попытка найти Developer Command Prompt
set DEVENV_CMD=""
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    set DEVENV_CMD="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
    set DEVENV_CMD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" (
    set DEVENV_CMD="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
) else (
    echo ОШИБКА: Developer Command Prompt не найден!
    echo Попробуйте использовать Собрать_Проект.bat
    echo.
    pause
    exit /b 1
)

echo Найден: %DEVENV_CMD%
echo.

echo Инициализация Developer Command Prompt...
call %DEVENV_CMD%

echo.
echo Сборка XYZEngine...
msbuild XYZEngine\XYZEngine.vcxproj /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% neq 0 (
    echo ОШИБКА при сборке XYZEngine!
    pause
    exit /b 1
)

echo.
echo Сборка XYZRoguelike...
msbuild XYZRoguelike\XYZRoguelike.vcxproj /p:Configuration=Debug /p:Platform=x64
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
echo Запуск игры...
XYZRoguelike\x64\Debug\XYZRoguelike.exe

pause


