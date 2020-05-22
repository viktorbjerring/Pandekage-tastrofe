import watchdog.events 
import watchdog.observers 
import time 
import os
mainPath = os.getcwd() # Set the OS working directory (mainPath is uses in os)

class Handler(watchdog.events.PatternMatchingEventHandler): 
	def __init__(self, src, fileName): 
		# Set the patterns for PatternMatchingEventHandler 
		self.__fileName = fileName
		self.__lastModified = 0
		fileNamePath = '*'+fileName # Necessary to make it a pattern...
		observer = watchdog.observers.Observer()
		observer.schedule(self, path=src, recursive=True) 
		observer.start()
		watchdog.events.PatternMatchingEventHandler.__init__(self, patterns=[fileNamePath], 
                                                             ignore_directories=True, case_sensitive=False) 

	#def on_created(self, event): 
		#print("Watchdog received created event - % s." % event.src_path) 
		# Event is created, you can process it now 

	def on_modified(self, event): 
		statbuf = os.stat(self.__fileName)
		newEvent = statbuf.st_mtime
		if(newEvent - self.__lastModified) > 0.5:
			print("Watchdog received modified event - % s." % event.src_path)
		self.__lastModified = newEvent
		# Event is modified, you can process it now 