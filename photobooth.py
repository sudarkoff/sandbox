#!/usr/bin/python
#
# iSight Photo Booth for the Gnome Desktop
# powered by GStreamer, Python and GTK :)
#

# Copyright (c) 2007 Thomas Perl <thp@perli.net>
# Wed, 18 Apr 2007 21:47:13 +0200

# Fun stuff:
# for i in *.jpg; do convert "$i" -resize 120x120 "$i"; done
# convert -delay 10 -loop 0 *.jpg animation.gif


import gst
import gtk
import os.path

class PhotoBooth(gtk.Window):
    def __init__( self):
        gtk.Window.__init__( self)

        self.set_title( 'iSight Photo Booth (Python/GTK)')
        self.set_default_size( 500, 400)
        self.set_icon_name('camera-web')

        self.current_snapshot = 0

	vbox = gtk.VBox()
	self.add( vbox)

        hbox = gtk.HBox()
        vbox.pack_start( hbox, False)

	self.movie_window = gtk.DrawingArea()
        self.movie_window.set_size_request( 640, 480)
	hbox.add( self.movie_window)

        preview_vbox = gtk.VBox()
        self.images = []
        self.pixbufs = []
        for i in range(6):
            self.pixbufs.append( None)
            self.images.append( gtk.Image())
            preview_vbox.pack_start( self.images[-1])
        hbox.pack_start( preview_vbox)

	hbox = gtk.HBox()
        hbox.set_border_width( 6)
        hbox.set_spacing( 6)
	vbox.pack_start( hbox, False)

        hbox.pack_start( gtk.Label(' '))

        self.button2 = gtk.Button('Snapshot')
        self.button2.set_image( gtk.image_new_from_icon_name( 'camera', gtk.ICON_SIZE_BUTTON))
        self.button2.connect("clicked", self.screenshot)
        hbox.pack_start( self.button2, False)

        self.button3 = gtk.Button('Save Photos')
        self.button3.set_image( gtk.image_new_from_icon_name( gtk.STOCK_SAVE, gtk.ICON_SIZE_BUTTON))
        self.button3.connect("clicked", self.save_shots)
        hbox.pack_start( self.button3, False)

        hbox.pack_start( gtk.Label(' '))

        self.isight = gst.parse_launch('v4l2src ! ffmpegcolorspace ! ximagesink')
        bus = self.isight.get_bus()
        bus.add_signal_watch()
        bus.enable_sync_message_emission()
        bus.connect( 'sync-message::element', self.on_sync_message)

        self.connect('destroy', self.quit)
        self.show_all()

        self.start_stop( None)

        gtk.gdk.threads_init()
        gtk.main()

    def image_clicked( self, widget, event, id):
        print id

    def quit( self, widget):
        self.isight.set_state( gst.STATE_NULL)
        gtk.main_quit()

    def start_stop( self, widget):
        if self.isight.get_state()[1] == gst.STATE_PLAYING:
            self.isight.set_state( gst.STATE_NULL)
        else:
            self.isight.set_state(gst.STATE_PLAYING)

    def screenshot( self, widget):
        pixbuf = gtk.gdk.Pixbuf( gtk.gdk.COLORSPACE_RGB, False, 8, 640, 480)
        pixbuf.get_from_drawable( self.movie_window.window, self.movie_window.get_colormap(), 0, 0, 0, 0, 640, 480)
        self.pixbufs[self.current_snapshot] = pixbuf
        self.images[self.current_snapshot].set_from_pixbuf( pixbuf.scale_simple( 640/len(self.images), 480/len(self.images), gtk.gdk.INTERP_BILINEAR))
        self.current_snapshot += 1
        if self.current_snapshot >= len(self.images):
            self.current_snapshot = 0

    def save_shots( self, widget):
        for i in range(len(self.pixbufs)):
            if self.pixbufs[i] != None:
                self.pixbufs[i].save( os.path.expanduser( '~/Desktop/PhotoBooth %d.jpg' % ( i+1, )), 'jpeg', {'quality':'100'})

    def on_sync_message( self, bus, message):
       	if message.structure is None:
	    return

	message_name = message.structure.get_name()

	if message_name == 'prepare-xwindow-id':
		imagesink = message.src
		imagesink.set_property('force-aspect-ratio', True)
		imagesink.set_xwindow_id( self.movie_window.window.xid)

PhotoBooth()
