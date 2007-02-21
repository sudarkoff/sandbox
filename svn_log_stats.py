#!/usr/bin/env python

"""
Calculate and display some stats.

Author: George Sudarkoff <george@sudarkoff.com>

<?xml version="1.0" encoding="utf-8"?>
<log>
	<logentry revision="115">
		<author>george</author>
		<date>2007-02-01T06:49:20.804061Z</date>
		<paths>
			<path action="M">/Sandbox/FizzBuzz.py</path>
			<path action="A">/Sandbox/useful_main.py</path>
		</paths>
		<msg></msg>
	</logentry>
	...
</log>

Ruby implementation:
==== CUT ====
require 'enumerator'
require 'magic_xml'
require 'time'

class Numeric
    def time_pp
        s = self.to_i
        return "#{self}s" if s < 60

        m = (s / 60).to_i
        s -= m*60
        return "#{m}m#{s}s" if m < 60

        h = (m / 60).to_i
        m -= h*60
        return "#{h}h #{m}m#{s}s" if h < 24

        d = (h / 24).to_i
        h -= d*24
        return "#{d}d #{h}h #{m}m#{s}s"
    end
end

log = XML.parse(STDIN)

summaries_by_author = Hash.new{|ht,k| ht[k] = {:dates => [], :sizes => []}}

log.descendants(:logentry) {|e|
    summaries_by_author[e[:@author]][:dates] << Time.parse(e[:@date])
    summaries_by_author[e[:@author]][:sizes] << e.descendants(:path).size
}

summaries_by_author.to_a.sort.each{|author, summary|
    dates = summary[:dates].enum_for(:each_cons, 2).map{|a,b| a-b}.sort
    sizes = summary[:sizes].sort

    puts "Activity of #{author}:"
    puts "Time between commits distribution:"
    puts "* 10% - #{dates[dates.size/10].time_pp}"
    puts "* 25% - #{dates[dates.size/4].time_pp}"
    puts "* median - #{dates[dates.size/2].time_pp}"
    puts "* 75% - #{dates[dates.size*3/4].time_pp}"
    puts "* 90% - #{dates[dates.size*9/10].time_pp}"
    puts "Median number of affected files: #{sizes[sizes.size/2]}"

    sizes_summary = Hash.new(0)
    sizes.each{|sz| sizes_summary[sz] += 1}
    sizes_summary.to_a.sort.each{|k,v|
        puts "* #{k} file#{(k == 1) ? '' : 's'} - #{v} time#{(v == 1) ? '' : 's'}"
    }
}
==== CUT ====
"""

import sys
import traceback

from xml.dom.minidom import parse, parseString

def getText(nodelist):
    rc = ""
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + node.data
    return rc

class LogHandler:
	def __init__(self):
		pass
		
	def handleLogEntries(self, logentries):
		for logentry in logentries:
			self.handleLogEntry(logentry)
			
	def handleLogEntry(self, logentry):
		print "Revision: %s\n" % logentry.getAttribute('revision')
		self.handleEntryAuthor(logentry.getElementsByTagName('author')[0])
		self.handleEntryDate(logentry.getElementsByTagName('date')[0])
		self.handleEntryPaths(logentry.getElementsByTagName('paths')[0])
		self.handleEntryMsg(logentry.getElementsByTagName('msg')[0])
		
	def handleEntryAuthor(self, entryauthor):
		print "Author: %s\n" % getText(entryauthor.childNodes)
		
	def handleEntryDate(self, entrydate):
		pass
		
	def handleEntryPaths(self, entrypaths):
		for entrypath in entrypaths.getElementsByTagName('path'):
			self.handleEntryPath(entrypath)
		
	def handleEntryPath(self, entrypath):
		print "%s\t" % entrypath.getAttribute('action')
		print "%s\n" % getText(entrypath.childNodes)
	
	def handleEntryMsg(self, entrymsg):
		print "\"%s\"" % getText(entrymsg.childNodes)

def main():
	try:
		lines = sys.stdin.read()
		log = parseString(lines)
		root = log.childNodes[0]
		if root.nodeName != 'log':
			print >>sys.stderr, "Error: is it an svn xml log?\n"
		loghandler = LogHandler()
		loghandler.handleLogEntries(root.getElementsByTagName('logentry'))

	except:
		info = sys.exc_info()
		traceback.print_exc()
		return 2

if __name__ == "__main__":
    sys.exit(main())
