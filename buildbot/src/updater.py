# -*- coding: utf-8 -*-
"""
Copyright (c) 2009, Marcel Hauf
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Marcel Hauf ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Marcel Hauf BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""
import configobj
from subprocess import Popen, PIPE

class Main:
    operator = []
    employer = ""

    def onload(self,tasc):
	self.client = tasc
	# config
	self.config = configobj.ConfigObj("FlexBot.conf")
	self.config.list_values = True
	self.operator = self.config['updater']['operator']

    def onsaidprivate(self,user,message):
        if user in self.operator and message.startswith("!update"):
            self.SayPrivate(user, "Looking for updates...")
            self.employer = user
            stdout = self.RunCommand("svn up -r HEAD")
            self.SayPrivate(user, "SVN report: " + stdout)

    def SayPrivate(self, user, message):
        self.client.sock.send ("SAYPRIVATE %s %s \n"% (user, message) )

    def RunCommand(self, command):
        p = Popen(command, stdout=PIPE, stderr=PIPE, shell=True)
        (stdout, stderr) = p.communicate()
        if p.returncode != 0:
            self.SayPrivate(self.employer, "Error executing " + command + "!\n" + str(stderr))
        return stdout
