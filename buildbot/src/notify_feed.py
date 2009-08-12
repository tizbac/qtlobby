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
import feedparser

class Main:
    operator = []
    feeds = []
    broadcastChannels = []
    displayedFeedIDs = []
    configSection = 'notify_feed'

    def onload(self,tasc):
	self.client = tasc
        self.channels = self.client.main.ph.plugins['channels']
	# config
	self.config = configobj.ConfigObj("FlexBot.conf")
	self.config.list_values = True
	self.feeds = self.config[self.configSection]['feeds']
	self.broadcastChannels = self.config[self.configSection]['broadcast']
        self.displayedFeedIDs.extend(self.config[self.configSection]['displayedFeedIDs'])
    
    def onloggedin(self, socket):
	for chan in self.broadcastChannels:
            self.channels.JoinChannel(chan)

    def onpong(self):
        changed = False
        for feed in self.feeds:
            d = feedparser.parse(feed)
            for entrie in d.entries:
                if not entrie.id in self.displayedFeedIDs:
                    self.displayedFeedIDs.append(entrie.id)
                    changed = True
                    for chan in self.broadcastChannels:
                        self.sendChannelEX(chan, entrie.title + " by " + entrie.author)
			self.sendChannelEX(chan, entrie.link)
			self.sendChannelEX(chan, "PM !build to me to get windows installer")
        if changed == True:
            self.config[self.configSection]['displayedFeedIDs'] = self.displayedFeedIDs
            self.config.write()

    def sendChannelEX(self, channel, message):
        self.client.sock.send ("SAYEX %s %s \n"% (channel, message) )
