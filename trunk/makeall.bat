@echo off
echo --- Building Mui ---
echo. 

if not exist .\src\mui\makefile goto no_clean

echo --- Clearing workspace ---
echo.

cd .\src\mui
mingw32-make clean
cd ..\..\


:no_clean
echo --- Building component cfmod ---
echo.

cd .\src\cfmod
mingw32-make win_lib

if not %errorlevel% == 0 goto bad_cfmod

echo.
echo --- Component cfmod built ---
echo.

echo --- Building component cplaylist ---
echo.
cd ..\cplaylist
mingw32-make win_lib

if not %errorlevel% == 0 goto bad_cplaylist

echo.
echo --- Component cplaylist built ---
echo.

echo --- Building component caudiotags ---
echo.

cd ..\caudiotags
mingw32-make win_lib

if not %errorlevel% == 0 goto bad_caudiotags

echo.
echo --- Component caudiotags built ---
echo.


echo --- Building app MUI ---
echo.

cd ..\MUI
qmake
mingw32-make

if not %errorlevel% == 0 goto bad_qmake
echo.
echo --- MUI built ---
echo.
goto end

:bad_cfmod
echo --- Error building cfmod ---
echo.
goto end

:bad_cplaylist
echo --- Error building cplaylist ---
echo.
goto end

:bad_caudiotags
echo --- Error building caudiotags ---
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



