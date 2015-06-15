#!/usr/bin/env python

from operator import itemgetter
import sys
import re

current_word = None
current_count = 0
word = None

# input comes from STDIN
for line in sys.stdin:
      # remove leading and trailing whitespace
      line = line.strip()

      # parse the input we got from mapper.py
      word, count = line.split('\t', 1)

      # convert count (currently a string) to int
      try:
          count = int(count)
      except ValueError:
          # count was not a number, so silently
          # ignore/discard this line
          continue

      # this IF-switch only works because Hadoop sorts map output
      # by key (here: word) before it is passed to the reducer
      if current_word == word:
          current_count += count
      else:
          if current_word and current_count == 3:
              # write result to STDOUT
              nums = re.findall(r'\b\d+\b', current_word)
              first = nums[0]
              second = nums[1]
              third = nums[2]
              firstlead = [first, second, third]
              secondlead = [second, third, first]
              thirdlead = [third, first, second]
              print '%s' % (firstlead)
              print '%s' % (secondlead)
              print '%s' % (thirdlead)
          current_count = count
          current_word = word

# do not forget to output the last word if needed!
if current_word == word and current_count == 3:
    nums = re.findall(r'\b\d+\b', current_word)
    first = nums[0]
    second = nums[1]
    third = nums[2]
    firstlead = [first, second, third]
    secondlead = [second, third, first]
    thirdlead = [third, first, second]
    print '%s' % (firstlead)
    print '%s' % (secondlead)
    print '%s' % (thirdlead)
