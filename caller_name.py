#!/usr/bin/python

import sys
import inspect

def functionName():
    """Function Name does blah."""
    worker()

def worker():
    func = inspect.getmembers(inspect.stack()[1])
    print func
    print "Called by: %s: %s" % (func.__name__, func.__doc__)

if __name__ == "__main__":
    functionName()
    sys.exit(0)

