import java.io.*;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Stream;
import java.util.stream.Collectors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


import static java.lang.System.out;

class cmdexecutor implements Runnable
{
    private List<String> cmd;
    private String filename;
    public cmdexecutor(List<String> cmd, String filename)
    {
        this.cmd = cmd;
        this.filename = filename;
    }
    public void run()
    {
        List<String> esbmccommandline = this.cmd;
        BufferedReader br = null;
        try {
            System.out.println(esbmccommandline.toString());
            ProcessBuilder pb = new ProcessBuilder().command(esbmccommandline).inheritIO();
            Path outpath = Paths.get(this.filename + Instant.now().toString()+".interval");
            Path errpath = Paths.get(this.filename + Instant.now().toString()+".err");
            pb.redirectError(errpath.toFile());
            pb.redirectOutput(outpath.toFile());
            Instant start = Instant.now();
            pb.start().waitFor();
            Instant finish = Instant.now();
            long timeElapsed = Duration.between(start, finish).toMillis();
            System.out.println("execute finished in "+ timeElapsed + " ms");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if(br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

public class keras2c_injectfxp {

    public static int isInterval;

    public List<Path> listFiles(Path path) throws IOException {
        List<Path> result;
        try (Stream<Path> walk = Files.walk(path)) {
            result = walk.filter(Files::isRegularFile)
                    .collect(Collectors.toList());
        }
        return result;
    }

    public List<String> executepython(String tmpfilepath, String tmpfilename, String envfilepath, String pyfilename) {
        Runtime runtime = Runtime.getRuntime();
        BufferedReader br = null;
        List<String> lines = new ArrayList<>();
        try {
            File file = new File(tmpfilepath);
            File tmpFile = new File(tmpfilename);
            if (!file.exists()) {
                file.mkdirs();
            }
            if (!tmpFile.exists()) {
                tmpFile.createNewFile();
            }
            String[] commandlinekeras2c = new String[]{"C:\\Users\\DELL\\anaconda3\\envs\\tensorflow2\\python.exe", pyfilename};
            System.out.println(Arrays.toString(commandlinekeras2c));
            ProcessBuilder pb = new ProcessBuilder().command(commandlinekeras2c).inheritIO();
            pb.redirectErrorStream(true);
            pb.redirectOutput(tmpFile);
            pb.directory(new File(envfilepath));
            pb.start().waitFor();
            InputStream in = new FileInputStream(tmpFile);
            br = new BufferedReader(new InputStreamReader(in));
            String line = null;

            while ((line = br.readLine()) != null) {
                System.out.println(line);
                lines.add(line);
            }
            br.close();
            br = null;
            System.out.println("executed finished");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return lines;
    }

    public List<String> onnx2keras(String sourcefilepath, String outputpath) {
        String filename = ".././onnx2keras/converter.py";
        try {
            Files.deleteIfExists(Paths.get(filename));
            BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
            writer.write("import onnx");
            writer.newLine();
            writer.write("from onnx2keras import onnx_to_keras");
            writer.newLine();
            writer.write("import keras");
            writer.newLine();
            writer.write("import os");
            writer.newLine();
            writer.write("g = os.walk(r\"" + sourcefilepath + "\")\n");
            writer.newLine();
            writer.write("for path,dir_list,file_list in g:");
            writer.newLine();
            writer.write("    for file_name in file_list:");
            writer.newLine();
            writer.write("        with open(os.path.join(path, file_name), encoding=\"utf8\", errors='ignore') as f:");
            writer.newLine();
            writer.write("            onnx_model = onnx.load(os.path.join(path, file_name))");
            writer.newLine();
            writer.write("            linecache = f.readlines()[2]");
            writer.newLine();
            writer.write("            print(firstlayername)");
            writer.newLine();
            writer.write("            k_model = onnx_to_keras(onnx_model, [firstlayername])");
            writer.newLine();
            writer.write("            functionname = file_name.split(\".\")[0]");
            writer.newLine();
            writer.write("            k_model.save(functionname)");
            writer.newLine();
            writer.write("            keras.models.save_model(k_model," + outputpath + "functionname + '.h5' ,overwrite=True,include_optimizer=True)");
            writer.newLine();
            return (executepython("../", ".././onnx2keras.log", ".././onnx2keras", ".././onnx2keras/converter.py"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public void onnx2c(String sourcefile, String outputpath, String onnx_2cpath) {
        Runtime runtime = Runtime.getRuntime();
        BufferedReader br = null;
        List<String> lines = new ArrayList<>();
        try {
            String functionname = sourcefile.substring(0, sourcefile.lastIndexOf("."));
            String[] commandlineonnx2c = new String[]{onnx_2cpath, sourcefile, functionname};
            System.out.println(Arrays.toString(commandlineonnx2c));
            ProcessBuilder pb = new ProcessBuilder().command(commandlineonnx2c).inheritIO();
            pb.redirectErrorStream(true);
            pb.start().waitFor();
            out.println("executed finished");
        } catch (Exception e) {

            e.printStackTrace();
        }
    }

    public List<String> Keras2c(String sourcefile, String functionname) {
        Runtime runtime = Runtime.getRuntime();
        BufferedReader br = null;
        List<String> lines = new ArrayList<>();
        try {
            File file = new File(".././keras2c");
            File tmpFile = new File(".././keras2clog.log");
            if (!file.exists()) {
                file.mkdirs();
            }
            if (!tmpFile.exists()) {
                tmpFile.createNewFile();
            }
            String[] commandlinekeras2c = new String[]{"python3", "-m", "keras2c", sourcefile, functionname};
            out.println(Arrays.toString(commandlinekeras2c));
            ProcessBuilder pb = new ProcessBuilder().command(commandlinekeras2c).inheritIO();
            pb.redirectErrorStream(true);
            pb.redirectOutput(tmpFile);
            pb.directory(new File(".././keras2c"));
            pb.start().waitFor();
            InputStream in = new FileInputStream(tmpFile);
            br = new BufferedReader(new InputStreamReader(in));
            String line = null;
            while ((line = br.readLine()) != null) {
                out.println(line);
                lines.add(line);
            }
            br.close();
            br = null;
            out.println("executed finished");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return lines;
    }

    public void replace_content_all(File file, String target, String newContent) {
        try {
            InputStream is = new FileInputStream(file);
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(is));
            String filename = file.getName();
            File tmpfile = new File(file.getParentFile().getAbsolutePath()
                    + "\\" + filename + ".tmp");
            BufferedWriter writer = new BufferedWriter(new FileWriter(tmpfile));
            boolean flag = false;
            String str = null;
            while (true) {
                str = reader.readLine();
                if (str == null)
                    break;
                if (str.contains(target)) {
                    writer.write(newContent + "\n");
                    flag = true;
                } else
                    writer.write(str + "\n");
            }
            is.close();
            writer.flush();
            writer.close();
            if (flag) {
                file.delete();
                tmpfile.renameTo(new File(file.getAbsolutePath()));
            } else
                tmpfile.delete();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void replace_one_line(File file, String target, String newContent) {
        try {
            InputStream is = new FileInputStream(file);
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(is));
            String filename = file.getName();
            File tmpfile = new File(file.getParentFile().getAbsolutePath()
                    + "\\" + filename + ".tmp");
            BufferedWriter writer = new BufferedWriter(new FileWriter(tmpfile));
            boolean flag = false;
            String str = null;
            while (true) {
                str = reader.readLine();
                if (str == null)
                    break;
                if (str.contains(target)) {
                    writer.write(newContent + "\n");
                    flag = true;
                    break;
                } else
                    writer.write(str + "\n");
            }
            is.close();
            writer.flush();
            writer.close();
            if (flag) {
                file.delete();
                tmpfile.renameTo(new File(file.getAbsolutePath()));
            } else
                tmpfile.delete();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void generate_Iris_Intervals(Path pathdir) throws IOException, InterruptedException {
        List<Path> paths = listFiles(pathdir);
        ExecutorService executor = Executors.newFixedThreadPool(5);
        paths.forEach(x -> {
            String extension = "";
            String name = x.toString();
            extension = name.substring(name.lastIndexOf("."));
            if (extension.equals(".c") && name.contains("annIris") && !name.contains("interval")) {
                out.println(name);
                List<String> execcmd = new ArrayList<>();
                execcmd.add("frama-c");
                execcmd.add("-eva");
                execcmd.add("-eva-plevel 255");
                execcmd.add(name);
                cmdexecutor cmdexec = new cmdexecutor(execcmd, x.getFileName().toString());
                executor.execute(cmdexec);
            }
        });
    }

    public void generate_ACAS_Intervals(Path pathdir) throws IOException {
        List<Path> paths = listFiles(pathdir);
        ExecutorService executor = Executors.newFixedThreadPool(5);
        paths.forEach(x -> {
            String name = x.toString();
            if (name.contains(".c") && name.contains("ACASXU") && !name.contains("interval")) {
                out.println(name);
                out.println("running command: frama-c -eva " + name);
                List<String> execcmd = new ArrayList<>();
                execcmd.add("frama-c");
                execcmd.add("-eva");
                execcmd.add(name);
                cmdexecutor cmdexec = new cmdexecutor(execcmd, pathdir.toString());
                executor.execute(cmdexec);
            }
        });
    }

    public String breakframacline(String framacline, String char1, String char2) {
        return (framacline.substring(framacline.indexOf(char1) + 1, framacline.lastIndexOf(char2)));
    }

    public List<String> castFramacToEsbmc(String framacline) {

        String value = "";
        String neuronmin = "";
        String neuronmax = "";
        List<String> outputlist = new ArrayList<>();
        List<String> neuronlist = new ArrayList<>();
        String[] parts = framacline.split("\u2208");
        if (parts[0].contains("..")) {
            String neuron = framacline.substring(0, framacline.lastIndexOf("["));
            String startindex = framacline.substring(framacline.indexOf("[") + 1, framacline.lastIndexOf(".."));
            String endindex = framacline.substring(framacline.indexOf("..") + 2, framacline.lastIndexOf("]"));
            for (int i = Integer.valueOf(startindex); i < Integer.valueOf(endindex); i++) {
                neuronlist.add(neuron + "[" + i + "]");
            }
        } else {
            neuronlist.add(framacline.substring(0, framacline.lastIndexOf(" \u2208")));
        }
        String[] parts2 = parts[0].split("\u2208");
        if (parts2[1].contains("[")) {
            //these are old intervals generated by frama-c, should be substituted
            neuronmin = parts2[1].substring(parts2[1].indexOf("\u2208 [") + 3, parts2[1].lastIndexOf(" .. "));
            neuronmax = parts2[1].substring(parts2[1].lastIndexOf(" .. ") + 4, -2);
            //neuronmax = framacline[framacline.find(" .. ") + 4 : -2]
        } else {
            isInterval = 0;
            value = parts2[1].substring(parts2[1].indexOf("\u2208 {") + 4, parts2[1].lastIndexOf(" .. "));
        }
        String finalNeuronmin = neuronmin;
        String finalNeuronmax = neuronmax;
        neuronlist.forEach(neuron -> {
            if (isInterval == 0) {
                outputlist.add("__ESBMC_assume((" + neuron + " >= " + finalNeuronmin + ") && (" + neuron + " <= " + finalNeuronmax + "));");
            }
        });
        out.println(outputlist);
        return outputlist;

    }

    public List<String> summarize_Interval(Path filepath) throws IOException {
        int foundlayer = 0;
        String lastlayer = "";
        int counter = 0;
        List<String> esbmcNeuronIntervalList = new ArrayList<>();
        List<String> allLines = Files.readAllLines(filepath);
        for (String line : allLines) {
            String framacline = "";
            if (line.contains("layer")) {
                foundlayer = 1;
            }
            if (line.contains("layer") && foundlayer == 1) {
                lastlayer = line.substring(0, line.lastIndexOf("[") + 1);
                framacline = line;
                esbmcNeuronIntervalList.addAll(castFramacToEsbmc(framacline.substring(framacline.lastIndexOf("layer"))));
                counter++;

            } else if (line.contains("[") && line.contains("]") && foundlayer == 1 && !line.contains("ANALYSIS SUMMARY")) {
                framacline = lastlayer + line.substring(line.indexOf("["));
                esbmcNeuronIntervalList.addAll(castFramacToEsbmc(framacline.substring(framacline.lastIndexOf("layer"))));
                counter++;
            }
        }
        out.println("I counted " + esbmcNeuronIntervalList.size() + " intervals on file: " + filepath);
        return esbmcNeuronIntervalList;
    }

    public List<String> getEsbmcIntervalFromNeuron(String neuron, List<String> esbmcassumes) {
        List<String> esbmcassume = new ArrayList<>();
        esbmcassumes.forEach(x -> {
            if (x.contains(neuron)) {
                esbmcassume.add(x);
            }
        });
        return esbmcassume;
    }

    public void importInterval(Path esbmcfile, List<String> esbmcassumes) throws IOException {
        List<String> esbmclines = new ArrayList<>();
        esbmclines = Files.readAllLines(esbmcfile);
        FileWriter writer = new FileWriter(esbmcfile.toString());
        BufferedWriter bw = new BufferedWriter(writer);
        esbmclines.forEach(x -> {
            if (x.contains("tanhFunctionLUT")) {
                try {
                    bw.write(x.toString());
                    //bw.write(getEsbmcIntervalFromNeuron(esbmcline[0:esbmcline.find(" =")], esbmcassumes)+ "\n")
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                try {
                    bw.write(x.toString());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        bw.close();
        writer.close();
    }

    public void importInterval_cifar(Path esbmcfile, List<String> esbmcassumes) throws IOException {

    }

    public void importInterval_mnist(Path esbmcfile, List<String> esbmcassumes) throws IOException {
        List<String> esbmclines = new ArrayList<>();
        esbmclines = Files.readAllLines(esbmcfile);
        FileWriter writer = new FileWriter(esbmcfile.toString());
        BufferedWriter bw = new BufferedWriter(writer);
        /*esbmclines.forEach(x->{
            if(x.contains("void entry"))
            {

            }
        }*/
    }

    public List<String> castFramacToEsbmc_onnx(String framacline) {
        String value = "";
        String neuronmin = "";
        String neuronmax = "";
        List<String> outputlist = new ArrayList<>();
        List<String> neuronlist = new ArrayList<>();
        String[] parts = framacline.split("\u2208");
        if (parts[0].contains("..")) {

        }
        return outputlist;
    }


    public List<String> summarizeIntervals_onnx(Path filepath) throws IOException
    {
        int foundlayer = 0;
        String lastlayer = "";
        int counter = 0;
        List<String> esbmcNeuronIntervalList = new ArrayList<>();
        List<String> allLines;
        allLines = Files.readAllLines(filepath);
        for (String line : allLines) {
            String framacline = "";
            if (line.contains("tensor")) {
                foundlayer = 1;
            }
            if (line.contains("tensor") && foundlayer == 1) {
                lastlayer = line.substring(0, line.lastIndexOf("[") + 1);
                framacline = line;
                esbmcNeuronIntervalList.addAll(castFramacToEsbmc_onnx(framacline.substring(framacline.lastIndexOf("tensor"))));
                //need to rewrite the framactoesbmc in onnx2c format
                counter++;

            } else if (line.contains("[") && line.contains("]") && foundlayer == 1 && !line.contains("ANALYSIS SUMMARY")) {
                framacline = lastlayer + line.substring(line.indexOf("["));
                esbmcNeuronIntervalList.addAll(castFramacToEsbmc_onnx(framacline.substring(framacline.lastIndexOf("tensor"))));
                //need to rewrite the framactoesbmc in onnx2c format
                counter++;
            }
        }
        out.println("I counted " + esbmcNeuronIntervalList.size() + counter + " intervals on file: " + filepath);
        return esbmcNeuronIntervalList;
    }
    public void importIntervalAcasXU(Path esbmcfile, List<String> esbmcassumes) throws IOException //import intervals from frama-c
    {
        List<String> esbmclines = new ArrayList<>();
        esbmclines = Files.readAllLines(esbmcfile);
        FileWriter writer = new FileWriter(esbmcfile.toString());
        File file = new File(esbmcfile.toString());
        InputStream is = new FileInputStream(file);
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(is));
        // tmpfile is a cache file, after substitution, it will replace the original file
        File tmpfile = new File(file.getParentFile().getAbsolutePath()
                + "\\" + file + ".tmp");
        boolean flag = false;
        String str = null;
        while (true) {
            str = reader.readLine();

            if (str == null)
                break;

            if (str.contains("if") && str.contains("< 0") && str.contains("ANALYSIS SUMMARY")) {
                if (!str.contains("node_linear_7_Add"))
                    try {
                        writer.write(str + "\n");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                else if(str.contains("node_linear_7_Add"))
                {
                    for (String x : esbmclines) {
                        String newContent = new String();
                        newContent = String.valueOf(getEsbmcIntervalFromNeuron(x.substring(x.lastIndexOf("(") + 1, x.lastIndexOf(" <")), esbmcassumes));
                        try {
                            writer.write(newContent + "\n");
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
                else{
                    try {
                        writer.write(str + "\n");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
            is.close();
            writer.flush();
            writer.close();
            if (flag) {
                file.delete();
                tmpfile.renameTo(new File(file.getAbsolutePath()));
            } else
                tmpfile.delete();
        }
    }
    public void managercastEsbmcToFramac(Path filepath) throws IOException
    {

        List<Path> paths = listFiles(filepath);
        ExecutorService executor = Executors.newFixedThreadPool(5);
        paths.forEach(x -> {
            String extension = "";
            String name = x.toString();
            extension = name.substring(name.lastIndexOf("."));
            if(extension.equals(".c") && name.contains("ACASXU") && !name.contains(".interval") && !name.contains("framac.c"))
            {
                try {
                    generate_framac_annotations(x.toString());
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        });
    }

    public void generate_framac_annotations(String filepath) throws IOException
    {
        try {
            String pathtowrite = filepath + Instant.now().hashCode() + ".c";
            List<String> minlist = new ArrayList<>();
            List<String> maxlist = new ArrayList<>();
            List<String> indexlist = new ArrayList<>();
            BufferedReader br = new BufferedReader(new FileReader(filepath));
            File file = new File(filepath);
            InputStream is = new FileInputStream(file);
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(is));
            String filename = file.getName();
            // tmpfile is a cache file, after substitution, it will replace the original file
            File tmpfile = new File(file.getParentFile().getAbsolutePath()
                    + "\\" + filename + ".tmp");
            BufferedWriter bw = new BufferedWriter(new FileWriter(tmpfile));

            boolean flag = false;
            String str = null;
            out.println(pathtowrite);
            String x = null;
            x = br.readLine();
            out.println(x);
            while (true) {
                x = reader.readLine();
                out.println(x);
                if (x.contains("__fc_builtin")) {
                    out.println("contains __fc_builtin");
                    bw.write(x.replaceFirst("//", ""));
                    bw.newLine();
                } else if (x.contains("__ESBMC_assume")) {
                    out.println("contains esbmc assume");
                    bw.write(x.replaceFirst("__ESBMC", "//__ESBMC"));
                    bw.newLine();
                    minlist.add(x.substring(x.indexOf(">=")+2, x.indexOf("&&")));
                    maxlist.add(x.substring(x.indexOf("<=")+2, x.lastIndexOf(");")));
                    indexlist.add(x.substring(x.indexOf("(")+2, x.lastIndexOf(">=")));
                } else if (x.contains("nondet_float") || x.contains("nondet_int")) {
                    bw.write("//" + x);
                    bw.newLine();
                } else if (x.contains("float c1")) {
                    for (int i = 0; i < minlist.size(); i++) {
                        bw.write("float" + indexlist.get(i) + " = Frama_C_float_interval(" + minlist.get(i) + "," + maxlist.get(i) + ");\n");
                        bw.newLine();
                    }
                    bw.write("\n" + x);
                    bw.newLine();
                } else if (x.contains("utils.h")) {
                    bw.write("//" + x);
                    bw.newLine();
                } else if (x.contains("__ESBMC_assert")) {
                    bw.write("//" + x);
                    bw.newLine();
                    bw.write("//@assert(" + x.substring(x.indexOf("(") + 1, x.indexOf(",")) + ");");
                    bw.newLine();
                } else if (x == null) {
                    break;
                } else
                    bw.write(x);
                bw.newLine();
            }
            is.close();
            bw.flush();
            bw.close();
            if (flag) {
                file.delete();
                tmpfile.renameTo(new File(file.getAbsolutePath()));
            } else {
                tmpfile.delete();
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
    public static void main(final String[] arguments) throws IOException {
        keras2c_injectfxp converter = new keras2c_injectfxp();
        //firstly deal with the cmd args
        if (arguments.length != 3)
        {
            System.out.println("The length of your arguments is not enough.");
            System.out.println("This tool is for converting models to C files and insert properties automatically.");
            System.out.println("Models will be converted to C files via keras2c, and the properties are from vnnlib.");
            System.out.println("usage:");
            System.out.println("-onnx2keras PATHTOONNX OUTPUTPATH: The input models are in onnx format, and the PATH must be a file path that contains your onnx models");
            System.out.println("-onnx2c PATHTOONNX OUTPUTPATH: The input models are in onnx format, and the PATH must be a file path that contains your onnx models");
            System.out.println("-h PATHTOH5 OUTPUTPATH: The input models are in h5 format, and the PATH must be a file path that contains your h5 models");
            System.out.println("-p PATHTOPROPERTIES OUTPUTPATH: The properties are in vnnlib, they will be automatically injected into c files after Invariant generation");
            System.out.println("-properties PATHTOCFILES OUTPUTPATH: The properties are already inserted to the C files, run frama-c to generate the intervals");

        }
        else if(arguments[0].contains("onnx2keras"))
        {
            System.out.println("The provided onnx path is '" + arguments[1] +"'.");
            //deal with the onnx path
            converter.onnx2keras(arguments[1], arguments[2]);

        }
        else if(arguments[0].contains("h"))
        {
            System.out.println("The provided h5 path is " + arguments[1] + ".");
            System.out.println("The output path is " + arguments[2] + ".");
            converter.generate_ACAS_Intervals(Path.of(arguments[1]));
        }
        else if(arguments[0].contains("onnx2c"))
        {
            System.out.println("The provided onnx path is " + arguments[1] + ".");
            System.out.println("The output path is " + arguments[2] + ".");
            converter.onnx2c(arguments[1],arguments[2], "/mnt/c/projects/onnx2c/build/onnx2c.o");
        }
        else if(arguments[0].contains("properties"))
        {
            System.out.println("The provided c file path is " + arguments[1] + ".");
            System.out.println("The output path is " + arguments[2] + ".");
            converter.generate_ACAS_Intervals(Path.of(arguments[1]));
        }
        else if(arguments[0].contains("import"))
        {
            System.out.println("The provided c file path is " + arguments[1] + ".");
            System.out.println("The provided intervals file path is " + arguments[2] + ".");

        }
        else if(arguments[0].contains("summarize"))
        {
            System.out.println("The provided interval file path is " + arguments[1] + ".");
            System.out.println("The output path is " + arguments[2] + ".");
            converter.summarizeIntervals_onnx(Path.of(arguments[1]));
        }
        /*else if(arguments[0].contains("framac"))
        {
            System.out.println("The provided vnnlib path is " + arguments[1] + ".");
            System.out.println("The provided C file path is " + arguments[2] + ".");
        }*/
        else
        {

        }
    }
}