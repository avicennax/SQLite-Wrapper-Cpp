# Deletes build dir if it exists

if ( (Test-Path ../Build) )
	{ rm -Force -Recurse ../Build }
