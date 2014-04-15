#! /usr/bin/python

import os, sys

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
letters = ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]

os.system("rm all_log.txt")

outfile = open("all_log.txt", "a")
outfile.write("CONFIG\n")
outfile.close()

def parselogfile(top, eta, ite, sample, scalefactor):
	print "\nParsing file log.txt...\n"

	sam = sample

	differences = []
	udifferences = []

	trainedvalues = []
	trainingvalues = []

	trainingmostreconizedimage = -1
	trainingleastreconizedimage = -1
	trainedmostrecognizedimage = -1
	trainedleastrecognizedimage = -1

	trainedfinalmostrecimg = 0
	trainedfinalleastrecimg = 0
	trainingfinalmostrecimg = 0
	trainingfinalleastrecimg = 0

	traininglastmostrecognizedimage = 0
	trainedlastmostrecognizedimage = 0
	traininglastmostrecognizedvalue = 0
	trainedlastmostrecognizedvalue = 0

	traininglastleastrecognizedimage = 1
	trainedlastleastrecognizedimage = 1
	traininglastleastrecognizedvalue = 1
	trainedlastleastrecognizedvalue = 1

	mostrecognized = 0
	leastrecognized = 1

	avg = 0
	uavg = 0

	count = 0
	final = False
	curletter = "A"
	usedletter = True

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
			count += 1
			if sam > 0:
				usedletter = True
				sam -= 1
			elif sam <= 0:
				usedletter = False
			if line[4] != curletter:
				curletter = line[4]
				outfile.write("Character: " + curletter + "\n")
				
			final = False
		elif "Final" in line:
			final = True
		elif "[" in line and final:
			if usedletter:
				differences.append(float(line[9:].strip()))
			else:
				udifferences.append(float(line[9:].strip()))
		elif not line.strip():
			
			if usedletter:
				if len(differences) == 26:
					i = -1
					for d in differences:
						i += 1
						if d > mostrecognized:
							mostrecognized = d
							trainingmostreconizedimage = i
						if d < leastrecognized:
							leastrecognized = d
							trainingleastreconizedimage = i

					trainingvalues.append(mostrecognized)
					if mostrecognized > traininglastmostrecognizedvalue:
						traininglastmostrecognizedvalue = mostrecognized
						traininglastmostrecognizedimage = count
						trainingfinalmostrecimg = trainingmostreconizedimage
					if leastrecognized < traininglastleastrecognizedvalue:
						traininglastleastrecognizedvalue = leastrecognized
						traininglastleastrecognizedimage = count
						trainingfinalleastrecimg = trainingleastreconizedimage

					differences = []
			else:
				if len(udifferences) == 26:
					mostrecognized = 0
					i = -1
					for d in udifferences:
						i += 1
						if d > mostrecognized:
							mostrecognized = d
							trainedmostrecognizedimage = i
						if d < leastrecognized:
							leastrecognized = d
							trainedleastrecognizedimage = i

					trainedvalues.append(mostrecognized)
					if mostrecognized > trainedlastmostrecognizedvalue:
						trainedlastmostrecognizedvalue = mostrecognized
						trainedlastmostrecognizedimage = count
						trainedfinalmostrecimg = trainedmostrecognizedimage
					if leastrecognized < trainedlastleastrecognizedvalue:
						trainedlastleastrecognizedvalue = leastrecognized
						trainedlastleastrecognizedimage = count
						trainedfinalleastrecimg = trainedleastrecognizedimage
				
					udifferences = []

			if count == 20:
				cur = 0
				allval = 0
				for value in trainingvalues:
					cur += 1
					allval += value
				avg = allval / cur

				cur = 0
				allval = 0
				for value in trainedvalues:
					cur += 1
					allval += value
				uavg = allval / cur

				outfile.write("Best recognized image during training: " + str(traininglastmostrecognizedimage) + " | " + str(traininglastmostrecognizedvalue) + " = " + letters[trainingfinalmostrecimg] + "\n")
				outfile.write("Best reconized image after training: " + str(trainedlastmostrecognizedimage) + " | " + str(trainedlastmostrecognizedvalue) + " = " + letters[trainedfinalmostrecimg] + "\n")
				outfile.write("Least recognized image during training: " + str(traininglastleastrecognizedimage) + " | " + str(traininglastleastrecognizedvalue) + " = "  + letters[trainingfinalleastrecimg] + "\n")
				outfile.write("Least recognized image after training: " + str(trainedlastleastrecognizedimage) + " | " + str(trainedlastleastrecognizedvalue) + " = "  + letters[trainedfinalleastrecimg] + "\n")
				outfile.write("Trained avg recognition rate: " + str(avg) + "\n")
				outfile.write("Untrained avg recognition rate: " + str(uavg) + "\n\n")

				avg = 0
				uavg = 0
				count = 0
				sam = sample
				leastrecognized = 1
				mostrecognized = 0

				traininglastleastrecognizedvalue = 1
				trainedlastleastrecognizedvalue = 1
				traininglastmostrecognizedvalue = 0
				trainedlastmostrecognizedvalue = 0

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