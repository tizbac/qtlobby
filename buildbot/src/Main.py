import os
import sys
import string
import base64
import hashlib
import time
import ParseConfig
import thread
import Plugin
import traceback
from colors import *
import Client
import binascii
class MainApp:
	firstconnect = 1
	er = 0
	configfile = ""
	reg = False
	connected = False
	def PingLoop(self):
		while self.er == 0:
			self.tasclient.ping()
			time.sleep(10)
		raise SystemExit(0)	
	def onlogin(self,socket):
		if self.firstconnect == 1:
			thread.start_new_thread(self.tasclient.mainloop,())
			thread.start_new_thread(self.PingLoop,())
			self.firstconnect = 0
		#self.tasclient.events.onconnected = self.ph.onconnected 
		#self.tasclient.events.ondisconnected = self.ph.ondisconnected 
		
		self.tasclient.events.onmotd = self.ph.onmotd  
		self.tasclient.events.onsaid = self.ph.onsaid 
		self.tasclient.events.onsaidex = self.ph.onsaidex 
		self.tasclient.events.onsaidprivate = self.ph.onsaidprivate 
		self.tasclient.events.onpong = self.ph.onpong 
		self.tasclient.events.oncommandfromserver = self.ph.oncommandfromserver
		self.ph.onloggedin(socket)
		self.ph.oncommandfromserver("ACCEPTED",[],self.tasclient.sock)
		self.connected = True
		good("Logged in")
	def SaveConfig(self):
		ParseConfig.writeconfigfile(self.configfile,self.config)
	def Dologin(self):
		if self.tasclient.fl.register:
			notice("Not logging in because a registration is in progress")
			return
		if self.verbose:
			notice("Logging in...")
		m = hashlib.md5()
		m.update(self.config["password"])
		phash = base64.b64encode(binascii.a2b_hex(m.hexdigest()))
		self.tasclient.login(self.config["nick"],phash,"Newbot",2400)
	def Register(self,username,password):
		m = md5.new()
		m.update(self.config["password"])
		self.tasclient.register(self.config["nick"],base64.b64encode(binascii.a2b_hex(m.hexdigest())))
	def destroy(self):
		self.tasclient.er = 1
		self.er = 1
		raise SystemExit(0)
	def ReloadConfig(self):
		
		self.config = ParseConfig.readconfigfile(self.configfile)
		self.admins = ParseConfig.parselist(self.config["admins"],",")
	def run(self,configfile,register=False,verbose=False):
		self.cwd = os.getcwd()
		self.ph = Plugin.plghandler(self)
		self.configfile = configfile
		self.config = ParseConfig.readconfigfile(configfile)
		self.admins = ParseConfig.parselist(self.config["admins"],",")
		self.verbose = verbose
		self.tasclient = Client.tasclient(self)
		
		for p in ParseConfig.parselist(self.config["plugins"],","):
			self.ph.addplugin(p,self.tasclient)
		
		
		self.tasclient.events.onconnected = self.Dologin
		self.tasclient.events.onloggedin = self.onlogin
		self.reg = register
		notice("Connecting to %s:%i" % (self.config["serveraddr"],int(self.config["serverport"])))
		self.tasclient.connect(self.config["serveraddr"],int(self.config["serverport"]))

			
inst = MainApp()
cf = "Main.conf"
i = 0
r = False
try:
	for arg in sys.argv:
		if arg.strip() == "-c":
			cf = sys.argv[i+1]
		if arg.strip() == "-r":
			r = True
			notice("Registering account")
		i += 1
	inst.run(cf,r,True)
	while 1:
		time.sleep(10)
except SystemExit:
	raise SystemExit(0)
except KeyboardInterrupt:
	error("SIGINT, Exiting")
	exit(0)
except:
	error("parsing command line")
	print('-'*60)
	traceback.print_exc(file=sys.stdout)
	print ('-'*60)
	
