if ( -Not (Test-Path Build) )
	{ mkdir Build | Out-Null }
cd Build
cmake .. -G "MinGW Makefiles"
cd ..