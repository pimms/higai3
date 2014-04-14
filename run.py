#! /usr/bin/python

import os

tops = []
for i in xrange(12, 30):
	tops.append(i)

samples = []
for i in xrange(1,20):
	samples.append(i)

etas = [0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55 ,0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1]
iters = [100, 1000, 5000, 10000]
scalefactors = [1, 2, 3, 5, 6, 10]
letters = ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
diff = [0]

os.system("rm all_log.txt")

outfile = open("all_log.txt", "a")
outfile.write("CONFIG\n")
outfile.close()

def parselogfile(top, eta, ite, sample, scalefactor):
	print "\nParsing file log.txt...\n"
	infile = open("log.txt")
	outfile = open("all_log.txt", "a")
	high = 0.0;
	low = 1.0;
	differences = []
	largestdiff = 0
	leastdiff = 1
	final = False
	curletter = letters[0]
	largeimage = 0
	leastimage = 0

	outfile.write("------------------\n")
	outfile.write("\nTopology: " + str(top) + "\n")
	outfile.write("Eta: " + str(eta) + "\n")
	outfile.write("Iterations: " + str(ite) + "\n")
	outfile.write("Samples: " + str(sample) + "\n")
	outfile.write("Scalefactor: " + str(scalefactor) + "\n\n")
	outfile.write("Letter: " + curletter + "\n")

	for line in infile.readlines():
		if ".jpg" in line:
			if line[4] != curletter:
				curletter = line[4]
				outfile.write("Character: " + curletter + "\n")
			final = False
		elif "Final" in line:
			final = True
		elif "[" in line and final:
			if high < float(line[9:].strip()):
				high = float(line[9:].strip())
			if low > float(line[9:].strip()):
				low = float(line[9:].strip())
		elif not line.strip():
			diff[0] = high - low
			differences.append(diff[0])
			if len(differences) == 20:
				total = 0
				avg = 0
				i = 0
				for d in differences:
					total += d
					i += 1
					if d > largestdiff:
						largestdiff = d
						largeimage = i
					if d < leastdiff:
						leastdiff = d
						leastimage = i

				avg = total / i
				differences = []

				outfile.write("Trained avg recognition rate: " + str(avg) + "\n")
				outfile.write("Untrained avg recognition rate: ????????????\n")
				outfile.write("Best recognized image: " + str(largeimage) + " - " + str(largestdiff) + "\n")
				outfile.write("Least recognized image: " + str(leastimage) + " - " + str(leastdiff) + "\n\n")

	infile.close()
	outfile.close()


for top in tops:
	for eta in etas:
		for ite in iters:
			for sample in samples:
				for scalefactor in scalefactors:
					output = "-top " + str(top) + " -deta " + str(eta) + " -iter " + str(ite) + " -samples " + str(sample) + " -scalefactor " + str(scalefactor)
					os.system('./ann %s' % output)
					parselogfile(top, eta, ite, sample, scalefactor)