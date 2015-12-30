@echo off

set PATHTOFILE=%~dp1
set FILENAME=%~n1
set INFILE="%PATHTOFILE%%~nx1"
set OUTFILE="%PATHTOFILE%%FILENAME%.h"
set VARNAME=effect%FILENAME%

echo static char %VARNAME%[] = { > %OUTFILE%

FOR /F "usebackq tokens=* delims=" %%i IN (%INFILE%) DO (
  echo "%%i\n" >> %OUTFILE%
)

echo } >> %OUTFILE%
