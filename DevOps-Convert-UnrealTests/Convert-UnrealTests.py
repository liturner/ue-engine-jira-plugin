print("##### Start Python")

import json
import sys
import os.path
import io
import codecs
import xml.etree.ElementTree as elemTree
import datetime
import uuid

# https://docs.microsoft.com/en-us/azure/devops/pipelines/tasks/test/publish-test-results?view=azure-devops&tabs=trx%2Cyaml#result-formats-mapping

def generateEtree(rootDict):
    root = elemTree.Element("TestRun") 
    root.attrib['xmlns'] ="http://microsoft.com/schemas/VisualStudio/TeamTest/2010"
    root.attrib['runUser'] = "S1\devops"

    testSettingsElement = elemTree.Element("TestSettings")
    root.append(testSettingsElement)

    timesElement = elemTree.Element("Times")
    timesElement.attrib['creation'] = formatDateTime(rootDict['reportCreatedOn'])
    timesElement.attrib['start'] = formatDateTime(rootDict['reportCreatedOn'])
    timesElement.attrib['finish'] = formatDateTime(rootDict['reportCreatedOn'])
    root.append(timesElement)
    
    resultsSummaryElement = elemTree.Element("ResultSummary")
    resultsSummaryElement.attrib['outcome'] = "Passed"
    for testResult in rootDict['tests']:
        if(formatOutcome(testResult['state']) == "Failed"):
            resultsSummaryElement.attrib['outcome'] = "Failed"
    counters = elemTree.SubElement(resultsSummaryElement, "Counters")
    counters.attrib['total'] = str(len(rootDict['tests']))
    counters.attrib['warning'] = str(rootDict['succeededWithWarnings'])
    counters.attrib['failed'] = str(rootDict['failed'])
    counters.attrib['notExecuted'] = str(rootDict['notRun'])
    root.append(resultsSummaryElement)
    
    testListsElement = elemTree.Element("TestLists")
    allTestsList = elemTree.SubElement(testListsElement, "TestList")
    allTestsList.attrib['name'] = "Results Not in a List"
    allTestsList.attrib['id'] = "8c84fa94-04c1-424b-9868-57a2d4851a1d"
    root.append(testListsElement)
    
    testEntriesElement = elemTree.Element("TestEntries")    
    resultsElement = elemTree.Element("Results")
    testDefinitionsElement = elemTree.Element("TestDefinitions")
    for testResult in rootDict['tests']:
        unitTest = elemTree.SubElement(testDefinitionsElement, "UnitTest")
        unitTest.attrib['name'] = testResult['testDisplayName']
        unitTest.attrib['storage'] = testResult['fullTestPath']
        unitTest.attrib['id'] = str(uuid.uuid5(uuid.NAMESPACE_DNS, testResult['fullTestPath']))
        testEntry = elemTree.SubElement(testEntriesElement, "TestEntry") 
        testEntry.attrib['testId'] = unitTest.attrib['id']
        testEntry.attrib['testListId'] = allTestsList.attrib['id']
        unitTestResult = elemTree.SubElement(resultsElement, "UnitTestResult")
        unitTestResult.attrib['testName'] = testResult['testDisplayName']
        unitTestResult.attrib['testId'] = unitTest.attrib['id']
        unitTestResult.attrib['startTime'] = timesElement.attrib['start']
        unitTestResult.attrib['duration'] = formatDuration(rootDict['totalDuration'] / len(rootDict['tests']))
        unitTestResult.attrib['outcome'] = formatOutcome(testResult['state'])
        unitTestResult.attrib['computerName'] = "S1"
        unitTestResult.attrib['testListId'] = allTestsList.attrib['id']
        unitTestResultOutput = elemTree.SubElement(unitTestResult, "Output")
        unitTestResultOutputStd = elemTree.SubElement(unitTestResultOutput, "StdOut")
    root.append(testDefinitionsElement)
    root.append(resultsElement)
    root.append(testEntriesElement)

    tree = elemTree.ElementTree(root) 
    return tree
    
def writeXML(elementTree, outFile):
    with open(outFile, "wb") as files : 
        elementTree.write(files) 

def formatOutcome(outcomeFromUnreal):
    if(outcomeFromUnreal == "Success"):
        return "Passed"    
    return "Failed"
    
def formatDateTime(dateTimeFromUnreal):
    dateTime = dateTimeFromUnreal.split("-")
    dateComponents = dateTime[0].split(".")
    timeComponents = dateTime[1].split(".")
    return dateComponents[0] + "-" + dateComponents[1] + "-" + dateComponents[2] + "T" + timeComponents[0] + ":" + timeComponents[1] + ":" + timeComponents[2] + ".1337+01:00"

# "2020.12.05-12.10.19",
# 2012-02-19T09:25:24.8479038-05:00

def formatDuration(timeInSeconds):
    return str(datetime.timedelta(seconds=timeInSeconds))

inDir = str(sys.argv[1])
inFile = str(inDir + "\index.json")
outFile = str(sys.argv[2])

print("Input directory is " + inDir)
print("Input file is " + inFile)
print("Output file is " + outFile)

if(not os.path.isdir(inDir)):
    print("Input Directory is not a valid path")
    sys.exit(1)

if(not os.path.isfile(inFile)):
    print("Input Directory does not contain an index.json file")
    sys.exit(1)

with open(inFile, 'r', encoding='utf-8-sig') as jsonFile :
    testResults = json.load(jsonFile)
 
elementTree = generateEtree(testResults)
writeXML(elementTree, outFile)

print("##### End Python")

