import string
import os
from colors import *
def readconfigfile(filename):
	entries = dict()
	try:
		f = open(filename,"r")
	except:
		error("Error reading config file "+filename)
		return entries
	s1 = f.read()
	s2 = ""
	for l in s1.replace("\r","").split("\n"):
		if not l.strip(" \t").startswith("---"):
			if l.count("---") == 0:
				s2 += l+ "\n"
			else:
				h = l.split("---")
				s2 += h[0]+ "\n"
	s = s2.strip(" \n\r\t;").replace("\r\n","")
	f.close()
	j = s.split(";")
	
	for entry in j:
		ed = entry.split("=")
		if len(ed) >= 2:
			entries.update([(ed[0].lower().strip(" \n\r\t;").replace("\r\n",""),"=".join(ed[1:]).strip(" \n\r\t;").replace("\r\n",""))])
		else:
			error("Invalid line on config file %s :\n\t%s" % ( filename , entry ) + normal)
	good("Loaded config file %s succesfully, %i entries" % (filename,len(entries)))
	return entries
def writeconfigfile(filename,entries):
	try:
		os.remove(filename)
	except:
		pass
	f = open(filename,"aw")
	for entry in entries:
		f.write("%s=%s;\n" % (entry.lower().strip(),entries[entry].strip()))
def parselist(string,sep):
	if string.count(sep) < 1:
		return [string]
	j = string.split(sep)
	l = []
	for i in j:
		l.append(i.strip())
	return l
			
