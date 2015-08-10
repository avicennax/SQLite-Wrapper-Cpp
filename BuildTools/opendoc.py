# File: openf.py
# Author: Simon Haxby
# Description: Opens a file with the system default application

import sys
import os
import subprocess

def openf(filepath):
	if os.path.exists(filepath):
		if sys.platform.startswith('darwin'):
			subprocess.call(('open', filepath))
		elif os.name == 'nt':
			os.startfile(filepath)
		elif os.name == 'posix':
			subprocess.call(('xdg-open', filepath))
		else:
			print "System not detected..\n"
	else:
		print "File not found..\n"
		
if __name__ == "__main__":
	if len(sys.argv) == 1:
		paths = ['..\Docs\html\index.html'] # Enter strings here for reoccurring files.
	else:
		paths = sys.argv
		paths.pop(0)
	for filepath in paths:
		openf(filepath)