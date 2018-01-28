@echo off

SET OPTS=-FC -GR- -EHa- -nologo -Zi
SET CODE_HOME=%cd%\src
pushd output
cl %OPTS% %CODE_HOME%\main.c -Fetest.exe 
popd
