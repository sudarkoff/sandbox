import math

length_of_the_necklace = int(raw_input("Desired length of the necklace (inches): "))
number_of_bead_types = int(raw_input("Number of different beads in the pattern: "))

beads = []
pattern_length = 0.0
for i in range(number_of_bead_types):
    bead_number = int(raw_input("Number of bead %i in the pattern: " % (i+1)))
    bead_size = float(raw_input("Size (diameter) of the bead %i (mm): " % (i+1)))
    bead_size = bead_size * 0.03937 # convert to inches
    beads.append((bead_number, bead_size))
    pattern_length = pattern_length + bead_number * bead_size
number_of_patterns = math.ceil(length_of_the_necklace / pattern_length)
for i in range(number_of_bead_types):
    strands = math.ceil((number_of_patterns * beads[i][0] * beads[i][1]) / 16)
    print "You need %f strands of bead %i" % (int(strands), i+1)
