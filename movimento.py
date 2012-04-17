import sys
import Image, ImageDraw
import time
import pygame
from VideoCapture import Device
#import urllib2
#from cStringIO import StringIO

class ImageDiff(object):
    '''This class is inspired in the post of 
        http://gumuz.looze.net/wordpress/index.php/archives/2005/06/06/python-webcam-fun-motion-detection/
    '''
    def __init__(self, rows=2, cols=2, size = (320,240)):
        self.old = None
        self.new = None
        self.motion = None
        self.rows = rows
        self.cols = cols
        self.motion_factor = 10 # 10% of piece was changed to have a motion
        self.pixel_threshold = 50 # Yes, this was copied from link into diff docstring
        self.size = size

    def update(self, new):
        self.new, self.old = new, self.new

    def diff(self):
        '''This function returns a object of type Image with differences highlighted
        and a grid to see all blocks'''
        self.motion = self.new.copy() # Copy the new image to the motion                
        if not self.is_images_valid():
            self._draw_grid()
            return self.motion

        width, height = self.new.size
        piece_w = width/self.cols
        piece_h = height/self.rows
        start = time.time()
        for x in xrange(0, width, piece_w):
            for y in xrange(0, height, piece_h):
                box = (x, y, x+piece_w, y+piece_h)
                new_piece = self.new.crop(box)
                pieces = (
                            new_piece,
                            self.old.crop(box)
                )
                if self.has_motion(pieces):
                    #print 'Motion detected'
                    self.highlight(new_piece, box)
        end = time.time()
        print '%.3f seconds to detect motion'%(end-start)
        self._draw_grid()
        self.motion.transpose(0)
        return self.motion

    def highlight(self, piece, box):
        data = piece.getdata()
        new_data = []
        for r,g,b in data:
            new_data.append((r+60, g+20, b+50))
        piece.putdata(new_data)
        self.motion.paste(piece, box)

    def has_motion(self, pieces):
        '''Receive a piece of the bigger image and checks if are differences between both'''
        if len(pieces) != 2:
            raise ValueError, 'Pieces must be a tuple of 2 positions'
        p1 = pieces[0].getdata()
        p2 = pieces[1].getdata()
        pixel_count = len(p1)
        pixel_diff_count = 0.0
        for i in xrange(pixel_count): # Todo: Going 2 in 2 pixels have better performance?
            if abs(sum(p1[i]) - sum(p2[i])) > self.pixel_threshold:
                pixel_diff_count += 1
                ratio = pixel_diff_count / (pixel_count/100)
                if ratio > self.motion_factor:
                    return True
        return False
        
    def is_images_valid(self):
        '''Checks if diff can run in new and old images'''
        if not hasattr(self.new, 'size') or not hasattr(self.old, 'size'): # Someone knows a better way to detect if is a Image instance?
            # Both images must be of type Image
            sys.stderr.write('The new image or old image is invalid\n')
            return False

        if self.new.size != self.old.size:
            # Images to compare must have same size
            sys.stderr.write('Images must have same size')
            return False
        return True


    def _draw_grid(self):
        draw = ImageDraw.Draw(self.motion)
        width, height = self.motion.size
        piece_w = width/self.cols
        piece_h = height/self.rows
        
        grid_color = (100,100,120)
        for x in xrange(0, width, piece_w):
            draw.line((x, 0, x, height+x), fill=grid_color)

        for y in xrange(0, height, piece_h):
            draw.line((0, y, width, y), fill=grid_color)

# get cam device
cam = Device()

pygame.init()

width, height = 320, 240
size = (width, height)

screen = pygame.display.set_mode(size)

img_diff = ImageDiff(cols = 20, rows = 15, size=size)
im = None
stop = False

im = cam.getImage()
img_diff.update(im)

while not stop:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            stop = True
            sys.exit(0)

    # Update with the new image
    im = cam.getImage()
    img_diff.update(im)

    display = pygame.image.fromstring(img_diff.diff().tostring(), (320,240), "RGB")
    screen.blit(display, (0,0))
    pygame.display.update()
    pygame.display.flip()
    pygame.time.delay(1000)
