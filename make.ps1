if ( -Not (Test-Path Build) )
	{ mkdir Build | Out-Null }
cd Build
cmake .. -G "Visual Studio 12 2013"
cd ..