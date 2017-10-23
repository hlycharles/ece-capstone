# translate a file to C array

import sys
import os

def translateFile(src, dest, arrType, arrName):
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

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print "Please provide a file that needs to be translated"
    if (len(sys.argv) < 3):
        print "Please provide a destination file"
    if (len(sys.argv) < 4):
        print "Please provide an array type"
    if (len(sys.argv) < 5):
        print "Please provide an array name"
    translateFile(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
