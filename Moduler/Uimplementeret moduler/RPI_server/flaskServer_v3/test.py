if __name__ == "__main__":
    testDict = {
        1234: 1,
        3210: 1,
        9999: 0,
        9812: 1
    }
    id = 0
    number = None
    testList = list(testDict.values())
    for i in range(len(testList)):
        if(testList[i] == 0):
            number = i
            break
    print(testDict)
    toBeUpdated = list(testDict.keys())[number]
    testDict.update({toBeUpdated:1})
    print(testDict)