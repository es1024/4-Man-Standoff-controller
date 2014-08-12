import sys, random
targetData = sys.argv[2].split()[random.randint(0,3)]
print(targetData.split(',')[len(targetData.split(','))-1])
