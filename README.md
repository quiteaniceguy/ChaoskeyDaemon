# ChaoskeyDaemon
This application runs the NistSatisticalTestSuite in an unstupid way(through c), testing for randomness in binary files. Also includes a daemon for users to check continuing functionality.

May be purchased here:

https://altusmetrum.org/ChaosKey/,
http://shop.gag.com/random.html

To compile, run-- 
	"gcc TestSuite.c frequency.c blockFrequency.c cephes.c runs.c longestRunOfOnes.c rank.c matrix.c discreteFourierTransform.c dfft.c universal.c approximateEntropy.c cusum.c randomExcursions.c randomExcursionsVariant.c serial.c linearComplexity.c nonOverlappingTemplateMatchings.c overlappingTemplateMatchings.c -o TestSuiteTest -lm"

This should be put in a makefile

to execute, run the following command, using it's format. --raw & --chaoskeybits are optional. 
	"./NameOfRunnableFile [--raw] [--chaoskeybits] [--nbytes <nbytes>] [<file>]"

