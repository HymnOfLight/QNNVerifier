from os import listdir
import os
import sys
import getopt
from subprocess import run
import argparse

def main(argv):
    #try:
    print(sys.argv[1])
    #managerAcasXUImporting(pathc)
    if sys.argv[1] == '-h':
        print("Usage: Intervalgenerator.py -mode <inputfile>")
        print("-h help")
        print("-f1 run frama-c to generate Intervals for the C file in the folder (Other)")
        print("-f2 run frama-c to generate Intervals for the C file in the folder (ACAS)")
        print("-e1 insert Other frama-c intervals(.interval) to C files")
        print("-e2 insert ACAS frama-c intervals(.interval) to C files")
        print("-m make the C files that can be analyse by frama-c")
        print("-g make the C files that can be analyse by esbmc\n")
    elif sys.argv[1] == '-f1':
        pathc = input("Please type the targeted folder\n")
        generateIntervals(pathc)
    elif sys.argv[1] == '-f2':
        pathc = input("Please type the targeted folder\n")
        generateIntervalsAcasXU(pathc)
    elif sys.argv[1] == '-e1':
        pathframacp8 = input("Please type the targeted file\n")
        print(pathframacp8)
        esbmcAssumes = sumarizeInterval(pathframacp8)
        esbmcFile = pathframacp8[0:pathframacp8.find(".interval")] + ".c"
        importInterval(esbmcFile, esbmcAssumes)
    elif sys.argv[1] == '-e2':
        pathframacp9 = input("Please type the targeted file\n")
        print(pathframacp9)
        esbmcAssumes = sumarizeInterval(pathframacp9)
        esbmcFile = pathframacp9[0:pathframacp9.find(".interval")] + ".c"
        importIntervalAcasXU(esbmcFile, esbmcAssumes)
    elif sys.argv[1] == '-m':
        pathc = input("Please type the targeted folder\n")
        castEsbmcAcasToFramac(pathc)
    elif sys.argv[1] == '-g':
        pathc = input("Please type the targeted file\n")
        path = input("Please specify the targeted folder\n")
        castEsbmcAcasToFramac(pathc)
        generateIntervalsAcasXU(path)
        esbmcAssumes = sumarizeInterval(pathc)
        esbmcFile = pathc[0:path.find(".interval")] + "c"
        importIntervalAcasXU(esbmcFile, esbmcAssumes)
    else:
        print("Invalid parameters, please use Intervalgenerator.py -h")
    #except:
        #print("Invalid parameters or filepath, please use Intervalgenerator.py -h and check the input")
        #sys.exit()

def generateIntervals(pathdir):
    for f in listdir(pathdir):
        f = os.path.join(pathdir, f)
        print(f)
        if os.path.isfile(f):
            if "annIris" in f and ".c" in f and "interval" not in f:
                os.system("frama-c -eva -eva-plevel 255 " + f  + " > " + f[:-2] + ".interval")


def cleanIntervalsAndFramacFiles(pathdir):
    for d in sorted(listdir(pathdir)):
        d = os.path.join(pathdir, d)
        if os.path.isdir(d):
            for f in sorted(listdir(d)):
                f = os.path.join(d, f)
                if os.path.isfile(f):
                    if "ACASXU" in f and ("framac" in f or "interval" in f):
                        print("deleting: " + f)
                        os.system("rm "+ f)

def generateIntervalsAcasXU(pathdir):
    for d in listdir(pathdir):
        d = os.path.join(pathdir, d)
        if os.path.isdir(d):
            for f in sorted(listdir(d)):
                f = os.path.join(d, f)
                if os.path.isfile(f):
                    if "ACASXU" in f and "framac" in f and "interval" not in f:
                        print(f)
                        print("running command: frama-c -eva " + f  + " > " + f.replace("framac.c",".interval"))
                        run("frama-c -eva " + f  + " > " + f.replace("framac.c",".interval"), shell=True, cwd=d)

