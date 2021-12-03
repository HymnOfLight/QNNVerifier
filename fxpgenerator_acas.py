import os
import glob
import pretext
import errno

def getfxpline(expression, currentLayerSize, previousLayerSize, neuronIndex, layerIndex):
    result = f"\nfloat w{layerIndex}{neuronIndex}[{previousLayerSize}] = " + "{"
    expressions = expression.split("+")
    for i in range(0, int(previousLayerSize)):
        if i != 0:
            result += ","
        findw = False
        for operation in expressions:
            if f"[{i}]" in operation:
                findw = True
                weight = operation[:operation.find("*")].replace("(", "")
                weight = weight.replace(")", "")
                result += weight
        if findw == False:
            result += "0"
    result += "};\n"

    result += f"float b{layerIndex}{neuronIndex} = "
    if "[" in expressions[-1] and "]" in expressions[-1]:
        result += "0;\n"
    else:
        result += expressions[-1].replace("(", "").replace(")", "") + "\n"
    if layerIndex == 1:
        result += f"fxp_t r{layerIndex}{neuronIndex} = performf(w{layerIndex}{neuronIndex}, {previousLayerSize}, i, {previousLayerSize},b{layerIndex}{neuronIndex});\n"
    else:
        result += f"fxp_t r{layerIndex}{neuronIndex} = performf(w{layerIndex}{neuronIndex}, {previousLayerSize}, layer{layerIndex-1}, {previousLayerSize},b{layerIndex}{neuronIndex});\n"

    result += f"layer{layerIndex}[{neuronIndex}] = r{layerIndex}{neuronIndex};\n"
    return result

def castCondition(cline):
    element1 = cline[cline.find("(")+1: cline.find(">")].strip()
    element1index = element1[element1.find("[")+1: element1.find("]")]
    element2 = cline[cline.find(">")+1: cline.find(")")].strip()
    result = f"if (fxp_to_float({element1}) > fxp_to_float({element2})) r = {element1index};\n"
    return result

def castAssume(cline):
    result = ""
    if "&&" in cline:
        string1 = cline[cline.find(">="): cline.find(")")]
        string1r = string1.replace(">=", "").replace(")", "").strip()
        string2 = cline[cline.find("<="): cline.find("))")]
        string2r = string2.replace("<=", "").replace("))", "").strip()
        result = cline.replace(string1, f">= fxp_float_to_fxp({string1r})")
        result = result.replace(string2, f"<= fxp_float_to_fxp({string2r})")
        
    else:
        if "==" in cline:   
            string1 = cline[cline.find("=="): cline.find(")")].strip()
            result = cline.replace(string1, f"== fxp_float_to_fxp({string1[2:]})")
        else:
            string1 = cline[cline.find("="): cline.find(")")].strip()
            result = cline.replace(string1, f"= fxp_float_to_fxp({string1[1:]})")

    return result

def processAcasXuFile(cfile, genfile):
    print(cfile)
    esbmcfile = open(cfile, 'r')
    esbmclines = esbmcfile.readlines()
    esbmcfile.close()
    doingNormalization = False
    indexNorm = ""
    currentLayerSize = ""
    previousLayerSize = ""
    #currentLayerName = "layer"
    neuronIndex = ""
    layerIndex = 0
    with open(genfile[:-2]+"fxp.c", 'w') as f:
        #f.write(pretext.pretext)
        for index, item in enumerate(esbmclines):
            cline = item
            if "#include" in cline or "// " in cline:
                continue
            elif "main()" in cline:
                f.write(cline)
                #f.write(pretext.pretext2)
            elif doingNormalization == True and "i[" in cline and "/" in cline:
                currentExpression = cline[cline.find("=")+2:-2]
                f.write(cline[0:cline.find("=")+2] + f"fxp_float_to_fxp({currentExpression});\n")
            elif doingNormalization == True and "i[" not in cline:
                doingNormalization = False
            elif "float i[5];" in cline:
                doingNormalization = True
                f.write("\nfxp_t i[5];\n")
                currentLayerSize = "5"
            elif "float layer" in cline and "];" in cline:
                layerIndex += 1
                previousLayerSize = currentLayerSize
                currentLayerSize = cline[cline.find("[")+1:cline.find("]")]
                f.write("\n"+cline.replace("float", "fxp_t"))
            elif "layer" in cline and "if" not in cline and "+" in cline and "*c2)+c1;" not in cline:
                neuronIndex = cline[cline.find("[")+1: cline.find("]")]
                expression = cline[cline.find("=")+1:].strip()
                f.write(getfxpline(expression, currentLayerSize, previousLayerSize, neuronIndex, layerIndex))
            elif "*c2)+c1;" in cline:
                currentNeuronText = cline[0:cline.find("=")]
                f.write(currentNeuronText + " = fxp_add(fxp_mult(" + currentNeuronText + ", fxp_float_to_fxp(c2)),fxp_float_to_fxp(c1));\n" )
            elif "sigmoid" in cline:
                continue
            elif "if" in cline and "[r]" in cline and ">" in cline:
                f.write(castCondition(cline))
            elif "__ESBMC_assume" in cline and doingNormalization == False and "layer" in cline:
                f.write(castAssume(cline))
            else:
                f.write(cline)
        f.flush()
    f.close()

def main():
    #cfiles = glob.glob(path + "/*/*/*/*.c", recursive = True)
    #for cfile in sorted(cfiles):
    cfile = input("Please specify the floating-point file")
    genfile = input("Please specify the targeted folder")
    if "/ACASXU_Reluplex" in cfile and "/ACASXU_Reluplex_fxp" not in cfile:
        if not os.path.exists(os.path.dirname(genfile)):
            try:
                os.makedirs(os.path.dirname(genfile))
                print("Created dir: "+ os.path.dirname(genfile))
            except OSError as exc: # Guard against race condition
                if exc.errno != errno.EEXIST:
                    raise
    processAcasXuFile(cfile, genfile)
        #if "annVocalic" in os.path.basename(cfile) and "fxp" not in os.path.basename(cfile):
        #    processAcasXuFile(cfile)
    
if __name__ == "__main__":
    main()