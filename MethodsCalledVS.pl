#!/usr/bin/perl
#
########################
#
# DISCLAIMER! This script is not realy-realy smart, so it doesn't 
# care about objects' scopes and stuff, be very carefull to use 
# its results.
#
# Author: George_Sudarkov@amat.com
#

###
#
## Global variables
#
$results_path = ".";
$classes_path = "Z:\\yes\\src\\include\\".$ARGV[0];
#@objects_path = ("Z:\\yes\\src\\include\\KLAExport", "Z:\\yes\\src\\KLAExportDll");
$i=1;
while (defined($ARGV[$i])) {
  push (@objects_path, $ARGV[$i++]);
}

# this array is being used to show job progress
@progress = qw(\ | / -);

#open (LOG, ">$results_path\\methods_called.log") || die "ERROR: $!";

###
#
## Get the list of classes we want to look for
#
sub get_classes {
#  print LOG "-------------------- get_classes --------------------\n";
  opendir (CLASSES, $classes_path) || die "ERROR: $!";
  while ($classes = readdir (CLASSES)) {
    chomp ($classes);
    if ($classes =~ /\w+\.(cpp|h)/) {
      open (FILE, "$classes_path\\$classes") || die "ERROR: $!";
      while (<FILE>) {
        chomp ($_);
        if ($_ =~ /^\s*\bclass\b\s+\w*\s+(\w+)/) {
          unless($i{$1}++) {
            push (@found_classes, $1);
#            print LOG "$classes_path\\$classes ($.): $1\n";
          }
        }
      }
      close (FILE);
    }
  }
  closedir (CLASSES);
  return @found_classes;
}

###
#
## Get the list of objects instantiated from our classes
#
sub get_objects {
  @classes = @_;
#  print LOG "-------------------- get_objects --------------------\n";
  print ("Collecting objects. This may take a while; please, be patient...\n");
  foreach $path (@objects_path) {
    opendir (OBJECTS, $path) || die "ERROR: $!";
    while ($objects = readdir (OBJECTS)) {
      chomp ($objects);
      if ($objects =~ /\w+\.(cpp|h)/) {
        open (FILE, "$path\\$objects") || die "ERROR: $!";
        while (<FILE>) { # for all lines in file
          chomp;
          foreach $class (@classes) { # for all classes
            # check if the object of this class is declared
            if ($_ =~ /\b$class\b( |\t|\*)*(\w*::)?(\w+)/) { 
              unless($i{$+}++) {
                push (@found_objects, $+);
#                print LOG "$path\\$objects ($.): $+\n";
              }
            }
          }
        }
        close (FILE);
      }
    }
  }
  return @found_objects;
}

###
#
## Get the list of methods
#
sub get_methods {
  @objects = @_;
  $i=0;
  print ("Listing found methods:\n");
#  print LOG "-------------------- get_methods --------------------\n";
  foreach $path (@objects_path) {
    opendir (OBJECTS, $path) || die "ERROR: $!";
    while ($file = readdir (OBJECTS)) {
      chomp ($file);
      if ($file =~ /\w+\.(cpp|h)/) {
        open (FILE, "$path\\$file") || die "ERROR: $!";
        while (<FILE>) { # for all lines in file
          chomp;
          foreach $object (@objects) { # for all objects
            # find methods, called for that objects
#            print (LOG "$path\\$file ($.): looking for $object\n"); # debug print
            if ($_ =~ /\b$object\b(.|->| )+(\w+)\((\w+|\W+)+(\)|$)/) { 
              s/\s+/ /g;
              s/^ //g;
              print ("$path\\$file ($.): $object IN $_\n");
              $i++;
            }
          }
        }
        close (FILE);
      }
    }
  }
  return $i;
}

###
#
## MAIN
#
print ("-------------------- Configuration: $classes_path --------------------\n");
$found = &get_methods(&get_objects(&get_classes()));
print ("\n$found method(s) found\n");
