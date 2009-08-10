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
import build
import getopt
import configobj

class Main:
    operator = []

    def onload(self,tasc):
        self.client = tasc
        self.app = tasc.main
        # config
        self.config = configobj.ConfigObj("FlexBot.conf")
        self.config.list_values = True
        self.users = self.config['buildbot']['operator']

        
    def onsaidprivate(self,user,message):
        if message.startswith("!build") == False or user in self.operator == False:
            return
        try:
            opts, args = getopt.gnu_getopt(message.split(), "h", ["profile=", "revision=", "help"])
        except getopt.GetoptError, err:
            self.SayPrivate(user, err)
            return;
        profile = "QtLobby"
        revision = "HEAD"
        help = False
        for o, a in opts:
            if o == "--profile":
                profile = a
            if o == "--revision":
                revision = a
            elif o in ("-h", "--help"):
                help = True
        
        if help:
            self.SendHelp(user)
            return;
        
        if profile == "":
            self.SayPrivate(user, "Please specify profile. For more information use --help.")
            return
        elif profile == "QtLobby":
            self.employer = user
            self.SayPrivate(user, "Building profile " + profile)
            builder = build.QtLobbyBuilder(self.onMessage, self.onCompleted)
            builder.revision = revision
            builder.start()
    
    def onMessage(self, msg):
        self.SayPrivate(self.employer, msg)
    
    def onCompleted(self, links):
        self.SayPrivate(self.employer, "Build complete.")
        for k in links.keys():
            self.SayPrivate(self.employer, k + ": " + links[k])
        
    def SayPrivate(self, user, message):
        self.client.sock.send ("SAYPRIVATE %s %s \n"% (user, message) )
        
    def SendHelp(self, user):
        self.SayPrivate(user, "Help:")
        self.SayPrivate(user, "--profile (required) The build profile.")
        self.SayPrivate(user, "--revision The targeted revision.")
        self.SayPrivate(user, "-h --help Displays this help text.")