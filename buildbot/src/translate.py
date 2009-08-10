#!/usr/bin/python                                                                   
# -*- coding: utf-8 -*-

import os                         
import tempfile

class CommandError(Exception):
        def __init__(self, value):
                self.value = value
        def __str__(self):        
                return repr(self.value)


class QtLobbyStackTranslator():
        def __init__(self):
		self.targetDir="~apache/qtlobby.oxnull.net/htdocs/translations"
		self.symbols = "~apache/qtlobby.oxnull.net/htdocs/qtlobby.r%d_symbols.dbg"

        def translate(self, url, revision):
		(handle1, tmp1) = tempfile.mkstemp()
		(handle2, tmp2) = tempfile.mkstemp()
		os.system("wget -q -O %s %s" % (tmp1, url))
		os.system("./translate.pl %s > %s < %s" % ((self.symbols % revision), tmp2, tmp1))
		os.system("cp %s %s" % (tmp2, self.targetDir))
		os.system("chmod 644 %s/%s" % (self.targetDir, tmp2.split("/")[-1]))
		os.close(handle1)
		os.close(handle2)
		os.system("rm -f %s %s" % (tmp1, tmp2))
		return "http://qtlobby.oxnull.net/translations/" + tmp2.split("/")[-1]

if __name__ == "__main__":
        translator = QtLobbyStackTranslator()
	print translator.translate("http://pastey.net/120762+", 355)

