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
import translate

class Main:
    operator = []
    profile = "debug"
    revision = "HEAD"
    employer = ""
    reportToChannel = ""

    def onload(self,tasc):
        self.client = tasc
        self.app = tasc.main
        # config
        self.config = configobj.ConfigObj("FlexBot.conf")
        self.config.list_values = True
        self.operator = self.config['buildbot']['operator']
        self.builder = build.QtLobbyBuilder(self.config['buildbot'][self.profile]['builddir'], self.onMessage, self.onCompleted)

    def onsaid(self, channel,user,message):
        self.sayTo("Marcel", message)
        self.reportToChannel = channel
        self.onsaidprivate(user, message)
        self.reportToChannel = ""
        
    def onsaidprivate(self,user,message):
        #if not user in self.operator:
        #    return
        if message.startswith("!translate"):
            try:
                opts, args = getopt.gnu_getopt(message.split(), "u:r:h", ["url=", "revision=", "help"])
            except getopt.GetoptError, err:
                self.sayTo(user, err)
                return;
            url = ""
            revision = ""
            for o, a in opts:
                if o == "--url":
                    url = a
                if o == "--revision":
                    revision = a
                elif o in ("-h", "--help"):
                    self.SendHelp(user)
                    return
            self.Translate(user, url, revision)
            return
        elif message.startswith("!build"):
            self.Build(user, message)
            return

    def Translate(self, user, url, revision):
        if revision == "":
            self.sayTo(user, "You are missing a revision.")
            return
        elif url == "":
            self.sayTo(user, "You are missing an url.")
            return
        else:
            self.sayTo(user, "Translating url " + url + " with revision " + revision)
            translator = translate.QtLobbyStackTranslator()
            message = translator.translate(url, int(revision))
            self.sayTo(user, message)

    def Build(self, user, message):
        try:
            opts, args = getopt.gnu_getopt(message.split(), "h", ["profile=", "revision=", "help"])
        except getopt.GetoptError, err:
            self.sayTo(user, err)
            return

        for o, a in opts:
            if o == "--profile":
                self.profile = a
            if o == "--revision":
                self.revision = a
            elif o in ("-h", "--help"):
                self.SendHelp(user)
                return
        
        if not self.profile in self.config['buildbot']:
            self.sayTo(user, "Could not find profile in config file.")
            return
        if self.config['buildbot'][self.profile]['builddir'] == "rpm":
            self.sayTo(user, "Rpm packaging not implemented.")
            return
        else:
            if self.builder.isAlive() == True:
                self.sayTo(user, "A build process is already running.")
                return
            else:
                try:
                    self.builder = build.QtLobbyBuilder(self.config['buildbot'][self.profile]['builddir'], self.onMessage, self.onCompleted)
                    self.employer = user
                    self.builder.dir = self.config['buildbot'][self.profile]['builddir']
                    self.builder.revision = self.revision
                    self.builder.start()
                    self.sayTo(user, "Building QtLobby with profile " + self.profile + " on revision " + self.revision)
                except Exception, err:
                    self.sayTo(user, "A build is already in progress.")
    
    def onMessage(self, msg):
        self.sayTo(self.employer, msg)
    
    def onCompleted(self, links):
        self.sayTo(self.employer, "Build complete.")
        for k in links.keys():
            self.sayTo(self.employer, k + ": " + links[k])
        return
        
    def sayTo(self, user, message):
        if self.reportToChannel == "":
            self.client.sock.send ("SAYPRIVATE %s %s \n"% (user, message) )
        else:
            self.client.sock.send ("SAY %s %s \n"% (self.reportToChannel, message) )
        
    def SendHelp(self, user):
        self.sayTo(user, "Help:")
        self.sayTo(user, "-h --help Displays this help text.")
        self.sayTo(user, "!build")
        self.sayTo(user, "     --profile The build profile.")
        self.sayTo(user, "     --revision The targeted revision.")
        self.sayTo(user, "!translate")
        self.sayTo(user, "     --url The url where to download the stacktrace from.")
        self.sayTo(user, "     --revision The targeted subversion revision.")
