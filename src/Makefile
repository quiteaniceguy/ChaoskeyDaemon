# Copyright © 2016 Bart Massey and Thomas Eblen
# Makefile for NIST assess stuff.

CC = gcc
#CDEBUG = -O4
CDEBUG = -g
CFLAGS = -Wall -fpic $(CDEBUG) \
         -I/usr/lib/jvm/default-java/include \
         -I/usr/lib/jvm/default-java/include/linux

JAVAC = javac
JAVAFLAGS = 

OBJS = 	approximateEntropy.o assess.o blockFrequency.o cephes.o \
	cusum.o dfft.o discreteFourierTransform.o frequency.o \
	generators.o genutils.o linearComplexity.o \
	longestRunOfOnes.o matrix.o \
	nonOverlappingTemplateMatchings.o \
	overlappingTemplateMatchings.o randomExcursions.o \
	randomExcursionsVariant.o rank.o runs.o serial.o universal.o \
	utilities.o
CLASSES = NistTestSuite.class ex/NistStatisticalTestSuite.class

.SUFFIXES: .java .class

.java.class:
	$(JAVAC) $(JAVAFLAGS) $*.java 

NistTestSuite: NistTestSuite.class ex/NistStatisticalTestSuite.class libassess.so

libassess.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o libassess.so $(OBJS)

clean:
	-rm -f libassess.so $(OBJS) $(CLASSES) core *.log
