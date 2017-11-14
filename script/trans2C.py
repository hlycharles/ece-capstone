# translate a file to C array

import sys
import os

def translateFile1D(src, dest, arrType, arrName):
    srcFile = open(src, "r")
    lines = srcFile.readlines()
    srcFile.close()

    destFile = open(dest, "w")
    arrLen = len(lines)
    declaration = arrType + " " + arrName + "[" + str(arrLen) + "] = {"
    destFile.write(declaration + "\n")

    for i in range(len(lines)):
        line = lines[i]
        content = "    " + line[:len(line) - 1]
        if (i < len(lines) - 1):
            content += ","
        content += "\n"
        destFile.write(content)
    
    destFile.write("};\n")
    destFile.close()

def translateFile2D(src, dest, arrType, arrName, row, col):
    srcFile = open(src, "r")
    lines = srcFile.readlines()
    srcFile.close()

    destFile = open(dest, "w")
    arrLen = len(lines)
    declaration = arrType + " " + arrName + "[" + str(row) + "][" + str(col) + "] = {"
    destFile.write(declaration + "\n")

    for i in range(row):
        destFile.write("  {\n")
        for j in range(col):
            line = lines[i * col + j]
            content = "    " + line[:len(line) - 1]
            if (j < col - 1):
                content += ","
            content += "\n"
            destFile.write(content)
        destFile.write("  }")
        if (i < row - 1):
            destFile.write(",")
        destFile.write("\n")
    
    destFile.write("};\n")
    destFile.close()

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print "Please provide a file that needs to be translated"
    if (len(sys.argv) < 3):
        print "Please provide a destination file"
    if (len(sys.argv) < 4):
        print "Please provide an array type"
    if (len(sys.argv) < 5):
        print "Please provide an array name"
    if (len(sys.argv) < 6):
        translateFile1D(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        row = int(sys.argv[5])
        col = int(sys.argv[6])
        translateFile2D(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], row, col)

    
