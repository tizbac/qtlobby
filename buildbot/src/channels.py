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

class Main:
	joinedChannels = []
	operator = []
	config = configobj.ConfigObj("FlexBot.conf")
	
	def onloggedin(self, socket):
		joinChan = self.config['channels']['join']
		for channel in joinChan:
			self.JoinChannel(channel)
			self.joinedChannels.append(channel)

	def onsaidprivate(self,user,message):
		if user in self.operator == False:
			return
		args = message.split()
		if "!chan.join" in args and len(args) > 1:
			for count in range(1, len(args)):
				self.JoinChannel(args[count])
				self.joinedChannels.append(args[count])
		elif "!chan.leave" in args and len(args) > 1:
			for count in range(1, len(args)):
				self.LeaveChannel(args[count])
				self.joinedChannels.remove(args[count])
		elif "!chan.list" in args:
			joinedChan = ""
			for channel in self.joinedChannels:
				joinedChan += " " + channel
			self.SayPrivate(user, "I'm joined in following channels:" + joinedChan)

	def onload(self, tasc):
		self.client = tasc
		self.config.list_values = True
		self.operator = self.config['channels']['operator']

	def SayPrivate(self, user, message):
		self.client.sock.send ("SAYPRIVATE %s %s \n"% (user, message) )
	
	def JoinChannel(self, channel):
		self.client.sock.send ("JOIN %s\n" % (channel))        
		
	def LeaveChannel(self, channel):
		self.client.sock.send("LEAVE %s\n" % (channel))