@echo off

premake5.exe --file="premake.lua" vs2022

IF NOT %errorlevel%==0 (
	echo Encountered error!
	pause
)