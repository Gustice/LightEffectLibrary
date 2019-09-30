# Script to compile and link source files seperately
# Call python CompileAll.py DirectoryOfProject


import sys
import os
import subprocess
import re # Regular expressions

################################################################################
## Prompts output with adjustable intent and saves output to log file
## param    indentStep  Intent steps for prompted text
## param    output      Text to be prompted
def PrintWithIndent(indentStep, output):
    global logFile
    indent = ""
    indent=indent.rjust(indentStep*2)

    formOutput = "{:s}{:s}".format(indent, output)
    print(formOutput)
    logFile.write(formOutput + "\n")
    return

################################################################################
## Iterates folders recursively to compile all containing c/cpp files
## param    srcPath Relative Path of source folder to current working directory 
## param    outPath Relative Path output file destination to current working directory
## level    Current folder iteration depth
def CompileFolderContontRecursively(srcPath, outPath, level):
    global binPathAnchor
    global relBinPath
    global maxRecursionDept
    global compilerArgs
    global compilerLog

    # Abort if recursion level is exceeded
    localLevel = level+1
    if (localLevel >= maxRecursionDept):
        raise NameError("Maximum recursion limit of {:d} is reached".format(maxRecursionDept)) 

    # Iterate through folder content
    content = os.listdir(srcPath)
    for element in content:
        # In case of nested folder
        if os.path.isdir(os.path.join(srcPath,element)):
            PrintWithIndent(localLevel, "Folder '{:s}' found".format(element))
            if (element != "Binary"): # todo make this Test less generic
                localRoot = os.path.join(srcPath,element)
                relativePath2Root = os.path.join(outPath,element)
                PrintWithIndent(localLevel, "Sweeping folder structure")
                binaryDirectory = os.path.join(binPathAnchor,relativePath2Root)

                # Make folder in output Path if not generated yet
                if ( True != os.path.isdir(binaryDirectory)):
                    os.mkdir(binaryDirectory)
                    PrintWithIndent(localLevel, "Generating Folder '{:s}' in Binary Path".format(element))

                # Iterate through nested folder by recursion
                CompileFolderContontRecursively(localRoot, relativePath2Root, localLevel)
            else:
                PrintWithIndent(localLevel, "Skip Binary Folder")
        # In case of file
        elif (element != ignoreFile):
            # Process only cpp and c files
            name, ext = os.path.splitext(element)
            if (ext == '.cpp' or ext == '.c'):
                PrintWithIndent(localLevel, "SourceCodeFile '{:s}' found".format(element))
                
                binaryDirectory = os.path.join(relBinPath,outPath) 
                targetFile = os.path.join(srcPath,element)
                outFileName = "{:s}.o".format(name)
                outputFilePath = os.path.join(binaryDirectory, outFileName)
                
                CompilerCall = "g++ -c -Wall -g {:s} {:s} -o {:s}".format(targetFile, compilerArgs, outputFilePath)
                PrintWithIndent(localLevel, "CompilerCall: {:s}".format(CompilerCall))
                # Execute Compiler with generated argument stream
                with open(compilerLog, 'w+') as myOutfile:
                    subprocess.call(CompilerCall, stdout=myOutfile)
                    #os.system(CompilerCall)
        else:
            PrintWithIndent(localLevel, "SourceCodeFile '{:s}' ignored due to call arguments".format(element))
    return

