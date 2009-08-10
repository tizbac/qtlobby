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
    isBusy = False
    profile = "debug"
    revision = "HEAD"
    
    def onload(self,tasc):
        self.client = tasc
        self.app = tasc.main
        # config
        self.config = configobj.ConfigObj("FlexBot.conf")
        self.config.list_values = True
        self.users = self.config['buildbot']['operator']

        
    def onsaidprivate(self,user,message):
        if user in self.operator == False:
            return
        elif message.startswith("!translate"):
            try:
                opts, args = getopt.gnu_getopt(message.split(), "u:r:h", ["url=", "revision=", "help"])
            except getopt.GetoptError, err:
                self.SayPrivate(user, err)
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

    def Translate(self, user, url, revision):
        if revision == "":
            self.SayPrivate(user, "You are missing a revision.")
            return
        elif url == "":
            self.SayPrivate(user, "You are missing an url.")
            return
        else:
            self.SayPrivate(user, "Translating url " + url + " with revision " + revision)
            translator = translate.QtLobbyStackTranslator()
            message = translator.translate(url, int(revision))
            self.SayPrivate(user, message)

    def Build(self, user, message):
        try:
            opts, args = getopt.gnu_getopt(message.split(), "h", ["profile=", "revision=", "help"])
        except getopt.GetoptError, err:
            self.SayPrivate(user, err)
            return;

        for o, a in opts:
            if o == "--profile":
                self.profile = a
            if o == "--revision":
                self.revision = a
            elif o in ("-h", "--help"):
                self.SendHelp(user)
                return
        
        if not self.profile in self.config['buildbot']:
            self.SayPrivate(user, "Could not find profile in config file.")
            return
        if self.config['buildbot'][self.profile]['builddir'] == "rpm":
            self.SayPrivate(user, "Rpm packaging not implemented.")
            return
        else:
            self.employer = user
            self.SayPrivate(user, "Building QtLobby with profile " + self.profile + " on revision " + self.revision)
            builder = build.QtLobbyBuilder(self.config['buildbot'][self.profile]['builddir'], self.onMessage, self.onCompleted)
            builder.revision = self.revision
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
        self.SayPrivate(user, "-h --help Displays this help text.")
        self.SayPrivate(user, "!build")
        self.SayPrivate(user, "     --profile The build profile.")
        self.SayPrivate(user, "     --revision The targeted revision.")
        self.SayPrivate(user, "!translate")
        self.SayPrivate(user, "     --url The url where to download the stacktrace from.")
        self.SayPrivate(user, "     --revision The targeted subversion revision.")