import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.FileWriter;

public class keras2c_injectfxp{

    public static List<String> onnx2keras(String sourcefile, String functionname)
    {
        File file = new File(aourcefile);
        FileInputStream stream = new java.io.FileInputStream(file);
        
    }

    public static List<String> Keras2c(String sourcefile, String functionname)
    {
        Runtime runtime = Runtime.getRuntime();
        BufferedReader br = null;
        List <String> lines = new ArrayList<>();
        try {
            File file = new File(".././keras2c");
            File tmpFile = new File(".././keras2clog.log");
            if (!file.exists()){
                file.mkdirs();
            }
            if(!tmpFile.exists()) {
                tmpFile.createNewFile();
            }
            String[] commandlinekeras2c = new String[] {"python3" , "-m" , "keras2c",sourcefile,functionname};
            System.out.println(Arrays.toString(commandlinekeras2c));
            ProcessBuilder pb = new ProcessBuilder().command(commandlinekeras2c).inheritIO();
            pb.redirectErrorStream(true);
            pb.redirectOutput(tmpFile);
            pb.directory(new File(".././keras2c"));
            pb.start().waitFor();
            InputStream in = new FileInputStream(tmpFile);
            br= new BufferedReader(new InputStreamReader(in));
            String line = null;
            while((line = br.readLine()) != null) {
                System.out.println(line);
                lines.add(line);
            }
            br.close();
            br = null;
            System.out.println("executed finished");
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
        return lines;
    }
    public static int isInterval;
    public static List<Path> listFiles(Path path) throws IOException {
        List<Path> result;
        try (Stream<Path> walk = Files.walk(path)) {
            result = walk.filter(Files::isRegularFile)
                    .collect(Collectors.toList());
        }
        return result;
    }
    public void generate_Iris_Intervals(Path pathdir) throws IOException, InterruptedException
    {
        List<Path> paths = listFiles(pathdir);
        ExecutorService executor = Executors.newFixedThreadPool(5);
        paths.forEach(x -> {
            String extension = "";
            String name = x.toString();
            extension = name.substring(name.lastIndexOf("."));
            if(extension.equals(".c") && name.contains("annIris") && !name.contains("interval"))
            {
                System.out.println(name);
                List<String> execcmd =  new ArrayList<>();
                execcmd.add("frama-c");
                execcmd.add("-eva");
                execcmd.add("-eva-plevel 255");
                execcmd.add(name);
                cmdexecutor cmdexec = new cmdexecutor(execcmd, x.getFileName().toString());
                executor.execute(cmdexec);
            }
        });
    }
    public static void generate_ACAS_Intervals(Path pathdir) throws IOException
    {
        List<Path> paths = listFiles(pathdir);
        ExecutorService executor = Executors.newFixedThreadPool(5);
        paths.forEach(x -> {
            String name = x.toString();
            if(name.contains(".c") && name.contains("ACASXU") && !name.contains("interval"))
            {
                System.out.println(name);
                System.out.println("running command: frama-c -eva " + name);
                List<String> execcmd =  new ArrayList<>();
                execcmd.add("frama-c");
                execcmd.add("-eva");
                execcmd.add(name);
                cmdexecutor cmdexec = new cmdexecutor(execcmd, pathdir.toString());
                executor.execute(cmdexec);
            }
        });
    }
    public String breakframacline(String framacline, String char1, String char2)
    {
        return(framacline.substring(framacline.indexOf(char1)+1, framacline.lastIndexOf(char2)));
    }
    public List<String> castFramacToEsbmc(String framacline)
    {

        String value = "";
        String neuronmin = "";
        String neuronmax = "";
        List<String> outputlist = new ArrayList<>();
        List<String> neuronlist = new ArrayList<>();
        String[] parts = framacline.split("\u2208");
        if(parts[0].contains(".."))
        {
            String neuron = framacline.substring(0, framacline.lastIndexOf("["));
            String startindex = framacline.substring(framacline.indexOf("[")+1,framacline.lastIndexOf("..")+1);
            String endindex = framacline.substring(framacline.indexOf("..")+2,framacline.lastIndexOf("]")+1);
            for(int i=Integer.valueOf(startindex); i<Integer.valueOf(endindex); i++)
            {
                neuronlist.add(neuron + "[" + i + "]");
            }
        }
        else
        {
            neuronlist.add(framacline.substring(0, framacline.lastIndexOf(" \u2208")));
        }
        String[] parts2 = parts[0].split("\u2208");
        if(parts2[1].contains("["))
        {
            neuronmin = parts2[1].substring(parts2[1].indexOf("\u2208 [") + 3, parts2[1].lastIndexOf(" .. "));
            //neuronmax = framacline[framacline.find(" .. ") + 4 : -2]
        }
        else
        {
            isInterval = 0;
            value = parts2[1].substring(parts2[1].indexOf("\u2208 {") + 4, parts2[1].lastIndexOf(" .. "));
        }
        neuronlist.forEach(neuron->{
            if(isInterval == 0)
            {
                //outputlist.add("__ESBMC_assume(("+ neuron + " >= " + neuronmin + ") && ("+neuron + " <= " + neuronmax +"));");
            }
        });
        System.out.println(outputlist);
        return outputlist;

    }

    public List<String> summarize_Interval(Path filepath) throws IOException
    {
        int foundlayer = 0;
        String lastlayer = "";
        int counter = 0;
        List<String> esbmcNeuronIntervalList = new ArrayList<>();
        List<String> allLines = Files.readAllLines(filepath);
        for (String line : allLines) {
            String framacline = "";
            if(line.contains("layer"))
            {
                foundlayer = 1;
            }
            if(line.contains("layer") && foundlayer ==1)
            {
                lastlayer = line.substring(0,line.lastIndexOf("[")+1);
                framacline = line;
                esbmcNeuronIntervalList.addAll(castFramacToEsbmc(framacline.substring(framacline.lastIndexOf("layer"))));
                counter++;

            }
            else if(line.contains("[") && line.contains("]") && foundlayer == 1 && !line.contains("ANALYSIS SUMMARY"))
            {
                framacline = lastlayer + line.substring(line.indexOf("["));
                esbmcNeuronIntervalList.addAll(castFramacToEsbmc(framacline.substring(framacline.lastIndexOf("layer"))));
                counter++;
            }
        }
        System.out.println("I counted "+ esbmcNeuronIntervalList.size() + " intervals on file: " + filepath);
        return esbmcNeuronIntervalList;
    }

    public List<String> getEsbmcIntervalFromNeuron(String neuron, List<String> esbmcassumes) {
        List<String> esbmcassume = new ArrayList<>();
        esbmcassumes.forEach(x->{
            if(x.contains(neuron))
            {
                esbmcassume.add(x);
            }
        });
        return esbmcassume;
    }

    public void importInterval(Path esbmcfile, List<String> esbmcassumes) throws IOException
    {
        List<String> esbmclines = new ArrayList<>();
        esbmclines = Files.readAllLines(esbmcfile);
        FileWriter writer = new FileWriter(esbmcfile.toString());
        BufferedWriter bw = new BufferedWriter(writer);
        esbmclines.forEach(x->{
            if(x.contains("tanhFunctionLUT"))
            {
                try {
                    bw.write(x.toString());
                    //bw.write(getEsbmcIntervalFromNeuron(esbmcline[0:esbmcline.find(" =")], esbmcassumes)+ "\n")
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else
            {
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


    public void importIntervalAcasXU(Path esbmcfile, List<String> esbmcassumes) throws IOException
    {
        List<String> esbmclines = new ArrayList<>();
        esbmclines = Files.readAllLines(esbmcfile);
        FileWriter writer = new FileWriter(esbmcfile.toString());
        BufferedWriter bw = new BufferedWriter(writer);
        esbmclines.forEach(x->{
            if(x.contains("if") && x.contains("< 0") && x.contains("ANALYSIS SUMMARY"))
            {
                if(!x.contains("layer7"))
                {
                    try {
                        bw.write(x.toString());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                else if(x.contains("layer7") && x.contains("*c2)+c1;"))
                {
                    try {
                        bw.write(x.toString());
                        //esbmcbuff.write(getEsbmcIntervalFromNeuron(esbmcline[esbmcline.find("(")+1:esbmcline.find(" <")], esbmcassumes)+ "\n")
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                else
                {
                    try {
                        bw.write(x.toString());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

        });
        bw.close();
        writer.close();
    }
    public static void managercastEsbmcToFramac(Path filepath) throws IOException
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
                    castEsbmcToFramac(x.toString());
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        });
    }

    public static void castEsbmcToFramac(String filepath) throws IOException
    {
        String pathtowrite = filepath + Instant.now().hashCode()+".c";
        List<String> minlist = new ArrayList<>();
        List<String> maxlist = new ArrayList<>();
        List<String> indexlist = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filepath));
        FileWriter writer = new FileWriter(pathtowrite);
        BufferedWriter bw = new BufferedWriter(writer);
        System.out.println(pathtowrite);
        String x = null;
        x=br.readLine();
        System.out.println(x);
        while ((x = br.readLine()) != null) {
            System.out.println(x);
            if(x.contains("__fc_builtin"))
            {
                System.out.println("contains __fc_builtin");
                bw.write(x.replaceFirst("//", ""));
                bw.newLine();
            }
            else if(x.contains("__ESBMC_assume"))
            {
                System.out.println("contains esbmc assume");
                bw.write(x.replaceFirst("__ESBMC", "//__ESBMC"));
                bw.newLine();
                //minlist.add(x.substring(x.indexOf(">=")+2, x.indexOf("&&")));
                //maxlist.add(x.substring(x.indexOf("<=")+2, x.lastIndexOf(");")));
                //indexlist.add(x.substring(x.indexOf("(")+2, x.lastIndexOf(">=")));
            }
            else if(x.contains("nondet_float") || x.contains("nondet_int"))
            {
                bw.write("//"+ x);
                bw.newLine();
            }
            else if(x.contains("float c1"))
            {
                for (int i=0; i<minlist.size(); i++)
                {
                    bw.write("float"+ indexlist.get(i)+ " = Frama_C_float_interval(" + minlist.get(i) + "," + maxlist.get(i) + ");\n");
                    bw.newLine();
                }
                bw.write("\n" + x);
                bw.newLine();
            }
            else if(x.contains("utils.h"))
            {
                bw.write("//" + x);
                bw.newLine();
            }
            else if(x.contains("__ESBMC_assert"))
            {
                bw.write("//" + x);
                bw.newLine();
                bw.write("//@assert(" + x.substring(x.indexOf("(")+1, x.indexOf(",")) + ");");
                bw.newLine();
            }
            else
            {
                bw.write(x);
                bw.newLine();
            }
        }
        br.close();
        bw.close();
    }
    public static void main(String[] args) throws IOException {

        //firstly

    }
}