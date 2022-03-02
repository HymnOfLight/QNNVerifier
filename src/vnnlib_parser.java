import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class vnnlib_parser {
    public static List<Path> listFiles(Path path) throws IOException {
        List<Path> result;
        try (Stream<Path> walk = Files.walk(path)) {
            result = walk.filter(Files::isRegularFile)
                    .collect(Collectors.toList());
        }
        return result;
    }
    public void pre_process_files(String filepath)
    {

    }
    public List<String> readstatements_acas(String filepath,int mode) {
        List<String> assumelist = new ArrayList<String>();
        BufferedReader vnnlibReader;
        float[] lowerbound = new float[5];
        float[] upperbound = new float[5];
        try {
            vnnlibReader = new BufferedReader(new FileReader(
                    filepath));
            String line = new String();
            do {
                line = vnnlibReader.readLine();
                // read next line
                if (line == null) {
                    System.out.println(assumelist);
                    return assumelist;
                } else if (line.startsWith("; Unscaled")) {
                    int i = Integer.valueOf(line.substring(line.indexOf("t") + 2, line.indexOf(":"))).intValue();
                    lowerbound[i] = Float.valueOf(line.substring(line.indexOf("(") + 1, line.indexOf(",") - 1)).floatValue();
                    upperbound[i] = Float.valueOf(line.substring(line.indexOf(",") + 2, line.indexOf(")") - 1)).floatValue();
                    if (mode == 0) {
                        assumelist.add("__ESBMC_assume(tensor_input[1][1][1][" + String.valueOf(i) + "]>=" + String.valueOf(lowerbound[i]) + " && tensor_input[1][1][1][" + String.valueOf(i) + "]<=" + String.valueOf(upperbound[i]) + ");");
                    } else if (mode == 1)//mode 1: insert frama-c annotations
                    {
                        String annotations = "float tensor_input[1][" + String.valueOf(i) + "] = Frama_C_float_interval(" + String.valueOf(lowerbound[i]) + " , " + String.valueOf(upperbound[i]) + ");";
                        System.out.println(annotations);
                        assumelist.add(annotations);
                    } else {

                    }
                }} while (line != null);
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println(assumelist);
        return assumelist;
    }
    public List<String> readstatements_cifar(String filepath, int mode) {
        List<String> assumelist = new ArrayList<String>();
        BufferedReader vnnlibReader;
        try {
            vnnlibReader = new BufferedReader(new FileReader(
                    filepath));
            String line = vnnlibReader.readLine();
            float lowerbound;
            float upperbound;
            do {
                // read next line
                line = vnnlibReader.readLine();
                if (line == null) {
                    return assumelist;
                } else if (line.startsWith("(assert (<= X")) {
                    int i = Integer.valueOf(line.substring(line.indexOf("_") + 1, line.lastIndexOf(" ")));
                    //System.out.println(i);
                    upperbound = Float.valueOf(line.substring(line.lastIndexOf(" "), line.lastIndexOf(")") - 2));
                    line = vnnlibReader.readLine();
                    lowerbound = Float.valueOf(line.substring(line.lastIndexOf(" "), line.lastIndexOf(")") - 2));
                    line = vnnlibReader.readLine();
                    int j = i % 3;
                    int m = i / 3;
                    int k = m % 32;
                    int n = m / 32;
                    if (mode == 0)//mode 0: insert esbmc assumes
                    {
                        String esbmcassumes = "__ESBMC_assume(tensor_x_0[1][" + String.valueOf(n) + "][" + String.valueOf(k) + "][" + String.valueOf(j) + "]>=" + String.valueOf(lowerbound) + " && tensor_x_0[1][" + String.valueOf(n) + "][" + String.valueOf(k) + "][" + String.valueOf(j) + "]<=" + String.valueOf(upperbound) + ");";
                        System.out.println(esbmcassumes);
                        assumelist.add(esbmcassumes);
                        //assumelist.add("__ESBMC_assume(tensor_x_0[1][" + String.valueOf(k) + "][" + String.valueOf(n) + "][" + String.valueOf(j) + "]>=" + String.valueOf(lowerbound) + " && tensor_x_0[1][" + String.valueOf(k) + "][" + String.valueOf(n) + "][" + String.valueOf(j) + "]<=" + String.valueOf(upperbound) + ");");
                    } else if (mode == 1)//mode 1: insert frama-c annotations
                    {
                        String annotations = "float tensor_x_0[1][" + String.valueOf(n) + "][" + String.valueOf(k) + "][" + String.valueOf(j) + "] = Frama_C_float_interval(" + String.valueOf(lowerbound) + " , " + String.valueOf(upperbound) + ");";
                        System.out.println(annotations);
                        assumelist.add(annotations);
                    } else {

                    }

                }
            }while (line != null) ;
            System.out.println("read properties finished");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return assumelist;
    }

    public List<String> readstatements_mnist(String filepath, int mode)
    {
        List<String> assumelist = new ArrayList<String>();
        BufferedReader vnnlibReader;
        try {
            vnnlibReader = new BufferedReader(new FileReader(
                    filepath));
            String line = vnnlibReader.readLine();
            do {
                // read next line
                line = vnnlibReader.readLine();
                float lowerbound, upperbound;
                if (line == null) {
                    return assumelist;
                }
                else if (line.startsWith("(assert (<= X")) {
                    upperbound = Float.valueOf(line.substring(line.lastIndexOf(" ") , line.lastIndexOf(")") - 2)).floatValue();
                    line = vnnlibReader.readLine();
                    lowerbound = Float.valueOf(line.substring(line.lastIndexOf(" ") , line.lastIndexOf(")") - 2)).floatValue();
                    int i = Integer.valueOf(line.substring(line.indexOf("_")+1, line.lastIndexOf(" "))).intValue();
                    if (mode == 0) {//mode 0 insert esbmc assumes
                        assumelist.add("__ESBMC_assume(tensor_0[1][" + String.valueOf(i) + "][1]>=" + String.valueOf(lowerbound) + " && tensor_0[1][" + String.valueOf(i) + "][1]<=" + String.valueOf(upperbound) + ");");
                    }else if (mode == 1)//mode 1: insert frama-c annotations
                    {
                        String annotations = "float tensor_input[1][" + String.valueOf(i) + "] = Frama_C_float_interval(" + String.valueOf(lowerbound) + " , " + String.valueOf(upperbound) + ");";
                        System.out.println(annotations);
                        assumelist.add(annotations);
                    } else {

                    }
                    line = vnnlibReader.readLine();
                }
            }while (line != null);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return assumelist;
    }

    public void insert_properties(String filepath, List<String> assumelist)
    {
        BufferedReader CfileReader;
        try {
            CfileReader = new BufferedReader(new FileReader(
                    filepath));
            String line = CfileReader.readLine();
            FileWriter fw = new FileWriter(filepath,true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write("\n");
            bw.write("int main(){\n");
            /*for(int k=0; k<assumelist.size() ;k++)
            {
                bw.write("float x" + String.valueOf(k)+" = nondet_float();\n");
                System.out.println("float x" + String.valueOf(k)+" = nondet_float();");
            }*/
            for(int j=0; j<assumelist.size(); j++) {
                System.out.println(assumelist.get(j));
                bw.write(assumelist.get(j) + "\n");
            }
            bw.write("}");
            bw.close();
    } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void insert_properties_byvnnlib_single(String Cfile, String vnnlibfile) throws IOException {
        String extension = "";
        String name = Cfile.toString();
        extension = name.substring(name.lastIndexOf("."));
        System.out.println("Inserting properties to single file");
        if (extension.equals(".c")) {
            List<String> assumelist;
            if (name.contains("ACASXU")) {
                assumelist = readstatements_acas(vnnlibfile,0);
            } else if(name.contains("mnist")){
                assumelist = readstatements_mnist(vnnlibfile,0);
            }
            else if(name.contains("cifar"))
            {
                assumelist = readstatements_cifar(vnnlibfile,0);
            }
            else
            {
                assumelist = readstatements_cifar(vnnlibfile,0);
            }
            insert_properties(Cfile.toString(), assumelist);
        }
    }
    public void insert_properties_byvnnlib(String CfilePath, String vnnlibfile) throws IOException
    {
        Path path = Paths.get(CfilePath);
        List<Path> paths = listFiles(path);
        for (Path x : paths) {
            String extension = "";
            String name = x.toString();
            extension = name.substring(name.lastIndexOf("."));
            if (extension.equals(".c")) {
                List<String> assumelist;
                if (name.contains("ACASXU")) {
                    assumelist = readstatements_acas(vnnlibfile,0);
                } else if(name.contains("mnist")){
                    assumelist = readstatements_mnist(vnnlibfile,0);
                }else
                {
                    assumelist = readstatements_cifar(vnnlibfile,0);
                }
                insert_properties(x.toString(), assumelist);
            }
        }
    }
    public void insert_all_properties(String CfilePath, String vnnlibfilepath) throws IOException
    {
        Path path = Paths.get(vnnlibfilepath);
        List<Path> paths = listFiles(path);
        for (Path x : paths) {
            String extension = "";
            String name = x.toString();
            extension = name.substring(name.lastIndexOf("."));
            if (extension.equals(".vnnlib")) {
                insert_properties_byvnnlib(CfilePath, x.toString());
            }
        }
    }
    public void insert_framac_annotations(String CfilePath, String vnnlibfile) throws IOException
    {
        Path path = Paths.get(CfilePath);
        List<Path> paths = listFiles(path);
        for (Path x : paths) {
            String extension = "";
            String name = x.toString();
            extension = name.substring(name.lastIndexOf("."));
            if (extension.equals(".c")) {
                List<String> assumelist;
                if (name.contains("ACASXU")) {
                    assumelist = readstatements_acas(vnnlibfile,1);//mode 1 insert frama-c annotations
                } else if(name.contains("mnist")){
                    assumelist = readstatements_mnist(vnnlibfile,1);
                }else
                {
                    assumelist = readstatements_cifar(vnnlibfile,1);
                }
                insert_properties(x.toString(), assumelist);
            }
        }
    }
    public static void main(String[] arguments) throws IOException {
        vnnlib_parser parser = new vnnlib_parser();
        if(arguments[0].contains("-all"))
        {
            parser.insert_all_properties(arguments[1], arguments[2]);
        }
        else if (arguments[0].contains("-single"))
        {
            System.out.println("Inserting properties to single file");
            parser.insert_properties_byvnnlib_single(arguments[1], arguments[2]);
        }
        else if (arguments[0].contains("-framac"))
        {
            System.out.println("Generating framac annotations from vnnlib");
            parser.insert_framac_annotations(arguments[1], arguments[2]);
        }
        else{

        }
    }
}
