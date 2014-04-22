#!/usr/bin/python2

import os
import sys


tops = ["30", "36", "30,30", "36,36", "36,30"]
samples = [10]
etas = [0.25, 0.5]
iters = [1000]
scalefactors = [5]
diff = [0, 0]
letters = ["A","B","C","D","E","F","G","H","I","J","K","L","M",
		   "N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]

# Contains information about a single image
class ImageStats(object):
	def __init__(self, filehandle, letter):
		self.file = ""
		self.letter = letter
		self.recognizedAs = ''
		self.reconrate = ''
		self.trained = 0
		self.certainty = 0

		# The image file is the first line
		self.file = filehandle.readline().strip()
		self.trained = filehandle.readline()[25:].strip()

		# Ignore line
		filehandle.readline()

		# Ignore the initial input
		for i in range(0, 26):
			filehandle.readline()

		# Read what I was reconized as
		line = filehandle.readline()
		self.recognizedAs = line[28]
		self.certainty = float( line[30:].split("):")[0] )

		# Ignore the final output (for now)
		for i in range(0, 26):
			filehandle.readline()

		# Ignore trailing newline
		filehandle.readline()


# Aggregates 20 ImageStats
class Letter(object):
	def __init__(self, filehandle, letter):
		self.letter = letter
		self.images = []
		
		for i in range(0, 20):
			img = ImageStats(filehandle, letter)
			self.images.append(img)
			assert(self.letter == img.letter)

	def successRate(self):
		recons = 0
		count = len(self.images)
		for img in self.images:
			if img.letter == img.recognizedAs:
				recons += 1
		return (float(recons) / float(count))

	def writeInfo(self, filehandle):
		filehandle.write("%s: %s%% \t " % (self.letter, str(self.successRate()*100)))
		for img in self.images:
			filehandle.write(img.recognizedAs)
		filehandle.write("\n")


# Contains the letter results and the configuration
class Execution(object):
	def __init__(self, filehandle, top, eta, ite, sample, scalefactor):
		self.topology = top
		self.eta = eta
		self.iters = ite
		self.samples = sample
		self.scalefactor = scalefactor
		
		# Parse the logfile
		self.letters = []
		for l in letters:
			ltr = Letter(filehandle, l)
			self.letters.append(ltr)

	def successRate(self):
		x = 0.0
		for ltr in self.letters:
			x += ltr.successRate()
		return x / float(len(self.letters))

	def strTop(self):
		inodes = (30 / self.scalefactor) ** 2
		onodes = 26
		return "{%s,%s,%s}" % (str(inodes), str(self.topology), str(onodes))

	def printInfo(self):
		print "Successrate: %s" % str(self.successRate())
		print "Topology:    %s" % self.strTop()
		print "Iterations:  %s" % self.iters
		print "Samples:     %s" % self.samples
		print "Scalefactor: %s" % self.scalefactor

	def writeInfo(self, filehandle):
		filehandle.write("-----------------\n")
		filehandle.write("Successrate: %s\n" % str(self.successRate()))
		filehandle.write("Topology:    %s\n" % self.strTop())
		filehandle.write("Iterations:  %s\n" % self.iters)
		filehandle.write("Samples:     %s\n" % self.samples)
		filehandle.write("Scalefactor: %s\n" % self.scalefactor)

		# Write information about the letters
		for ltr in self.letters:
			ltr.writeInfo(filehandle)



def parselogfile(top, eta, ite, sample, scalefactor, logfile):
	filehandle = open(logfile, "r")
	ex = Execution(filehandle, top, eta, ite, sample, scalefactor)
	filehandle.close()
	return ex

def runExecution():
	for top in tops:
		for eta in etas:
			for ite in iters:
				for sample in samples:
					for scalefactor in scalefactors:
						for i in range(0,10):
							args = "-top " + str(top) 
							args += " -deta " + str(eta) 
							args += " -iter " + str(ite) 
							args += " -samples " + str(sample) 
							args += " -scalefactor " + str(scalefactor)
							cmd = './ann %s > /dev/null' % args

							print "Running command: " + cmd
							ret = os.system(cmd)
							if ret == 0:
								ex = parselogfile(top, eta, ite, sample, scalefactor, "log.txt")
								print "Successrate: " + str(ex.successRate()*100) + "%"
								executions.append(ex)
							else:
								print "EXECUTION ABORTED WITH STATUS %s" % str(ret)


executions = []

if len(sys.argv) == 1:
	runExecution()
else:
	# These numbers are RANDOM and NOT THE ACTUAL VALUES. We do 
	# not know the initial configuration
	ex = parselogfile("?", 0.5, 1, 1, 1, sys.argv[1])
	executions.append(ex)

# Sort the executions 
executions.sort(key=lambda x: x.successRate(), reverse=True)
for e in executions:
	e.printInfo()
	print "----------"

# Write DETAILED information to "all_log.txt"
filehandle = open("all_log.txt", "w")
for e in executions:
	e.writeInfo(filehandle)
