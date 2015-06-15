#!/usr/bin/env python
import itertools
from itertools import combinations
import sys

# input comes from STDIN (standard input)
for line in sys.stdin:
      # remove leading and trailing whitespace
      line = line.strip()
      # split the line into words
      nums = line.split()

      # get a sublist of all the friends
      first = nums[0]
      friends = nums[1:len(nums)]

      # find all combinations of friends
      for x in range(0,len(friends)):
          for y in range(x+1,len(friends)):
          # write the results to STDOUT (standard output);
          # what we output here will be the input for the
          # Reduce step, i.e. the input for reducer.py
          #
          # tab-delimited; the trivial word count is 1
              temp = [friends[x],friends[y]]
              temp.append(first)
              temp.sort()
              print '%s\t%s' % (temp, 1)
