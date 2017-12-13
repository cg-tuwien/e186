@echo off
IF EXIST ..\..\application\project (
	echo Target directory '..\..\application\project' already exists. Will not copy/overwrite files.
) ELSE (
	xcopy "project-template" "..\..\application\project" /E /I
)
pause