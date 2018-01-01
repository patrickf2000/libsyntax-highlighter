@echo off
echo Installing syntax files....
if not exist %ProgramData%\syntax-highlighting md %ProgramData%\syntax-highlighting
cd syntax
copy .\*.xml %ProgramData%\syntax-highlighting
echo Done!