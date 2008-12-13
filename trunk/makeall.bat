@echo off
echo --- Building Mui ---
echo. 

echo --- Building cfmod ---
echo.

cd src\cfmod
make lib

if not %errorlevel% == 0 goto bad_cfmod

echo.
echo --- cfmod built ---
echo.

echo --- Building app MUI ---
echo.

cd ..\MUI
qmake
make

if not %errorlevel% == 0 goto bad_qmake
echo.
echo --- MUI built ---
echo.
goto end

:bad_cfmod
echo --- Error building cfmod ---
echo.
goto end

:bad_qmake
echo --- Error building MUI ---
echo.
goto end

:end
cd ..\..\
echo Done.
echo.



