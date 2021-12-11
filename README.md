# Project Title

QNNVerifier

## Description

A Tool for Verifying Neural Networks using SMT-Based Model Checking. Using Frama-C and ESBMC as the backends. 

You can find detailed descriptions of QNNVerifyer in paper QNNVerifier: A Tool for Verifying Neural Networks using SMT-Based Model Checking

https://arxiv.org/pdf/2111.13110.pdf

## Getting Started

### Dependencies

Ubuntu or The Windows Subsystem for Linux(WSL), We highly recommend users to compile and run the code under Ubuntu 20.04 in case any potential dependency or compilation errors.
tensorflow 2.7.0 keras 2.7.0(for keras2C). 

If you want to use the converting function related to nnet, please downgrade the tensorflow and keras to 1.x.

Frama-c 23.0

ESBMC 6.8.0

openjdk 11 or jre 1.8.0

python 3.8


### Installing

Firstly install Frama-C and ESBMC and add them to PATH:

Frama-C:

https://frama-c.com/html/get-frama-c.html

ESBMC:

https://github.com/esbmc/esbmc

Then just clone this repo to anywhere you want.


### Executing program

Use ./src/nnetconverter or ./src/nnetconverterwindowed, following the guidance to convert various models to C files. 

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

Finally, use ./src/run_benchmark to run the benchmark. You can choose ./ACASXU_Reluplex, ./ACASXU_Reluplex_fxp, ./benchmarks-fxp-latest-2/Iris, benchmarks-fxp-latest-2/vocalic.

## Authors

Xidan Song (University of Manchester, United Kingdom) <xidan.song@manchester.ac.uk>

Edoardo Manino (University of Manchester, United Kingdom) <edoardo.manino@manchester.ac.uk>

Luiz Sena (Federal University of Amazonas, Brazil) <coelho.luiz.sena@gmail.com>

Erickson Alves (Federal University of Amazonas, Brazil) <erickson.higor@gmail.com>

Eddie de Lima Filho (Federal University of Amazonas, Brazil) <eddie_batista@yahoo.com.br>

Iury Bessa (Federal University of Amazonas, Brazil) <iury.bessa@gmail.com>

Mikel Luján (University of Manchester, United Kingdom) <Mikel.Lujan@manchester.ac.uk>

Lucas Cordeiro (University of Manchester, United Kingdom) <lucas.cordeiro@manchester.ac.uk>

## Version History

* 0.2
    * Update README
* 0.1
    * Initial Release

