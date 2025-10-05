@echo off
echo Запуск новой рогалик игры...
echo.
echo Управление:
echo - WASD: движение
echo - ПРОБЕЛ: атака
echo - ESC: выход
echo.
echo Система здоровья:
echo - Игрок: 100 HP, 50 AP
echo - Враг: 50 HP, 25 AP
echo - Зеленые полоски: здоровье
echo - Синие полоски: броня
echo.
pause
echo Запускаем игру...

REM Попытка запуска через Visual Studio
if exist "XYZRoguelike\x64\Debug\XYZRoguelike.exe" (
    echo Найден исполняемый файл, запускаем...
    XYZRoguelike\x64\Debug\XYZRoguelike.exe
) else (
    echo Исполняемый файл не найден!
    echo Пожалуйста, соберите проект в Visual Studio:
    echo 1. Откройте Game.sln
    echo 2. Выберите Debug x64
    echo 3. Соберите проект XYZRoguelike
    echo.
    pause
)