def generateIntervalsAcasXUonDir(pathdir):
    for f in sorted(listdir(pathdir)):
        f = os.path.join(pathdir, f)
        if os.path.isfile(f):
            if "ACASXU" in f and "framac.c" in f and "interval" not in f:
                print(f)
                print("running command: frama-c -eva " + f  + " > " + f.replace("framac.c",".interval"))
                run("frama-c -eva " + f  + " > " + f.replace("framac.c",".interval"), shell=True, cwd=pathdir)

def countSumarizedIntervals(pathbench):
    for f in listdir(pathbench):
        f = os.path.join(pathbench, f)
        if os.path.isfile(f):
            if "annIris" in f and ".c" not in f and ".interval" in f:
                listre = sumarizeInterval(f)
                for element in listre:
                    print(element)


def sumarizeInterval(filepath):
    foundlayer = False
    lastlayer = " "
    counter = 0
    esbmcNeuronIntervalList = []
    with open(filepath, 'r') as f:
        for line in f.readlines():
            framacline = " "
            if "layer" in line:
                foundlayer = True
            if foundlayer and "layer" in line:
                lastlayer = line[0:line.find('[')]
                framacline = line
                esbmcNeuronIntervalList.extend(castFramacToEsbmc(framacline[framacline.find("layer"):]))
                counter += 1
            elif foundlayer and "[" in line and "]" in line and "ANALYSIS SUMMARY" not in line:
                framacline = lastlayer + line[line.find('['):]
                esbmcNeuronIntervalList.extend(castFramacToEsbmc(framacline[framacline.find("layer"):]))
                counter += 1
        f.close()
    print("I counted "+ str(len(esbmcNeuronIntervalList)) + " intervals on file: " + filepath)
    return esbmcNeuronIntervalList

def castFramacToEsbmc(framacline):
    outputlist = []
    neuronlist = []
    neuronmin = ""
    neuronmax = ""
    value = ""
    isInterval = True
    if ".." in framacline[0:framacline.find(" ∈")]:
        neuron = framacline[0:framacline.find("[")]
        startindex = framacline[framacline.find("[")+1:framacline.find("..")]
        endindex  = framacline[framacline.find("..")+2:framacline.find("]")]
        for x in range(int(startindex), int(endindex)+1) :
            neuronlist.append(neuron + "[" + str(x) + "]")
    else:
        neuronlist.append(framacline[0:framacline.find(" ∈")].strip())
    if "[" in framacline[framacline.find(" ∈")+2:]:
            neuronmin = framacline[framacline.find("∈ [") + 3:framacline.find(" .. ")]
            neuronmax = framacline[framacline.find(" .. ") + 4 : -2]
    else:
        isInterval = False
        value = framacline[framacline.find("∈ {") + 3:framacline.find("}")]

    for neuron in neuronlist:
        if isInterval:
            outputlist.append("__ESBMC_assume(("+ neuron + " >= " + neuronmin + ") && ("+neuron + " <= " + neuronmax +"));")
        else:
            outputlist.append("__ESBMC_assume("+ neuron + " == " + value + ");")
    return outputlist


def manager(filebenchmarks):
    for f in listdir(filebenchmarks):
        f = os.path.join(filebenchmarks, f)
        if os.path.isfile(f):
            if "annIris" in f and ".c" not in f and ".interval" in f:
                esbmcAssumes = sumarizeInterval(f)
                esbmcFile = f[0:f.find(".interval")] + ".c"
                importInterval(esbmcFile, esbmcAssumes)


def importInterval(esbmcfile, esbmcassumes):
    esbmclines = []
    with open(esbmcfile, 'r') as f:
        esbmclines = f.readlines()
        f.close()
    esbmcbuff = open(esbmcfile, 'w')
    for esbmcline in esbmclines:
        if "tanhFunctionLUT" in esbmcline:
            esbmcbuff.write(esbmcline)
            esbmcbuff.write(getEsbmcIntervalFromNeuron(esbmcline[0:esbmcline.find(" =")], esbmcassumes)+ "\n")
        else:
            esbmcbuff.write(esbmcline)
    esbmcbuff.close()

