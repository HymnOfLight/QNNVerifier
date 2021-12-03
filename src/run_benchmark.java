import java.io.BufferedReader;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import javax.swing.JTextArea;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class benchmarkrunner implements Runnable
{
	private String benchmarkfilepath;
	private String esbmcfilepath;
	private int benchmark_type;
	private int quantized_bits;
	private int benchmark_mode;
	public benchmarkrunner(String esbmcfilepath, String benchmarkfilepath, int benchmark_type, int quantized_bits, int benchmark_mode )
	{
		this.esbmcfilepath = esbmcfilepath;
		this.benchmarkfilepath = benchmarkfilepath;
		this.benchmark_type = benchmark_type;
		this.quantized_bits = quantized_bits;
		this.benchmark_mode = benchmark_mode;
	}
	public int getintfrominput()
	{
		Scanner in = new Scanner(System.in);
		String processes_msg = in.nextLine();
		return(Integer.valueOf(processes_msg));
	}

	public List<String> buildesbmccmd()
	{
		String quantized_bits_string = null;
		String mode_string = null;
		List<String> list = new ArrayList<>();
		//System.out.println(this.benchmark_mode);
		//System.out.println(this.benchmark_type);
		//System.out.println(this.quantized_bits);
		if(benchmark_type == 1)
		{
			if (benchmark_mode == 1)
			{
				if(quantized_bits == 32)
				{
					quantized_bits_string = "-I . -DINT_BITS=27 -DFRAC_BITS=4";
				}
				else
				{
			    	System.out.println("Other quantized bits is not supported by ACAS benchmarks, use default 32 bits");
			    	quantized_bits_string = "-I . -DINT_BITS=27 -DFRAC_BITS=4";
				}
				mode_string = "--fixedbv";
			}
			else if(benchmark_mode == 2)
			{
				mode_string = "--floatbv";
			}
			else
			{
				
			}
		}
		else if(benchmark_type == 2)
		{
			if (benchmark_mode == 1)
			{
				if(quantized_bits ==8)
				{
					quantized_bits_string = "-I . -DINT_BITS=6 -DFRAC_BITS=1";
				}
				else if(quantized_bits == 16)
				{
			    	quantized_bits_string = "-I . -DINT_BITS=6 -DFRAC_BITS=9";
				}
				else if(quantized_bits == 32)
				{
			    	quantized_bits_string = "-I . -DINT_BITS=6 -DFRAC_BITS=25";
				}
				else
				{
					System.out.println("Other quantized bits is not supported by vocalic benchmarks, use default 32 bits");
			    	quantized_bits_string = "-I . -DINT_BITS=27 -DFRAC_BITS=4";
				}
				mode_string = "--fixedbv";

			}
			else if(benchmark_mode == 2)
			{
				mode_string = "--floatbv";
			}	
			else {
				
			}

		}else if(benchmark_type == 3)
		{
			if (benchmark_mode == 1)
			{
				if(quantized_bits ==8)
				{
					quantized_bits_string = "-I . -DINT_BITS=5 -DFRAC_BITS=2";
				}
				else if(quantized_bits == 16)
				{
			    	quantized_bits_string = "-I . -DINT_BITS=5 -DFRAC_BITS=10";
				}
				else if(quantized_bits == 32)
				{
			    	quantized_bits_string = "-I . -DINT_BITS=5 -DFRAC_BITS=26";
				}
				else
				{
					System.out.println("Other quantized bits is not supported by Iris benchmarks, use default 32 bits");
			    	quantized_bits_string = "-I . -DINT_BITS=5 -DFRAC_BITS=26";
				}
				mode_string = "--fixedbv";	
			}
			else if(benchmark_mode == 2)
			{
				mode_string = "--floatbv";	
			}	
			else {				
			}		
		}
		else 
		{			
		}
		list.add(this.esbmcfilepath);
		list.add(this.benchmarkfilepath);
		list.add("--force-malloc-success");
		list.add("--no-bounds-check");
		list.add("--no-div-by-zero-check");
		list.add("--no-pointer-check");
		list.add("--interval-analysis"); 
		list.add("--boolector");
		//System.out.println(mode_string);
		//System.out.println(quantized_bits_string);
		if(mode_string!=null && mode_string.length()!=0) 
			list.add(mode_string);
		if(quantized_bits_string!=null && quantized_bits_string.length()!=0) 
			list.add(quantized_bits_string);
		return list;
	}
	public void run()
	{		
		List<String> esbmccommandline = buildesbmccmd();
		BufferedReader br = null;
		try {
        	System.out.println(esbmccommandline.toString());
        	ProcessBuilder pb = new ProcessBuilder().command(esbmccommandline).inheritIO();
        	Path outpath = Paths.get(benchmarkfilepath + ".log");
        	Path errpath = Paths.get(benchmarkfilepath + ".err");
        	pb.redirectError(errpath.toFile());
        	pb.redirectOutput(outpath.toFile());
        	Instant start = Instant.now();
        	pb.start().waitFor();
        	Instant finish = Instant.now();
        	long timeElapsed = Duration.between(start, finish).toMillis();
        	System.out.println(benchmarkfilepath+ " execute finished in "+ timeElapsed + " ms");
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
public class run_benchmark {
	private static int benchmark_int;
	public static int mode_int;
	public static int quantized_int;
	public static int processes_int;
	private static String esbmcpath;
	public static String buildesbmcpath()
	{
		//Scanner sc = new Scanner(System.in);
		//System.out.println("Please specify esbmc directory:");
		//String filename = sc.nextLine();
		//return ("C:\\projects\\Iris\\esbmc-6.8.0\\esbmc.exe");
		return ("esbmc");
		//return filename;
	}
	 public static List<Path> listFiles(Path path) throws IOException {
	        List<Path> result;
	        try (Stream<Path> walk = Files.walk(path)) {
	            result = walk.filter(Files::isRegularFile)
	                    .collect(Collectors.toList());
	        }
	        return result;
	}
	public static void run_MultiBenchMarks(String PathtoBenchmark, JTextArea textArea) throws IOException
	{
		ExecutorService executor = Executors.newFixedThreadPool(processes_int);
		Path path = Paths.get(PathtoBenchmark);
        List<Path> paths = listFiles(path);
        esbmcpath=buildesbmcpath();
        paths.forEach(x -> {
        	String extension = "";
        	String name = x.toString();
        	extension = name.substring(name.lastIndexOf("."));
        	if(extension.equals(".c"))
        	{
        	benchmarkrunner benchmarkrun = new benchmarkrunner(esbmcpath, x.toString(), benchmark_int, quantized_int, mode_int);
        	executor.execute(benchmarkrun);
        	textArea.append(x.toString() + " is running");
        	}
    	});
        
	}	
	public static void run_Single_Benchmark(String PathtoBenchmark, JTextArea textArea)
	{
		ExecutorService executor = Executors.newFixedThreadPool(processes_int);
		Path path = Paths.get(PathtoBenchmark);
		esbmcpath=buildesbmcpath();
		benchmarkrunner benchmarkrun = new benchmarkrunner(esbmcpath, path.toString(), benchmark_int, quantized_int, mode_int);
    	executor.execute(benchmarkrun);
    	textArea.append(path.toString() + " is running");
	}
	public static void main(String[] args) throws IOException {
		System.out.println("Please select benchmarks:");
		System.out.println("1.run all ACAS_Xu");
		System.out.println("2.run all Iris");
		System.out.println("3.run all vocalic");
		System.out.println("4.run single benchmark");
		Scanner in = new Scanner(System.in);
		String benchmark_msg = in.nextLine();
		benchmark_int = Integer.valueOf(benchmark_msg);
		System.out.println("Please enter benchmarks: ");
		System.out.println("Please select mode:");
		System.out.println("1.fxp:");
		System.out.println("2.floating-point");
		Scanner in2 = new Scanner(System.in);
		String mode_msg = in2.nextLine();
		mode_int = Integer.valueOf(mode_msg);
		JTextArea textArea = new JTextArea();
		if(mode_int == 1)
		{
			System.out.println("Please enter quantized_bits: ");
			Scanner in4 = new Scanner(System.in);
			String quantized_int_str = in4.nextLine();
			quantized_int = Integer.valueOf(quantized_int_str);
		}
	
		System.out.println("Please input how many processes in parallel:");

		Scanner in3 = new Scanner(System.in);
		String processes_msg = in3.nextLine();
		processes_int = Integer.valueOf(processes_msg);			
		if(benchmark_int == 1)//run ACAS_Xu
		{
			//run_MultiBenchMarks("C:\\projects\\Iris\\ACASXU_Reluplex_fxp");
			run_MultiBenchMarks(".././ACASXU_Reluplex_fxp", textArea);
		}
		else if(benchmark_int == 2)//run Iris
		{
			//run_MultiBenchMarks("C:\\projects\\Iris\\benchmarks-fxp-latest-2\\Iris");
			run_MultiBenchMarks(".././benchmarks-fxp-latest-2/Iris", textArea);
		}
		else if(benchmark_int == 3)//run vocalic
		{
			//run_MultiBenchMarks("C:\\projects\\Iris\\benchmarks-fxp-latest-2\\vocalic");
			run_MultiBenchMarks(".././benchmarks-fxp-latest-2/vocalic", textArea);
		}
		else if(benchmark_int == 4)//run single benchmark
		{
			System.out.println("Please specify the file path of the benchmark you want to run");
			Scanner in4 = new Scanner(System.in);
			String singlebenchmark_filepath = in4.nextLine();
			run_Single_Benchmark(singlebenchmark_filepath, textArea);
		}
	}
}


