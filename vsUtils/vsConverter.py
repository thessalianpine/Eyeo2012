########################################
# vsConverter.py
# parse common vs project (vs2010) and
# adjust contents (names etc.) to
# user-given name
#
# requires Python 2.7.3 or higher
########################################

import sys
import os
import string
import argparse
import shutil
import xml.etree.ElementTree as ET

ET._namespace_map["http://schemas.microsoft.com/developer/msbuild/2003"] = '' #empty ns alias or visual studio won't like the resulting xml

slnName = 'room.sln'
vcxprojName = 'room.vcxproj'
filtersName = vcxprojName + ".filters"
resName = 'Resources.rc'

def parseDirArg():
    parser = argparse.ArgumentParser(description='Supply path to a folder with base project to duplicate')
    parser.add_argument('folderpath', help='Folder that contains the project files')
    args = parser.parse_args()
    return args.folderpath

def copyFiles(newName, sourceDir):
    targetDir = os.getcwd() + './vs/'
    os.mkdir(targetDir)
    shutil.copyfile(sourceDir + '/' + slnName, targetDir + newName + '.sln')
    shutil.copyfile(sourceDir + '/' + vcxprojName, targetDir + newName + '.vcxproj')
    shutil.copyfile(sourceDir + '/' + filtersName, targetDir + newName + '.vcxproj.filters')
    shutil.copyfile(sourceDir + '/' + resName, targetDir + resName)

def updateContents(newName):
    dir = os.getcwd() + './vs/'

    #sln
    slnTargetName = dir + newName + '.sln'
    slnFile = open(slnTargetName, 'r+')
    slnContent = slnFile.read()    
    slnContent = string.replace(slnContent, string.split(slnName, '.')[0], newName)
    slnFile.seek(0, 0)
    slnFile.write(slnContent)
    slnFile.close()
    
    #vcxproj
    vcxprojTargetName = dir + newName + '.vcxproj'
    vcxProjTree = ET.parse(vcxprojTargetName)
    pgNode = vcxProjTree.find("{http://schemas.microsoft.com/developer/msbuild/2003}PropertyGroup[@Label='Globals']")
    nsNode = pgNode.find("{http://schemas.microsoft.com/developer/msbuild/2003}RootNamespace")
    nsNode.text = newName
    vcxProjTree.write(vcxprojTargetName)

def generateResName(filename):
    if filename == None or filename[0] == '.':
        return None
    items = string.split(filename, '.')
    res = "RES"
    for part in items:
        res += ('_' + part.upper())       
    return res

def getResType(suffix):
    suffix = suffix.lower()
    if suffix == "ico" or suffix == "jpg" or suffix == "png" or suffix == "icns":
        return "IMAGE"
    if suffix == "frag" or suffix == "vert":
        return "GLSL"   
    return "UNKNOWN"

def generateCinderResMacro(rcName, numID):
    macro = "#define " + rcName + "\t\t\t\t" + "CINDER_RESOURCE( ../resources/, "
    items = string.split(rcName, '_')
    filename = ''
    for i in xrange(1, len(items)):
        filename += items[i].lower()
        if i == len(items) - 2:
            filename += '.'    
    macro += filename + ', '
    macro += str(numID) + ', '
    macro += getResType( items[len(items)-1 ]) + ' )'
    return macro

def updateResources():
    targetpath = os.getcwd()

    resourceNames = []
    for fname in os.listdir(targetpath + '/resources/'):
        res = generateResName(fname)
        if(res != None):
            resourceNames.append(res)
    
    rcFile = open(targetpath + '/vs/Resources.rc', 'a')    
    rcFile.write("\n/////////////////////////////////////////////\n// section generated by vsConverter\n/////////////////////////////////////////////")    
    for rn in resourceNames:
        rcFile.write("\n" + rn )
    rcFile.write("\n/////////////////////////////////////////////")
    rcFile.close()

    idCount = 140
    resHeader = open(targetpath + '/include/Resources.h', 'a')
    resHeader.write("\n/////////////////////////////////////////////\n// section generated by vsConverter\n/////////////////////////////////////////////")
    for rcName in resourceNames:
        resHeader.write("\n" + generateCinderResMacro(rcName, idCount))
        idCount += 1
    resHeader.write("\n/////////////////////////////////////////////")
    resHeader.close()    
    
def main():
    sourceDir = parseDirArg()
    name = raw_input("Enter desired project name: ")
    copyFiles(name, sourceDir)
    updateContents(name)
    updateResources()

if __name__ == "__main__":
    main()
