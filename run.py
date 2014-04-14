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
diff = [0, 0]

os.system("rm all_log.txt")

outfile = open("all_log.txt", "a")
outfile.write("CONFIG\n")
outfile.close()

def parselogfile(top, eta, ite, sample, scalefactor):
	print "\nParsing file log.txt...\n"
	usedhigh = 0.0;
	usedlow = 1.0;
	unusedhigh = 0.0
	unusedlow = 1.0
	differences = []
	udifferences = []
	sam = sample
	sam2 = sample

	largediff = 0
	leastdiff = 1
	ulargediff = 0
	uleastdiff = 1

	final = False
	curletter = "A"

	largeimage = 0
	leastimage = 0
	ulargeimage = 0
	uleastimage = 0

	infile = open("log.txt")
	outfile = open("all_log.txt", "a")

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
				sam = sample
				sam2 = sample
			final = False
		elif "Final" in line:
			final = True
		elif "[" in line and final:
			if sam != 0:
				if usedhigh < float(line[9:].strip()):
					usedhigh = float(line[9:].strip())
				if usedlow > float(line[9:].strip()):
					usedlow = float(line[9:].strip())
				sam -= 1
			else:
				if unusedhigh < float(line[9:].strip()):
					unusedhigh = float(line[9:].strip())
				if unusedlow > float(line[9:].strip()):
					unusedlow = float(line[9:].strip())
		elif not line.strip():
			avg = 0
			uavg = 0
			if sam2 != 0:
				diff[0] = usedhigh - usedlow
				differences.append(diff[0])
				sam2 -= 1
			else:
				diff[1] = unusedhigh - unusedlow
				udifferences.append(diff[1])
			if len(differences) == sample:
				total = 0
				i = 0
				for d in differences:
					total += d
					i += 1
					if d > largediff:
						largediff = d
						largeimage = i
					if d < leastdiff:
						leastdiff = d
						leastimage = i

				avg = total / i
				differences = []

				outfile.write("Best recognized image: " + str(largeimage) + " - " + str(largediff) + "\n")
				outfile.write("Least recognized image: " + str(leastimage) + " - " + str(leastdiff) + "\n")
				outfile.write("Trained avg recognition rate: " + str(avg) + "\n")
			elif len(udifferences) == (20 - sample):
				total = 0
				i = 0
				for d in udifferences:
					total += d
					i += 1
				
				uavg = total / i
				udifferences = []

				outfile.write("Untrained avg recognition rate: " + str(uavg) + "\n\n")


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