def managerAcasXUImporting(filebenchmarks):
    for d in sorted(listdir(filebenchmarks)):
        d = os.path.join(filebenchmarks, d)
        if os.path.isdir(d):
            for f in sorted(listdir(d)):
                f = os.path.join(d, f)
                if os.path.isfile(f):
                    if "ACASXU" in f and ".c" not in f and ".interval" in f:
                        esbmcAssumes = sumarizeInterval(f)
                        esbmcFile = f[0:f.find(".interval")] + ".c"
                        importIntervalAcasXU(esbmcFile, esbmcAssumes)

def importIntervalAcasXU(esbmcfile, esbmcassumes):
    esbmclines = []
    with open(esbmcfile, 'r') as f:
        esbmclines = f.readlines()
        f.close()
    esbmcbuff = open(esbmcfile, 'w')
    for esbmcline in esbmclines:
        if "if" in esbmcline and "< 0" in esbmcline and "= 0;" in esbmcline:
            if "layer7" not in esbmcline:
                esbmcbuff.write(esbmcline)
                esbmcbuff.write(getEsbmcIntervalFromNeuron(esbmcline[esbmcline.find("(")+1:esbmcline.find(" <")], esbmcassumes)+ "\n")
        elif "*c2)+c1;" in esbmcline and "layer7" in esbmcline:
            esbmcbuff.write(esbmcline)
            esbmcbuff.write(getEsbmcIntervalFromNeuron(esbmcline[0:esbmcline.find(" =")], esbmcassumes)+ "\n")
        else:
            esbmcbuff.write(esbmcline)
    esbmcbuff.close()

def importIntervalcifar(esbmcfile, esbmcassumes):
    esbmclines = []
    with open(esbmcfile, 'r') as f:
        esbmclines = f.readlines()
        f.close()
    esbmcbuff = open(esbmcfile, 'w')
    for esbmcline in esbmclines:
        if ""

def getEsbmcIntervalFromNeuron(neuron, esbmcassumes):
    for esbmcassume in esbmcassumes:
        if neuron in esbmcassume:
            return esbmcassume

def managerCastAcasXUToEsbmc(filepath):
    for d in listdir(filepath):
        d = os.path.join(filepath, d)
        if os.path.isdir(d):
            for f in listdir(d):
                f = os.path.join(d, f)
                if os.path.isfile(f) and "ACASXU" in f and ".c" in f and ".interval" not in f and "framac.c" not in f:
                    castEsbmcAcasToFramac(f)

def castEsbmcAcasToFramac(filepath):
    pathtowrite = filepath.replace(".c", "framac.c")
    esbmclines = []
    minlist = []
    maxlist = []
    indexlist = []
    with open(filepath, 'r') as f:
        esbmclines = f.readlines()
        f.close()
    esbmcbuff = open(pathtowrite, 'w')
    for line in esbmclines:
        if "__fc_builtin" in line:
            esbmcbuff.write(line.replace("//", ""))
        elif "__ESBMC_assume" in line:
            esbmcbuff.write(line.replace("__ESBMC", "//__ESBMC"))
            minlist.append(line[line.find(">=")+2:line.find("&&")])
            maxlist.append(line[line.find("<=")+2:line.find(");")])
            indexlist.append(line[line.find("(")+1:line.find(">=")].strip())
        elif "nondet_float" in line or "nondet_int" in line:
            esbmcbuff.write("//" + line)
        elif "float c1" in line:
            for x in range(0, len(minlist)):
                esbmcbuff.write("float "+ indexlist[x] + " = Frama_C_float_interval(" + minlist[x] + "," + maxlist[x] + ");\n")
            esbmcbuff.write("\n" + line)
        elif "utils.h" in line:
            esbmcbuff.write("//" + line)
        elif "__ESBMC_assert" in line:
            esbmcbuff.write("//" + line)
            esbmcbuff.write("//@assert(" + line[line.find("(")+1: line.find(",")] + ");\n")
            esbmcbuff.write("}\n")
            break
        else:
            esbmcbuff.write(line)
    esbmcbuff.close()

if __name__ == "__main__":
    main(sys.argv[1:])