import sys
import time

a = 0
for x in range (0,3):
    a = a + 1
    b = ("Loading" + "." * a)
    # \r prints a carriage return first, so `b` is printed on top of the previous line.
    sys.stdout.write('\r'+b)
    time.sleep(0.5)
print (a)

# from __future__ import print_function
# import time
# for x in range (0,5):
#    b = "Loading" + "." * x
#    print (b, end="\r")
#    time.sleep(1)
