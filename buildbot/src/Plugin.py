from colors import *
import sys
import traceback
import inspect
import ctypes
def _async_raise(tid, exctype):
    '''Raises an exception in the threads with id tid'''
    if not inspect.isclass(exctype):
        raise TypeError("Only types can be raised (not instances)")
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, ctypes.py_object(exctype))
    #if res == 0:
    #    raise ValueError("invalid thread id")
    if res != 1:
        # """if it returns a number greater than one, you're in trouble, 
        # and you should call it again with exc=NULL to revert the effect"""
        ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, 0)
        
class plghandler:
	plugins = dict()
	pluginthreads = dict()
	app = ""
	def __init__(self,main):
		self.app = main
	def addplugin(self,name,tasc):
		if name in self.plugins:
			bad("Plugin %s is already loaded" % name)
			return
		try:
			code = __import__(name)
		except:
			print ('-'*60)
			traceback.print_exc(file=sys.stdout)
			print ('-'*60)
			error("Cannot load plugin   "+name)
			return
		
		self.plugins.update([(name,code.Main())])
		self.pluginthreads.update([(name,[])])
		
		self.plugins[name].threads = self.pluginthreads[name]
		self.plugins[name].sock = tasc.sock
		#print "Pluging %s has %s functions" % (name,str(dir(self.plugins[name])))
		
		try:
			if "onload" in dir(self.plugins[name]):
				self.plugins[name].onload(tasc)
			if "onloggedin" in dir(self.plugins[name]) and self.app.connected:
				self.plugins[name].onloggedin(tasc.sock)
		except:
			error("Cannot load plugin   "+name)
			print ('-'*60)
			traceback.print_exc(file=sys.stdout)
			print ('-'*60)
			return
		loaded("Plugin " + name)
	def unloadplugin(self,name):
		if not name in self.plugins:
			error("Plugin %s not loaded"%name)
			return
		try:
			if "ondestroy" in dir(self.plugins[name]):
				self.plugins[name].ondestroy()
			notice("Killing any threads spawned by the plugin...")
			for tid in self.pluginthreads[name]:
				_async_raise(tid,SystemExit)
			self.pluginthreads.pop(name)
			self.plugins.pop(name)
			notice("%s Unloaded" % name)
		except:
			error("Cannot unload plugin   "+name)
			error("Use forceunload to remove it anyway")
			print ('-'*60)
			traceback.print_exc(file=sys.stdout)
			print ('-'*60)
	def forceunloadplugin(self,name,tasc):
		if not name in self.plugins:
			error("Plugin %s not loaded"%name)
			return
		self.plugins.pop(name)
		bad("%s Unloaded(Forced)" % name)
	def reloadplugin(self,name):
		if not name in self.plugins:
			error("Plugin %s not loaded"%name)
			return
		try:
			if "ondestroy" in dir(self.plugins[name]):
				self.plugins[name].ondestroy()
			notice("%s Unloaded" % name)
		except:
			error("Cannot unload plugin   "+name)
			error("Use forceunload to remove it anyway")
			print ('-'*60)
			traceback.print_exc(file=sys.stdout)
			print ('-'*60)
		try:
			code = reload(sys.modules[name])
		except:
			error("Cannot reload plugin %s!" % name)
			return
		notice("Killing any threads spawned by the plugin...")
		for tid in self.pluginthreads[name]:
			_async_raise(tid,SystemExit)
		self.plugins.update([(name,code.Main())])
		self.pluginthreads.update([(name,[])])
		self.plugins[name].threads = self.pluginthreads[name]
		self.plugins[name].sock = self.app.tasclient.sock
		#print "Pluging %s has %s functions" % (name,str(dir(self.plugins[name])))
		
		try:
			if "onload" in dir(self.plugins[name]):
				self.plugins[name].onload(self.app.tasclient)
		except:
			error("Cannot load plugin   "+name)
			print ('-'*60)
			traceback.print_exc(file=sys.stdout)
			print ('-'*60)
			return
		loaded("Plugin " + name)
	def onconnected(self):
		for plugin in self.plugins:
			try:
				if "onconnected" in dir(self.plugins[plugin]):
					self.plugins[plugin].onconnected()
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def ondisconnected(self):
		for plugin in self.plugins:
			try:
				if "ondisconnected" in dir(self.plugins[plugin]):
					self.plugins[plugin].ondisconnected()
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def onmotd(self,content):
		for plugin in self.plugins:
			try:
				if "onmotd" in dir(self.plugins[plugin]):
					self.plugins[plugin].onmotd(content)
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def onsaid(self,channel,user,message):
		for plugin in self.plugins:
			try:
				if "onsaid" in dir(self.plugins[plugin]):
					self.plugins[plugin].onsaid(channel,user,message)
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def onsaidex(self,channel,user,message):
		for plugin in self.plugins:
			try:
				if "onsaidex" in dir(self.plugins[plugin]):
					self.plugins[plugin].onsaidex(channel,user,message)
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def onsaidprivate(self,user,message):
		args = message.split(" ")
		if args[0].lower() == "!reloadconfig" and user in self.app.admins:
			self.app.ReloadConfig()
		if args[0].lower() == "!unloadplugin" and user in self.app.admins and len(args) == 2:
			try:
				self.unloadplugin(args[1])
			except:
				bad("Unloadplugin failed")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
		if args[0].lower() == "!loadplugin" and user in self.app.admins and len(args) == 2:
			try:
				self.addplugin(args[1],self.app.tasclient)
			except:
				bad("addplugin failed")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
		if args[0].lower() == "!reloadplugin" and user in self.app.admins and len(args) == 2:
			try:
				self.reloadplugin(args[1])
			except:
				bad("Unloadplugin failed")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
		for plugin in self.plugins:
			try:
				if "onsaidprivate" in dir(self.plugins[plugin]):
					self.plugins[plugin].onsaidprivate(user,message)
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def onloggedin(self,socket):
		
		for plugin in self.plugins:
			
			try:
				
				if "onloggedin" in dir(self.plugins[plugin]):
					self.plugins[plugin].onloggedin(socket)
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def onpong(self):
		for plugin in self.plugins:
			try:
				if "onpong" in dir(self.plugins[plugin]):
					self.plugins[plugin].onpong()
			except SystemExit:
				raise SystemExit(0)
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
	def oncommandfromserver(self,command,args,socket):
		
		for plugin in self.plugins:
			try:
				if "oncommandfromserver" in dir(self.plugins[plugin]):
					
					self.plugins[plugin].oncommandfromserver(command,args,socket)
					
			except SystemExit:
				raise SystemExit(0)	
			except:
				error("PLUGIN ERROR")
				print ('-'*60)
				traceback.print_exc(file=sys.stdout)
				print ('-'*60)
