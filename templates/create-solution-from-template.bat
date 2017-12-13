@echo off
IF EXIST "..\..\e186_application.sln" (
	echo Solution ..\..\e186_application.sln already exists. Will not overwrite.
	pause
) ELSE (
	copy "solution-template\e186_application.sln" "..\..\e186_application.sln"
	echo Continue to open folder ..\..\
	pause
	explorer.exe "..\..\"
)
