#!/usr/bin/python

import sys 
import getopt
from commands import getoutput
import subprocess
import time

def main(argv):
   inputfile = ""
   try:
      opts, args = getopt.getopt(argv,"hi:o:",["ifile="])
   except getopt.GetoptError:
      print "test.py -i <inputfile>"
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print "test.py -i <inputfile>"
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
   print "COMPILING: ", inputfile
   result = subprocess.call(["../Assembler_c++/main", inputfile])
   print "Result", result
   if result != 0:
      print "COMPILATION FAILED. EXITING."
      exit(1)
   compiledProgram = ""
   dotIndex = inputfile.index(".")
   compiledProgram = inputfile[0:dotIndex]
   compiledProgram += ".bcm"
   time.sleep(1)
   result = subprocess.call(["../main", compiledProgram, "debug"])
   if result != 0:
		 print "EXECUTION FAILED. EXITING."
		 exit(1)
   exit(0)
   
   
if __name__ == "__main__":
   main(sys.argv[1:])
