@echo off
setlocal

cd /d "%~dp0"

compdb -p build list > compile_commands.json
if exist ".\build\compile_commands.json" del /q ".\build\compile_commands.json"
move /y ".\compile_commands.json" ".\build\compile_commands.json"

endlocal