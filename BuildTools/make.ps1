# Generic C++ Make script for calling Cmake on Windows
# Can be exported with export-make.ps1

if ( -Not (Test-Path ../Build) )
	{ mkdir ../Build | Out-Null }
cd ../Build
cmake .. -G "Visual Studio 12 2013"
cd ..