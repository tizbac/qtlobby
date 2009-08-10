from socket import *
import string
import re
import time
import utilities
from colors import *
import sys
import traceback
def parsecommand(cl,c,args,events,sock):
	if c.strip() != "":
		events.oncommandfromserver(c,args,sock)
		if c == "TASSERVER":
			good("Connected to server")
			
			if cl.fl.register:
				cl.register(cl.uname,cl.password)
				receive(cl,sock,events)
			else:
				events.onconnected()
		if c == "AGREEMENTEND":
			notice("accepting agreement")
			sock.send("CONFIRMAGREEMENT\n")
			cl.login(cl.uname,cl.password,"BOT",2000)
			events.onloggedin(sock)
		if c == "MOTD":
			events.onmotd(" ".join(args))
		if c == "ACCEPTED":
			events.onloggedin(sock)
		if c == "DENIED" and ' '.join(args).lower().count("already") == 0:
			error("Login failed ( %s ), trying to register..." % ' '.join(args))
			notice("Closing Connection")
			sock.close()
			cl.fl.register = True			
			cl.connect(cl.lastserver,cl.lastport)
			
		if c == "REGISTRATIONACCEPTED":
			good("Registered")
			notice("Closing Connection")
			sock.close()
			cl.fl.register = False
			cl.connect(cl.lastserver,cl.lastport)
		if c == "PONG":
			cl.lpo = time.time()
			events.onpong()
		if c == "SAIDPRIVATE" and len(args) >= 2:
			events.onsaidprivate(args[0],' '.join(args[1:]))
def receive(cl,socket,events): #return commandname & args
	buf = ""
	try:
		while not buf.strip("\r ").endswith("\n"):
			#print "Receiving incomplete command..."
			nbuf =  socket.recv(512)
			if nbuf == "":
				return 1
			buf += nbuf
	except:
		error("Connection Broken")
		return 1 # Connection broken
	commands = buf.strip("\r ").split("\n")
	for cmd in commands:
		c = cmd.split(" ")[0].upper()
		args = cmd.split(" ")[1:]
		parsecommand(cl,c,args,events,socket)
	return 0
class serverevents:
	def onconnected(self):
		good("Connected to TASServer")
	def ondisconnected(self):
		print (blue+"Disconnected"+normal)
	def onmotd(self,content):
		print (blue+"** MOTD ** "+content+normal)
	def onsaid(self,channel,user,message):
		print (cyan+"#"+channel+"\t<"+user+">"+normal+message)
	def onsaidex(self,channel,user,message):
		print (magenta+"#"+channel+"\t<"+user+">"+normal+message)
	def onsaidprivate(self,user,message):
		print (cyan+"$PRIVATE\t<"+user+">"+normal+message)
	def onloggedin(self,socket):
		print (blue+"Logged in."+normal)
	def onpong(self):
		#print blue+"PONG"+normal
		pass
	def oncommandfromserver(self,command,args,socket):
		#print yellow+"From Server: "+str(command)+" Args: "+str(args)+normal
		pass
class flags:
	norecwait = False
	register = False
	
class tasclient:
	sock = 0
	fl = flags()
	er = 0
	lp = 0.0
	lpo = 0.0
	def mainloop(self):
		while 1:
			if self.er == 1:
				raise SystemExit(0)
			try:
				
				#print "Waiting data from socket"
				result = receive(self,self.sock,self.events)
				#print "Received data"
				if result == 1:
					error("SERVER: Timed out, reconnecting in 40 secs")
					self.main.connected = False
					if not self.fl.norecwait:
						time.sleep(40.0)
						self.fl.norecwait = False
					try:
						self.sock.close()
					except:
						pass
					self.sock = socket(AF_INET,SOCK_STREAM)
					self.sock.settimeout(40)
					self.sock.connect((self.lastserver,int(self.lastport)))
					receive(self,self.sock,self.events)
					self.main.connected = True
			except SystemExit:
				raise SystemExit(0)
			except:
				print red+"Command Error"
				print '-'*60
				traceback.print_exc(file=sys.stdout)
				print '-'*60
	def __init__(self,app):
		self.events = serverevents()
		self.main = app
	def connect(self,server,port):
		self.lastserver = server
		self.lastport = port
		
		while 1:
			try:
				self.sock = socket(AF_INET,SOCK_STREAM)
				self.sock.settimeout(40)
				self.sock.connect((server,int(port)))
				if self.main.reg:
					notice("Registering nick")
					self.main.Register(self.main.config["nick"],self.main.config["password"])
				res = receive(self,self.sock,self.events)
				if not res == 1:
					return
			except SystemExit:
				raise SystemExit(0)
			except:
				self.main.connected = False
				error("Cannot connect, retrying in 40 secs...")
				print '-'*60
				traceback.print_exc(file=sys.stdout)
				print '-'*60
				if self.er == 1:
					raise SystemExit(0)
				time.sleep(40.0)
		
		
	def disconnect(self,hard=False):
		try:
			self.sock.send("EXIT\n")
		except:
			pass
		self.sock.close()
		self.sock = 0
	def login(self,username,password,client,cpu):
		notice("Trying to login with username %s " % (username))
		try:
			self.sock.send("LOGIN %s %s %i * %s\n" % (username,password,cpu,client))
		except:
			error("Cannot send login command")
		self.uname = username
		self.password = password
		receive(self,self.sock,self.events)
	def register(self,username,password):
		try:
			notice("Trying to register account")
			self.sock.send("REGISTER %s %s\n" % (username,password))
		except:
			error("Cannot send register command")
		
	def ping(self):
		if self.er == 1:
			return
		try:
			self.sock.send("PING\n")
			self.lp = time.time()
		except:
			error("Cannot send ping command")
	
			
		
