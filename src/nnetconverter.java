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

public class nnetconverter {

	public static String readsourcefile()
	{
		Scanner sc = new Scanner(System.in);
		System.out.println("please input file path, end with new line\n");
		String filename = sc.nextLine();
		return filename;	
	}
	public static String getfunctionname()
	{
		Scanner sc = new Scanner(System.in);
		System.out.println("please input function name, end with new line\n");
		String functionname = sc.nextLine();
		return functionname;
	}
	public static List<String> executepython(String tmpfilepath, String tmpfilename, String envfilepath, String pyfilename)
	{
		Runtime runtime = Runtime.getRuntime(); 
		BufferedReader br = null;
		List <String> lines = new ArrayList<>();
        try {
        	File file = new File(tmpfilepath);
        	File tmpFile = new File(tmpfilename);
        	if (!file.exists()){
        	file.mkdirs();
        		}
        		if(!tmpFile.exists()) {
        		tmpFile.createNewFile();
        		}
        	String[] commandlinekeras2c = new String[] {"python3" , pyfilename};
        	System.out.println(Arrays.toString(commandlinekeras2c));
        	ProcessBuilder pb = new ProcessBuilder().command(commandlinekeras2c).inheritIO();
        	pb.redirectErrorStream(true);
        	pb.redirectOutput(tmpFile);
        	pb.directory(new File(envfilepath));
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
	
	public static List<String> Onnx2keras(String sourcefile, String functionname) throws IOException
	{
		boolean append = true;
		String filename =".././onnx2keras/converter.py";
		Files.deleteIfExists(Paths.get(filename));
		BufferedWriter writer = new BufferedWriter(new FileWriter(filename, append));
		writer.write("import onnx");
		writer.newLine();
		writer.write("from onnx2keras import onnx_to_keras");
		writer.newLine();
		writer.write("onnx_model = onnx.load('"+sourcefile+"')");
		writer.newLine();
		writer.write("k_model = onnx_to_keras(onnx_model, ['0'])");
		writer.newLine();
		writer.write("k_model.save(\"" + functionname + "\"\u0029");
		writer.close();
		return(executepython("../",".././onnx2keras.log",".././onnx2keras",".././onnx2keras/converter.py"));
		
	}
	
	@SuppressWarnings("resource")
	public static List<String> h52pb(String sourcefile, String functionname) throws IOException
	{
		boolean append = true;
		String filename =".././h5topb.py";
		Files.deleteIfExists(Paths.get(filename));
		BufferedWriter writer = new BufferedWriter(new FileWriter(filename, append));
		writer.write("import onnx");
		writer.newLine();
		writer.write("from keras.models import load_model");
		writer.newLine();
		writer.write("import tensorflow as tf");
		writer.newLine();
		writer.write("import os"); 
		writer.newLine();
		writer.write("import os.path as osp");
		writer.newLine();
		writer.write("from keras import backend as K");
		writer.newLine();
		writer.write("input_path = './'");
		writer.newLine();
		writer.write("weight_file = '" + functionname +".h5'");
		writer.newLine();
		writer.write("weight_file_path = osp.join(input_path,weight_file)");
		writer.newLine();
		writer.write("output_graph_name = weight_file[:-3] + '.pb'");
		writer.newLine();
		writer.write("def h5_to_pb(h5_model,output_dir,model_name,out_prefix = \"Y\",log_tensorboard = True):");
		writer.newLine();
		writer.write("    if osp.exists(output_dir) == False:");
		writer.newLine();
		writer.write("        os.mkdir(output_dir)");
		writer.newLine();
		writer.write("    out_nodes = []");
		writer.newLine();
		writer.write("    for i in range(len(h5_model.outputs)):");
		writer.newLine();
		writer.write("        out_nodes.append(out_prefix)");
		writer.newLine();
		writer.write("        tf.identity(h5_model.output[i],out_prefix)");
		writer.newLine();
		writer.write("    sess = K.get_session()");
		writer.newLine();
		writer.write("    from tensorflow.python.framework import graph_util,graph_io");
		writer.newLine();
		writer.write("    init_graph = sess.graph.as_graph_def()");
		writer.newLine();
		writer.write("    main_graph = graph_util.convert_variables_to_constants(sess,init_graph,out_nodes)");
		writer.newLine();
		writer.write("    graph_io.write_graph(main_graph,output_dir,name = model_name,as_text = False)");
		writer.newLine();
		writer.write("    if log_tensorboard:");
		writer.newLine();
		writer.write("        from tensorflow.python.tools import import_pb_to_tensorboard");
		writer.newLine();
		writer.write("        import_pb_to_tensorboard.import_to_tensorboard(osp.join(output_dir,model_name),output_dir)");
		writer.newLine();
		writer.write("output_dir = osp.join(os.getcwd(),\"trans_model\")");
		writer.newLine();
		writer.write("h5_model = load_model(weight_file_path)");
		writer.newLine();
		writer.write("h5_to_pb(h5_model,output_dir = output_dir,model_name = output_graph_name)");
		writer.newLine();
		writer.write("print('model saved')");
		writer.close();	
		return(executepython("./","./h5topb.log","./",".././h5topb.py"));

	}
	
	@SuppressWarnings("resource")
	public static List<String> pb2nnet(String sourcefile, String functionname) throws IOException
	{
		boolean append = true;
		String filename =".././pb2nnet.py";
		Files.deleteIfExists(Paths.get(filename));
		BufferedWriter writer = new BufferedWriter(new FileWriter(filename, append));
		writer.write("from NNet.converters.pb2nnet import pb2nnet");
		writer.newLine();
		writer.write("inputMins  = [0.0,-3.141593,-3.141593,100.0,0.0]");
		writer.newLine();
		writer.write("inputMaxes = [60760.0,3.141593,3.141593,1200.0,1200.0]");
		writer.newLine();
		writer.write("means  = [1.9791091e+04,0.0,0.0,650.0,600.0,7.5188840201005975]");
		writer.newLine();
		writer.write("ranges = [60261.0,6.28318530718,6.28318530718,1100.0,1200.0,373.94992]");
		writer.newLine();
		writer.write("pbFile = './"+ functionname+".pb'");
		writer.newLine();
		writer.write("pb2nnet(pbFile, inputMins, inputMaxes, means, ranges)");
		writer.newLine();
		writer.close();
		return(executepython("./","./pb2nnet.log","./",".././pb2nnet.py"));
	}
	
	public static void main(String[] args) throws IOException {
		System.out.println("Please select mode:");
		System.out.println("1.Keras to C");
		System.out.println("2.Onnx to Keras");
		System.out.println("3.HDF5 to tensorflow pb");
		System.out.println("4.tensorflow pb to nnet");
		System.out.println("Please enter mode: ");
		Scanner in = new Scanner(System.in);
		String msg = in.nextLine();
		int modeint = Integer.valueOf(msg);
		String functionname = getfunctionname();
		String sourcefile = readsourcefile();
		switch (modeint) {
        case 1:  Keras2c(sourcefile, functionname);
                 break;
        case 2:  Onnx2keras(sourcefile, functionname);
                 break;
        case 3:  h52pb(sourcefile, functionname);
                 break;
        case 4:  pb2nnet(sourcefile, functionname);
                 break;
        
        default: System.out.println("invalid input number");
                 break;
    

	}

}
}
