import onnxruntime as ort
import onnx
import numpy as np
import sys
import os

ort_session = ort.InferenceSession('/mnt/c/projects/vnncomp2021/benchmarks/acasxu/ACASXU_run2a_1_1_batch_2000.onnx')
org_outputs = [x.name for x in ort_session.get_outputs()]

model = onnx.load('/mnt/c/projects/vnncomp2021/benchmarks/acasxu/ACASXU_run2a_1_1_batch_2000.onnx')
for node in model.graph.node:
    for output in node.output:
        if output not in org_outputs:
            model.graph.output.extend([onnx.ValueInfoProto(name=output)])

# excute onnx
ort_session = ort.InferenceSession(model.SerializeToString())
outputs = [x.name for x in ort_session.get_outputs()]
in_img = np.array([-0.303531156,-0.009549297,0.493380324,0.3,0.3]).reshape(1,1,1,5).astype(np.float32)
#in_img = np.array([-0.30353,-0.00954,0.49338,0.3,0.3]).reshape(1,1,1,5).astype(np.float32)

ort_outs = ort_session.run(outputs, {'input': in_img} )
print(ort_outs)
ort_outs = OrderedDict(zip(outputs, ort_outs))