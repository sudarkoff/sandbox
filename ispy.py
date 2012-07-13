#!/usr/bin/env python
# encoding: utf-8

import objc, AppKit, Foundation
import os, sys, time, datetime

from PyObjCTools import AppHelper
from PIL import Image, ImageDraw

width, height = 320, 240
size = (width, height)

class ImageDiff(object):
    def __init__(self, rows=2, cols=2, size = size):
        self.old = None
        self.new = None
        self.rows = rows
        self.cols = cols
        self.image_threshold = 90 # % of piece changed
        self.pixel_threshold = 70 # % of pixel changed
        self.size = size

    def update(self, new):
        self.new, self.old = new.copy(), self.new

    def motion_detected(self):
        width, height = self.size
        piece_w = width/self.cols
        piece_h = height/self.rows
        for x in xrange(0, width, piece_w):
            for y in xrange(0, height, piece_h):
                box = (x, y, x + piece_w, y + piece_h)
                if self._diff(self.new.crop(box), self.old.crop(box)):
                    return True
                    
        return False

    def _diff(self, new_block, old_block):
        p1 = new_block.getdata()
        p2 = old_block.getdata()
        pixel_count = len(p1)
        pixel_diff_count = 0.0
        
        for i in xrange(pixel_count):
            if abs(sum(p1[i]) - sum(p2[i])) > self.pixel_threshold:
                pixel_diff_count += 1
                ratio = pixel_diff_count / (pixel_count/100)
                if ratio > self.image_threshold:
                    return True
                    
        return False
        
    def images_valid(self):
        # Both images must be of type Image # TODO: isinstance?
        if not hasattr(self.new, 'size') or not hasattr(self.old, 'size'):
            return False
        # Images must have the same size
        if self.new.size != self.old.size:
            return False
        return True

class Controller(NSObject):
    def init(self):
        self = super(Controller, self).init()
        if self is None: return None
        
        self.img_diff = ImageDiff(cols = (width/16), rows = (height/16), size = size)
        self.stamp = 0
        
        # Start frame grabbing
        self.camera = CSGCamera.alloc().init()
        self.camera.setDelegate_(self)
        self.camera.startWithSize_(size)

        return self

    # CSGCamera delegate
    # This method is called when the sequence grabber has a new frame (30 FPS)
    def camera_didReceiveFrame_(self, aCamera, aFrame):
        imageRep = aFrame.representations()[0] # aFrame is an NSImage
        im = Image.frombuffer("RGBA", size, imageRep.bitmapData())
        self.img_diff.update(im)
        
        # If we have images to compare and motion is detected
        if self.img_diff.images_valid() and self.img_diff.motion_detected():
            self.stamp += 1
            im.transpose(1).save("~/Pictures/ispy/snapshot_%s.png" % self.stamp)
            
        # Sleep for half a second, to reduce CPU utilization
        # (TODO: we should turn off the camera for this time)
        time.sleep(0.5)

c = Controller.alloc().init()
AppHelper.runConsoleEventLoop(installInterrupt=True)
