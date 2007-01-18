#!/usr/bin/env python

import os,sys
from xmlrpclib import Server

username = 'george'
api_key = '74f5cba1954e8378c8a7cf930c7cdb991798b2c2'
feed_url = 'http://sudarkoff.com/tracks/feed/text/george/74f5cba1954e8378c8a7cf930c7cdb991798b2c2'

# some hardcoded contexts - tracks API is pretty spartan at the moment
context = {'home': 1, 'office': 2, 'online': 4, 'call': 6, 'waiting': 10, 'someday': 11, 'errands': 13}

server = Server('http://sudarkoff.com:80/tracks/backend/api')

if len(sys.argv) > 2:
    if sys.argv[1] != 'list':
    	server.NewTodo(username, api_key, str(context[sys.argv[1]]), sys.argv[2])
    else:
        os.system('curl %s?context=%d' % (feed_url, context[sys.argv[2]]))
else:
	print "Usage:\n\t$ todo <context> \'description\'\n\t$ todo list <context>\n\nContexts:\n\t%s" % "\n\t".join(context)
