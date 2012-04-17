#!/usr/bin/python
 
import xmlrpclib
server=xmlrpclib.Server("http://snipplr.com/xml-rpc.php")
snippet=server.snippet
print snippet.get("1273")
