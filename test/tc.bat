@echo off & setlocal
set std=%1
if "%std%"=="" set std=c++14

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

set  clang=C:\Program Files\LLVM\bin\clang
set  flags=-Wpedantic -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-noreturn -Wno-documentation-unknown-command -Wno-documentation-deprecated-sync -Wno-documentation -Wno-weak-vtables -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-exit-time-destructors -Wno-global-constructors

"%clang%" -std=%std% -O2 -Wall -Wextra %flags% %stdspn% %span_contract% %span_provide% -fms-compatibility-version=19.00 -isystem "%VCInstallDir%include" -isystem "%WindowsSdkDir_71A%include" -I../include/nonstd -o span-main.t.exe span-main.t.cpp span.t.cpp && span-main.t.exe
endlocal
