#!/usr/bin/python

import math
import random
import threading

class SqrtThread(threading.Thread):
	def run(self):
		while True:
			math.sqrt(random.random())

for x in xrange(20):				
	SqrtThread().start()
