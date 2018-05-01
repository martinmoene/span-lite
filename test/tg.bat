@echo off & setlocal
set std=%1
if "%std%"=="" set std=c++14

::set    stdspn=-Dspan_CONFIG_SELECT_STD_SPAN=1 -Dspan_CONFIG_SELECT_NONSTD_SPAN=1 
set  contract=-Dspan_CONFIG_CONTRACT_VIOLATION_TERMINATES=0 -Dspan_CONFIG_CONTRACT_VIOLATION_THROWS=1
set make_span=-Dspan_CONFIG_PROVIDE_MAKE_SPAN=1
set byte_span=-Dspan_CONFIG_PROVIDE_BYTE_SPAN=1

set     flags=-Wpedantic -Wno-padded -Wno-missing-noreturn
set       gpp=g++

%gpp% -std=%std% -O2 -Wall -Wextra %flags% %stdspn% %contract% %make_span% %byte_span% -o span-main.t.exe -I../include/nonstd span-main.t.cpp span.t.cpp && span-main.t.exe
endlocal
