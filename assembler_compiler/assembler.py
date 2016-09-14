#!/usr/bin/env python
# -*- coding: utf-8 -*- 

class ProgramContentsContainer:
	'FileContainer'

	def __init__(self, fileName):
		self.fileName = fileName

	def displayFileName(self):
		print "FileName : ", self.fileName

	def openFile(self):
		f = open(self.fileName, 'rU')
		self.programContents = f.read()
		
	def getContents(self):
		return self.programContents
		
	def setContents(self, newProgramContents):
		self.programContents = newProgramContents
		
class WhiteSpaceCleaner:
	'WhiteSpaceCleaner'
	
	def __init__(self, programContents):
		self.programContents = programContents
		
	def removeExtraSpaces(self):
		tmpString = ""
		comment = False
		for line in self.programContents.split('\n'):
			comment = False
			for word in line.split(' '):
				if word.startswith('#'):
					comment = True
				elif comment == False:
					tmpString += " " + word
				
		self.programContents = tmpString

		self.programContents = self.programContents.strip()
		self.programContents = " ".join(self.programContents.split())
		
	def getContents(self):
		return self.programContents

class Instruction:
	'Instruction'
	
	def __init__(self):
		self.label = ""
		
	def setLabel(self, label):
		self.label = label
		
	def setAddress(self, address):
		self.address = address
		
	def setInstruction(self, instruction):
		self.instruction = instruction
	
	def setParameter(self, parameter):
		self.parameter = parameter

class SyntaxParser:
	'SyntaxParser'
	
	def __init__(self, programContents):
		self.programContents = programContents
		
	def parse(self):
		for word in self.programContents.split():
			print word
			
class ProgramWriter:
	'ProgramWriter'
	
	def __init__(self, programContents, fileName):
		self.programContents = programContents
		self.fileName = fileName
		
	def writeBinary(self):
		outputFile = open(self.fileName, 'w')
		program = bytearray('\xff\xfe\xf1\xf0')
		outputFile.write(program)
		program = bytearray(self.programContents)
		outputFile.write(program)
		

def main(args):
	fc = ProgramContentsContainer(args[1])
	fc.displayFileName()
	
	fc.openFile()
	
	print fc.getContents()
	
	whiteSpaceCleaner = WhiteSpaceCleaner(fc.getContents())
	
	whiteSpaceCleaner.removeExtraSpaces()
	
	fc.setContents(whiteSpaceCleaner.getContents())
	
	print fc.getContents()
	
	sp = SyntaxParser(fc.getContents())
	
	sp.parse()
	
	pr = ProgramWriter(fc.getContents(), "output.as")
	pr.writeBinary()
	
	return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
