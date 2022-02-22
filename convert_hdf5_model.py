import keras
import tensorflow as tf
import os
import numpy as np
import sys
import os.path as osp
from keras import backend as K


def h5_to_pb(h5_model,output_dir,model_name,out_prefix = "X",log_tensorboard = True):
    if osp.exists(output_dir) == False:
        os.mkdir(output_dir)
    out_nodes = []
    for i in range(len(h5_model.outputs)):
        out_nodes.append(out_prefix)
        tf.identity(h5_model.output[i],out_prefix)
    sess = K.get_session()
    from tensorflow.python.framework import graph_util,graph_io
    init_graph = sess.graph.as_graph_def()
    main_graph = tf.compat.v1.graph_util.extract_sub_graph(sess,init_graph,out_nodes)
    graph_io.write_graph(main_graph,output_dir,name = model_name,as_text = False)
    if log_tensorboard:
        from tensorflow.python.tools import import_pb_to_tensorboard
        import_pb_to_tensorboard.import_to_tensorboard(osp.join(output_dir,model_name),output_dir)

def read_model_from_h5(filepath):
    read_model = keras.models.load_model(filepath)
    print("Loaded model:", read_model)
    read_model.summary()
    print(read_model.layers)
    return read_model

def read_model_from_pb(filepath):
    tf.reset_default_graph()
    with tf.gfile.GFile(filepath, 'rb') as f:
        graph_def_optimized = tf.GraphDef()
        graph_def_optimized.ParseFromString(f.read())

def delete_lambda_from_acas_graph():
    with open(input_model_filepath, 'rb') as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())

    # Delete nodes
    nodes = []
    for node in graph_def.node:
        if 'lambda' in node.name:
            print('Drop', node.name)
        else:
            nodes.append(node)

    mod_graph_def = tf.GraphDef()
    mod_graph_def.node.extend(nodes)

    # Delete references to deleted nodes
    for node in mod_graph_def.node:
        inp_names = []
        for inp in node.input:
            if 'lambda' in inp:
                pass
            else:
                inp_names.append(inp)

        del node.input[:]
        node.input.extend(inp_names)

    with open(output_model_filepath, 'wb') as f:
        f.write(mod_graph_def.SerializeToString())
def main(argv):
    path = sys.argv[1]
    model_name = sys.argv[2]
    model = read_model_from_h5(path)
    h5_to_pb(model,'/mnt/c/projects/submission/saved_pb_model', model_name)

if __name__ == "__main__":
    main(sys.argv[1:])

