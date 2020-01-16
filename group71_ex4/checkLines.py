import os
import glob
import sys

def readFile(currentFile, limitstr):
    count = 0
    limit = int(limitstr)
    countLines = 0
    prevLine = ""
    funcName = ""
    fp = open(currentFile, 'r')
    lines = fp.readlines()
    print("in file ", currentFile, "\n")
    for line in lines:
        if ("{" in line):
            if("=" not in line):
                if (count == 0):
                    funcName = prevLine
                count+= 1
        elif ("}" in line):
            if (";" not in line):
                count-= 1
                if (count == 0):
                    if (countLines >= limit):
                        print("\t",funcName,"\t", countLines + 1, "lines\n\n")
                    countLines = 0

        if (count > 0):
            countLines += 1

        prevLine = line

    fp.close()


def readFolder(limit):
    for filename in glob.glob("*.c"):
        readFile(filename, limit)

if(len(sys.argv) > 1):
    readFolder(sys.argv[1])
else:
    readFolder(1)
