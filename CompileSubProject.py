# Script to compile and link source files seperately
# Call python CompileAll.py DirectoryOfProject


import sys
import os
import subprocess

def PrintWithIndent(indentStep, output):
    global logFile
    indent = ""
    indent=indent.rjust(indentStep*2)

    formOutput = "{:s}{:s}".format(indent, output)
    print(formOutput)
    logFile.write(formOutput + "\n")
    return
# PrintWithIndent(localLevel, "".format(element))

def CompileFolderContontRecursively(path, relPath, level):
    global binPathAnchor
    global maxRecursionDept
    global compilerArgs
    global compilerLog
    global ignoreFiles

    localLevel = level+1
    if (localLevel >= maxRecursionDept):
        raise NameError("Maximum recursion limit of {:d} is reached".format(maxRecursionDept)) 

    content = os.listdir(path)
    for element in content:
        if os.path.isdir(os.path.join(path,element)):
            PrintWithIndent(localLevel, "Folder '{:s}' found".format(element))
            if (element != "Binary"):
                localRoot = os.path.join(path,element)
                relativePath2Root = os.path.join(relPath,element)
                PrintWithIndent(localLevel, "Sweeping folder structure")

                binaryDirectory = os.path.join(binPathAnchor,relativePath2Root)
                if ( True != os.path.isdir(binaryDirectory)):
                    os.mkdir(binaryDirectory)
                    PrintWithIndent(localLevel, "Generating Folder '{:s}' in Binary Path".format(element))

                CompileFolderContontRecursively(localRoot, relativePath2Root, localLevel)
            else:
                PrintWithIndent(localLevel, "Skip Binary Folder")
        elif (element != ignoreFile):
            name, ext = os.path.splitext(element)
            if (ext == '.cpp' or ext == '.c'):
                PrintWithIndent(localLevel, "SourceCodeFile '{:s}' found".format(element))
                binaryDirectory = os.path.join(binPathAnchor,relPath)
                targetFile = os.path.join(path,element)
                outFileName = "{:s}.o".format(name)
                outputFilePath = os.path.join(binaryDirectory, outFileName)
                
                CompilerCall = "g++ -c -Wall -g {:s} {:s} -o {:s}".format(targetFile, compilerArgs, outputFilePath)
                PrintWithIndent(localLevel, "CompilerCall: {:s}".format(CompilerCall))
                with open(compilerLog, 'w+') as myOutfile:
                    subprocess.call(CompilerCall, stdout=myOutfile)
                    #os.system(CompilerCall)
        else:
            PrintWithIndent(localLevel, "SourceCodeFile '{:s}' ignored due to call arguments".format(element))
    return


def GenerateLinkCommandRecursively(path, level):
    localLevel = level+1
    if (localLevel >= maxRecursionDept):
        raise NameError("Maximum recursion limit of {:d} is reached".format(maxRecursionDept)) 

    outFileStream = ""
    content = os.listdir(path)
    for element in content:
        if os.path.isdir(os.path.join(path,element)):
            PrintWithIndent(localLevel, "Folder '{:s}' found".format(element))
            localRoot = os.path.join(path,element)
            outFileStream = outFileStream + ' ' + GenerateLinkCommandRecursively(localRoot, localLevel)
        else:
            name, ext = os.path.splitext(element)
            if (ext == '.o'):
                PrintWithIndent(localLevel, "BinaryFile '{:s}' found".format(element))
                binaryFile = os.path.join(path,element)
                outFileStream = outFileStream + ' ' + binaryFile
    return outFileStream



################################################################################
#### Compiles Subproject
#### Is ment to compile either Test-Part, or Source Part
####
#### Constrains: 
####    - Python script has to be placed in Project Root
####    - There must be only one c/cpp file in the Subproject that contains an main()
#### Arguments
####    -#1: SubprojectDirectory 
####    -#2: Project Folder of Subproject
####        This name is also used als ProjectName for the exe output
####    -#3: Compiler Arguments primary for Include paths "-I C:\..."
####    - 

print("Setup of folder structure")

if len(sys.argv) < 3:
    print("Nothing to do ... There are at least 3 arguments required")

scirptPath =sys.argv[0]
rootDir = os.path.dirname(scirptPath)
prjFolder = sys.argv[1]
prjDir = os.path.join(rootDir,prjFolder)
binPathAnchor = os.path.join(prjDir,"Binary")
compilerLog = os.path.join(binPathAnchor,"CompileLog.log")
compilerArgs = sys.argv[2]

ignoreFile = ""
if len(sys.argv) >= 4:
    ignoreFile = sys.argv[3]

# Generate Binary directory if necessary
if ( True != os.path.isdir(binPathAnchor)):
    os.mkdir(binPathAnchor)

logFile = open(compilerLog, 'w')
logFile.write("CompileLog \n")
PrintWithIndent(0, "Project Root is set to '{:s}'".format(prjDir))
PrintWithIndent(0, "Binaries will be placed in '{:s}'".format(binPathAnchor))

maxRecursionDept = 10
recursionLevel = 0
relativePath = ""

# CompileFolderContontRecursively(prjDir, relativePath, recursionLevel)
PrintWithIndent(0, "Generate linker call '{:s}'".format(binPathAnchor))

objectFilesSteam = ""
if len(sys.argv) >= 5:
    addObjectFiles = sys.argv[4]
    objDir = os.path.join(rootDir,addObjectFiles)
    objBinPathAnchor = os.path.join(objDir,"Binary")
    objectFilesSteam = GenerateLinkCommandRecursively(objBinPathAnchor, recursionLevel)

binaryFilesSteam = GenerateLinkCommandRecursively(binPathAnchor, recursionLevel)




CompilerCall = "g++ -o {:s}.exe {:s} {:s}".format(prjFolder, binaryFilesSteam, objectFilesSteam)
PrintWithIndent(0, "CompilerCall: {:s}".format(CompilerCall))
with open(compilerLog, 'w+') as myOutfile:
    subprocess.call(CompilerCall, stdout=myOutfile)
    #os.system(CompilerCall)

logFile.close()
exit