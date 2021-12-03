ALL of the experiments go with the ACAS_XU benchmark.

Requirements:
It is highly recommended to run this tool in Linux. I run this tool on ubuntu 20.04.
tensorflow 2.7.0 keras 2.7.0(for keras2C). If you want to use the converting function related to nnet, please downgrade the tensorflow and keras to 1.x.
frama-c 23.0
esbmc 6.8.0
openjdk 11 or jre 1.8.0
python 3.8

Please do add esbmc, python3, java and frama-c to PATH.

Usage:

Firstly use ./src/nnetconverter or ./src/nnetconverterwindowed, following the guidance to convert various models to C files. 
If you end with format nnet, last step use NNet2C:
(batch) run generate.o in ./NNet2C/model/generate.o
(single file)run ./NNet2C/generate.o <targetednnetfile> <target generate folder>

Secondly insert safety properties expressible in plain C code. Here, we give examples of robustness properties. It is not suggested to come up with your own properties, 
just use the properties we already provide to you.

Thirdly, use the ./Intervalgenerator.py to generate fixed-point C files for esbmc to verify:
1. use -m to generate the suitable input for frama-c.
2. use -f1 or -f2 to run frama-c and analyse the Intervals, the logs are saved in .intervals
3. use -e1 -e2 to import these intervals to C file.
4. or use -g to generate everything automaticlly
<Optional>5. use ./fxpgenerator_acas.py to generate <filename>fxp.c. If floating-point file is in ACASXU_Reluplex, the fixed-point file could be in ACASXU_Reluplex_fxp.
6. the C file is ready to use.

It is likely to fail when running frama-c to generate the Intervals. If you encounter some strange problem, please use the command for example:
"frama-c -eva ./ACASXU_run2a_4_7_batch_2000annP1.c > ./ACASXU_Reluplex/Interval/p1/ACASXU_run2a_4_7_batch_2000annP1.interval"
to run the Interval analysis manually. Notice that .c framac.c and .interval should be in the same folder.

Finally, use ./src/run_benchmark to run the benchmark. You can choose ./ACASXU_Reluplex, ./ACASXU_Reluplex_fxp, ./benchmarks-fxp-latest-2/Iris, benchmarks-fxp-latest-2/vocalic
