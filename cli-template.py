#!/usr/bin/env python
# encoding: utf-8
"""
cli-template.py

Created by George Sudarkoff on 2010-11-06.
Copyright (c) 2010 George Sudarkoff. All rights reserved.
"""

import os
import sys
import optparse
import glob
import inspect

parser = None
description = 'CLI utility template.'
epilog = 'Available commands:'


class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg

class InvalidOperation(Exception): pass

class RuntimeError(Exception):
    def __init__(self, msg):
        self.msg = msg


# -----------------------------------------------------------------------------
# Operations
# -----------------------------------------------------------------------------

class Operation(object):
    """
    Base class for operations.

    To define new operations, subclass Operation and implement
    an appropriate invoke() method.
    """
    options = ''
    usage = ''
    notes = ''

    def __init__(self, opts):
        self.name = self.__class__.name()
        self.options = opts

    @classmethod
    def name(cls):
        return cls.__name__.lower()

    @classmethod
    def args(operation):
        allArgsStr = inspect.formatargspec(
            *inspect.getargspec(operation.invoke))[1:-1]
        allArgsList = allArgsStr.split(', ')

        return ' '.join(['<%s>' % arg for arg in allArgsList
            if arg not in ['self', 'host', 'vm']])

def getOperationByName(opts, name):
    ## `name` must be a string
    if not hasattr(name, 'endswith'):
        return None

    opClass = opsDict.get(name, None)

    if opClass:
        return opClass(opts)
    else:
        return None

def getOperationFromArgs(opts, args):
    for arg in args:
        operation = getOperationByName(opts, arg)
        if operation: return operation

def processArgs(opts, args):
    return [arg for arg in args if not getOperationByName(opts, arg)]


class Help(Operation):
    notes = 'Display help.'

    def invoke(self):
        lines = []
        lines.append("\n  Available Commands:")

        parser.print_help()

        for key, opClass in opsDict.items():
            args = opClass.args()
            usageString = "%s %s" % (opClass.name(), opClass.usage or args)
            lines.append('    %-19s %s' % (usageString, opClass.notes))

        print '\n'.join(lines)


# -----------------------------------------------------------------------------
# Your class incapsulates whatever abstraction(s) you're working with
# -----------------------------------------------------------------------------
class YourClass(object):
    def __init__(self, some_param):
        self.some_param = some_param


# -----------------------------------------------------------------------------
# Your concrete operations
# -----------------------------------------------------------------------------
class Op1(Operation):
    notes = 'Perform some operation.'

    def invoke(self, op1_param):
        yourObj = YourClass(op1_param)
        # TODO: actually do something interesting


# -----------------------------------------------------------------------------
# Main
# -----------------------------------------------------------------------------

globalsDict = dict(globals())
opsDict = dict(
   [(key.lower(), val)
    for key, val in globalsDict.items()
    if hasattr(val, 'invoke')]
   )

def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    try:
        global parser
        parser = optparse.OptionParser("usage: %prog [options] command [arguments...]")
        parser.allow_interspersed_args = False
        parser.add_option("-v",
            action="store_true", dest="verbose",
            help="Verbose")

        outputFormatOptGroup = optparse.OptionGroup(parser, 'Output Format Options')
        outputFormatOptGroup.add_option(
            "--output-file",
            action="store", dest="output_file", metavar="OUTPUT",
            help="Output file name")
        outputFormatOptGroup.add_option(
           "--format",
           action="store", dest="format", metavar="FORMAT",
           type="choice", choices=['txt', 'html'],
           default='txt',
           help="Output format (txt, html)")
        parser.add_option_group(outputFormatOptGroup)

        opts, args = parser.parse_args(argv)

        # Perform requested operation
        operation = getOperationFromArgs(opts, args)
        if not operation:
            raise InvalidOperation()

        processedArgs = processArgs(opts, args)
        result = operation.invoke(*processedArgs)

    except Usage, err:
        sys.stderr.write("Usage error: %s\n\n" % str(err.msg))
        help = Help(opts)
        help.invoke()
        return 2

    except RuntimeError, err:
        sys.stderr.write("Runtime error: %s\n\n" % str(err.msg))
        return 3

    except InvalidOperation:
        sys.stderr.write("Invalid operation.\n")
        help = Help(opts)
        help.invoke()
        return 4

if __name__ == "__main__":
    sys.exit(main())