################################################################################
## Iterates folders recursively to generate a linker call
## param    outPath Relative Path output file destination to current working directory
## level    Current folder iteration depth
def GenerateLinkCommandRecursively(outPath, level):
    outFileStream = ""

    # Abort if recursion level is exceeded
    localLevel = level+1
    if (localLevel >= maxRecursionDept):
        raise NameError("Maximum recursion limit of {:d} is reached".format(maxRecursionDept)) 

    # Iterate through folder content
    content = os.listdir(outPath)
    for element in content:
        # In case of nested folder
        if os.path.isdir(os.path.join(outPath,element)):
            PrintWithIndent(localLevel, "Folder '{:s}' found".format(element))
            localRoot = os.path.join(outPath,element)
            # Iterate through nested folder by recursion
            outFileStream = outFileStream + ' ' + GenerateLinkCommandRecursively(localRoot, localLevel)
        # In case of file
        else:
            name, ext = os.path.splitext(element)
            # Process only o files
            if (ext == '.o'):
                PrintWithIndent(localLevel, "BinaryFile '{:s}' found".format(element))
                binaryFile = os.path.join(outPath,element)
                # Add found file to linker argument list
                outFileStream = outFileStream + ' ' + binaryFile
    return outFileStream




def ProcessProjectRelatedArguments(argumentList):
    include = ""
    exclude = ""
    index = 0

    for arg in argumentList:
        index = index +1

        iM = re.search("(?<=^-In )(.*)$", arg)
        if (iM):
            include = iM.group()
        eM = re.search("(?<=^-Ex )(.*)$", arg)
        if (eM):
            exclude = eM.group()


    return include, exclude

################################################################################
#### Compiles Subproject
#### Is ment to compile either Test-Part, or Source Part
####
#### Constrains: 
####    - Python script has to be placed in Project Root
####    - There must be only one c/cpp file in the Subproject that contains an main()
#### Arguments
####    -#0: SubprojectDirectory 
####    -#1: SubProject_Folder [-In Include_SubProjectFolder] [-Ex Exclude_SourceFile]
####        This name of SubProject_Folder is also used als ProjectName for the exe output
####        The name of Include_SubProjectFolder will be used to link additional source files
####        The name of Exclude_SourceFile will be used to ignore additional source files
####    -#3: Compiler Arguments primary for Include paths "-I C:\..."
####    - 

print("Setup of folder structure")

if len(sys.argv) <= 2:
    print("Nothing to do ... There are at least 2 arguments required")

################################################################################
#### Reading call arguments

scirptPath =sys.argv[0]
rootDir = os.path.dirname(scirptPath)
os.chdir(rootDir)

prjArg = sys.argv[1]
prjArgs = prjArg.split(',')
prjFolder = prjArgs[0]
includeFolder, excludeFile = ProcessProjectRelatedArguments(prjArgs)

prjDir = os.path.join(rootDir,prjFolder)
binPathAnchor = os.path.join(prjDir,"Binary")
relBinPath = os.path.join(prjFolder,"Binary")
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

################################################################################
#### Compiling sourcefiles

CompileFolderContontRecursively(prjFolder, relativePath, recursionLevel)
PrintWithIndent(0, "Generate linker call '{:s}'".format(binPathAnchor))


################################################################################
#### Link sourcefiles

objectFilesSteam = ""
binaryFilesSteam = GenerateLinkCommandRecursively(relBinPath, recursionLevel)


if includeFolder != "":
    relObjBin = os.path.join(includeFolder,"Binary")
    objectFilesSteam = GenerateLinkCommandRecursively(relObjBin, recursionLevel)
    if excludeFile != "": 
        excludeFile = re.sub(includeFolder, "", excludeFile)
        excludeFile = relObjBin + excludeFile
        excludeFile = excludeFile.replace("\\","\\\\")
        excludeFile = re.sub("\.(cpp)|(c)$", ".o", excludeFile)
        objectFilesSteam = re.sub(excludeFile, "", objectFilesSteam)


CompilerCall = "g++ -o {:s}.exe {:s} {:s}".format(prjFolder, binaryFilesSteam, objectFilesSteam)
PrintWithIndent(0, "CompilerCall: {:s}".format(CompilerCall))
# Execute Linker with generated argument stream
with open(compilerLog, 'w+') as myOutfile:
    subprocess.call(CompilerCall, stdout=myOutfile)
    #os.system(CompilerCall)

logFile.close()
exit