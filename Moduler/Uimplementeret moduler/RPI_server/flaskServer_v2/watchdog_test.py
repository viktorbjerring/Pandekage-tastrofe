# HEAVY INSPIRATION: https://www.geeksforgeeks.org/create-a-watchdog-in-python-to-look-for-filesystem-changes/

################ IMPORTANT:
# When editing files in visual studio code, it interrupts twice. Use Nano or something else.

import watchdog.events 
import watchdog.observers 
import time 


class Handler(watchdog.events.PatternMatchingEventHandler): 
	def __init__(self, src, fileName): 
		# Set the patterns for PatternMatchingEventHandler 
		fileName = '*'+fileName # Necessary to make it a pattern...
		observer = watchdog.observers.Observer()
		observer.schedule(self, path=src, recursive=True) 
		observer.start()
		watchdog.events.PatternMatchingEventHandler.__init__(self, patterns=[fileName], 
                                                             ignore_directories=True, case_sensitive=False) 

	#def on_created(self, event): 
		#print("Watchdog received created event - % s." % event.src_path) 
		# Event is created, you can process it now 

	def on_modified(self, event): 
		print("Watchdog received modified event - % s." % event.src_path)
		# Event is modified, you can process it now 


if __name__ == "__main__": 
	src_path = r"/home/stud/Pandekage-tastrofe/Moduler/Uimplementeret moduler/RPI_server/flaskServer_v2/"
	event_handler = Handler(src_path,'panController.txt') 
	time.sleep(1)
	del event_handler
	try: 
		while True:
			time.sleep(1) 
			print("Hej")
	except KeyboardInterrupt: 
		#observer.stop() 
		pass
