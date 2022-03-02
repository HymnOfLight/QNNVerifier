from NNet.converters.nnet2onnx import nnet2onnx
nnetFile = "./ACASXU_run2a_1_1_batch_2000.nnet"

# ONNX file
onnxFile = './ACASXU_run2a_1_1_batch_2000.onnx'

# Convert the file
nnet2onnx(nnetFile, onnxFile)