@echo off & setlocal enableextensions enabledelayedexpansion
::
:: tg.bat - compile & run tests (GNUC).
::

:: if no std is given, use c++11

set std=%1
set args=%2 %3 %4 %5 %6 %7 %8 %9
if "%1" == "" set std=c++11

call :CompilerVersion version
echo g++ %version%: %std% %args%

::set stdspn=-Dspan_CONFIG_SELECT_STD_SPAN=1 -Dspan_CONFIG_SELECT_NONSTD_SPAN=1

set span_contract=^
    -Dspan_CONFIG_CONTRACT_VIOLATION_TERMINATES=0 ^
    -Dspan_CONFIG_CONTRACT_VIOLATION_THROWS=1

set span_provide=^
    -Dspan_FEATURE_CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE=1 ^
    -Dspan_FEATURE_WITH_CONTAINER_TO_STD=99 ^
    -Dspan_FEATURE_MEMBER_AT=2 ^
    -Dspan_FEATURE_MEMBER_BACK_FRONT=1 ^
    -Dspan_FEATURE_MEMBER_SWAP=1 ^
    -Dspan_FEATURE_SAME=1 ^
    -Dspan_FEATURE_MAKE_SPAN_TO_STD=99 ^
    -Dspan_FEATURE_BYTE_SPAN=1

set flags=-Wpedantic -Wconversion -Wsign-conversion -Wno-padded -Wno-missing-noreturn
set   gpp=g++

%gpp% -std=%std% -O2 -Wall -Wextra %flags% %stdspn% %span_contract% %span_provide% -o span-main.t.exe -I../include/nonstd span-main.t.cpp span.t.cpp && span-main.t.exe

endlocal & goto :EOF

:: subroutines:

:CompilerVersion  version
echo off & setlocal enableextensions
set tmpprogram=_getcompilerversion.tmp
set tmpsource=%tmpprogram%.c

echo #include ^<stdio.h^>     > %tmpsource%
echo int main(){printf("%%d.%%d.%%d\n",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);} >> %tmpsource%

g++ -o %tmpprogram% %tmpsource% >nul
for /f %%x in ('%tmpprogram%') do set version=%%x
del %tmpprogram%.* >nul
endlocal & set %1=%version%& goto :EOF
