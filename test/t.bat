@echo off & setlocal
set std=%1
if not "%std%"=="" set std=-std:%std%

::set    stdspn=-Dspan_CONFIG_SELECT_STD_SPAN=1 -Dspan_CONFIG_SELECT_NONSTD_SPAN=1 
set  contract=-Dspan_CONFIG_CONTRACT_VIOLATION_TERMINATES=0 -Dspan_CONFIG_CONTRACT_VIOLATION_THROWS=1
set make_span=-Dspan_CONFIG_PROVIDE_MAKE_SPAN=1
set byte_span=-Dspan_CONFIG_PROVIDE_BYTE_SPAN=1

cl -W3 -EHsc %std% %stdspn% %contract% %make_span% %byte_span% -DNOMINMAX -I../include/nonstd span-main.t.cpp span.t.cpp && span-main.t.exe
endlocal

