import json, os

txtfiles = []
for root, dirs, files in os.walk("src/paths"):
    #txtfiles = list["/"([root, files])]
    for file in files:
        if file.endswith(".txt"):
            txtfiles.append("/".join([root, file]))
            #print(txtfiles[len(txtfiles)-1])


pfile2 = open(file='include/pypaths.hpp', mode='wt')
pfile2.writelines(
"""#pragma once
#include "vex.h"

class cpath {
public:
    cpath(){}
""")
    

for filename in txtfiles:
    pathfile = open(file=filename, mode='rt')
    pathtext = pathfile.read().split('#PATH.JERRYIO-DATA ')[1]
    pathloads = json.loads(pathtext)
    pathdict = json.loads(pathtext)['paths'][0]['segments']

    pfile2.write(f"    BezierChain {pathloads['paths'][0]['name']} = BezierChain(")
    pfile2.write("{")

    for i, segment in enumerate(pathdict):
        controls = segment['controls']
        pfile2.writelines('\n        CubicBezier(')
        for j, control in enumerate(controls):
            pfile2.write(f"VecPoint({control['x']}, {control['y']})")
            #print(str(controls).count("'uid':"))
            if j+1 != 4:            # there will always be 4 controls in each path
                pfile2.write(', ')
        pfile2.write('),')

    pfile2.writelines("\n    });\n")
    pfile2.write(f"    VelocityPointList {pathloads['paths'][0]['name']}_list = ")
    pfile2.write('{')
    with open(filename) as pfile:
        for line in pfile:
            if "endData" in line:
                break
            pfile2.writelines('\n        VelocityPoint(')
            pfile2.write(line.rstrip())
            pfile2.write('),')
    pfile2.writelines('\n    };\n')


pfile2.write('};')

pfile.close()
pfile2.close()
pathfile.close()