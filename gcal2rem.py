__author__ = 'george@sudarkoff.com (George Sudarkoff)'

from xml.etree import ElementTree
import gdata.calendar.service
import gdata.service
import atom.service
import gdata.calendar
import atom

import getopt, sys, string, time, urllib, re
from datetime import *
from dateutil.tz import *
from dateutil.parser import *
from time import *

VERSION='0.1'

class GCal2Rem:
    def __init__(self, email, password, calendar_name):
        self.calendar_name = calendar_name
        self.cal_client = gdata.calendar.service.CalendarService()
        self.cal_client.email = email
        self.cal_client.password = password
        self.cal_client.source = 'GCal2Rem-' + VERSION
        self.cal_client.ProgrammaticLogin()

    def _GetCalendar(self):
        feed = self.cal_client.GetAllCalendarsFeed()
        for i, a_calendar in zip(xrange(len(feed.entry)), feed.entry):
            if a_calendar.title.text == self.calendar_name:
                match = re.match('^http://www.google.com/calendar/feeds/(.*?)/(.*?)/(.*)$',
                                 a_calendar.GetAlternateLink().href)
                a_calendar.gcal_username    = urllib.unquote(match.group(1))
                a_calendar.gcal_visibility  = urllib.unquote(match.group(2))
                a_calendar.gcal_projection  = urllib.unquote(match.group(3))
                return a_calendar
        return None

    def _DateRangeQuery(self, cal, start_date, end_date):
        query = gdata.calendar.service.CalendarEventQuery(
            cal.gcal_username, cal.gcal_visibility, cal.gcal_projection)
        query.start_min = start_date
        query.start_max = end_date
        feed = self.cal_client.CalendarQuery(query)
        for an_event in feed.entry:
            for a_when in an_event.when:
                event_date = parse(a_when.start_time)
                print 'REM %s %s +7 MSG %s %%b (%%m %%d%%s).%%' % (
                    event_date.strftime("%b"), event_date.strftime("%d"), an_event.title.text )

    def Run(self, start_date, end_date):
        self._DateRangeQuery(self._GetCalendar(), start_date, end_date)

def main():
    # parse command line options
    try:
        opts, args = getopt.getopt(sys.argv[1:], "", ["user=", "pw=", "calendar="])
    except getopt.error, msg:
        print ('python calendarExample.py --user [username] --pw [password] --calendar [calendar name]')
        sys.exit(2)

    user = ''
    pw = ''
    calendar_name = ''

    # Process options
    for o, a in opts:
        if o == "--user":
            user = a
        elif o == "--pw":
            pw = a
        elif o == "--calendar":
            calendar_name = a

    if user == '' or pw == '':
        print ('python gcal2rem.py --user [username] --pw [password] --calendar [calendar name]')
        sys.exit(2)

    gcal2rem = GCal2Rem(user, pw, calendar_name)

    start_date = date.today()
    end_date = start_date + timedelta(days=8)
    gcal2rem.Run(start_date.strftime("%Y-%m-%d"), end_date.strftime("%Y-%m-%d"))

if __name__ == '__main__':
    main()
