#!/usr/bin/python                                                                   
# -*- coding: utf-8 -*-

from threading import Thread
import threading
from subprocess import Popen, PIPE
import os                         

class CommandError(Exception):
        def __init__(self, value):
                self.value = value
        def __str__(self):        
                return repr(self.value)

builder_lock  = threading.RLock()

class QtLobbyBuilder(Thread):
        def __init__(self, targetDir, onMessage, onCompleted, clean):
                Thread.__init__(self)              
                self.onMessage = onMessage         
                self.onCompleted = onCompleted     
                self.revision = "HEAD"             
                self.abort = False
		self.dir = targetDir
		self.config = "debug"
		self.clean = clean
		print "QtLobbyBuilder instance created"

        def runCommand(self, command):
                print command         
                p = Popen(command, stdout=PIPE, stderr=PIPE, shell=True)
                (stdout, stderr) = p.communicate()
                if p.returncode != 0:                                   
                        self.onMessage("Error executing " + command + "!\n" + str(stderr))
                        raise CommandError("Command failed")                              
		print stdout
                return stdout                                                             

        def run(self):
		print "Building"
		pwd = os.getcwd()
       	        os.chdir(self.dir)
               	try:               
                       	rev = str(self.revision)
                        self.onMessage("Perforimg svn update...")
       	                self.runCommand("svn up -r " + rev)      
               	        rev = self.runCommand("svn up | perl -e '<> =~ /At revision (\d+)./; print $1;'")
			if not self.clean and os.path.exists("/var/www/qtlobby.oxnull.net/htdocs/qtlobby.r"+rev+"_installer.exe"):
				self.onMessage("Requested revision was already built")
				self.onCompleted({"Exe": "http://qtlobby.oxnull.net/qtlobby.r"+rev+".exe.zip", \
				                  "Installer": "http://qtlobby.oxnull.net/qtlobby.r"+rev+"_installer.exe", \
						  "Debug symbols": "http://qtlobby.oxnull.net/qtlobby.r"+rev+"_symbols.dbg.zip"})
				return
                        self.onMessage("Building revision "+rev+"")
			self.onMessage("Compressing resources...")
			self.runCommand("./mkresources.sh")
			self.onMessage("Building...")
			os.chdir("cbuild")
			if self.clean:
				self.onMessage("Cleaning up building environment...")
				self.runCommand("rm -rf *")
			self.runCommand("touch ../src/config.h")
			self.runCommand("rm -f cbuild/src/qtlobby.*")
			self.runCommand("cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake -DBUILDBOT=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo ..")
               	        self.runCommand("make")
                       	os.chdir("src")
                        self.runCommand("objcopy --only-keep-debug qtlobby.exe qtlobby.dbg")
       	                self.runCommand("strip --strip-debug --strip-unneeded qtlobby.exe")
			self.runCommand("rm -f *.zip")
			self.runCommand("zip -9 qtlobby.exe.zip qtlobby.exe")
			self.runCommand("zip -9 qtlobby.dbg.zip qtlobby.dbg")
               	        os.chdir("../..")
                       	self.onMessage("Compiling installer...")
                        self.runCommand("makensis installer.nsi")
       	                self.runCommand("mv cbuild/src/qtlobby.exe.zip ~apache/qtlobby.oxnull.net/htdocs/qtlobby.r"+rev+".exe.zip")
               	        self.runCommand("mv qtlobby_installer.exe ~apache/qtlobby.oxnull.net/htdocs/qtlobby.r"+rev+"_installer.exe")
                       	self.runCommand("mv cbuild/src/qtlobby.dbg.zip ~apache/qtlobby.oxnull.net/htdocs/qtlobby.r"+rev+"_symbols.dbg.zip")
                        self.onCompleted({"Exe": "http://qtlobby.oxnull.net/qtlobby.r"+rev+".exe.zip", \
       	                                  "Installer": "http://qtlobby.oxnull.net/qtlobby.r"+rev+"_installer.exe", \
               	                          "Debug symbols": "http://qtlobby.oxnull.net/qtlobby.r"+rev+"_symbols.dbg.zip"})
                except CommandError:
       	                pass
               	finally:
                        os.chdir(pwd)

def message(msg):
        print "[MESSAGE] " + msg;

def completed(links):
        print "Completed!"
        for k in links.keys():
                print k + ": " + links[k]

if __name__ == "__main__":
        builder = QtLobbyBuilder("../qtlobby", message, completed)
	builder.revision = "HEAD"
        builder.start()

