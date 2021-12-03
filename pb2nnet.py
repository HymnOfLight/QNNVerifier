from NNet.converters.pb2nnet import pb2nnet
inputMins  = [0.0,-3.141593,-3.141593,100.0,0.0]
inputMaxes = [60760.0,3.141593,3.141593,1200.0,1200.0]
means  = [1.9791091e+04,0.0,0.0,650.0,600.0,7.5188840201005975]
ranges = [60261.0,6.28318530718,6.28318530718,1100.0,1200.0,373.94992]
pbFile = './mnist.pb'
pb2nnet(pbFile, inputMins, inputMaxes, means, ranges)
