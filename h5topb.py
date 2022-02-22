from keras.models import load_model
import tensorflow as tf
import os
import os.path as osp
from keras import backend as K
input_path = '/mnt/c/projects/RnnVerify/models'
weight_file = 'model_20classes_rnn2_fc32_fc32_fc32_fc32_fc32.h5'
weight_file_path = osp.join(input_path,weight_file)
output_graph_name = weight_file[:-3] + '.pb'
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
    main_graph = graph_util.convert_variables_to_constants(sess,init_graph,out_nodes)
    graph_io.write_graph(main_graph,output_dir,name = model_name,as_text = False)
    if log_tensorboard:
        from tensorflow.python.tools import import_pb_to_tensorboard
        import_pb_to_tensorboard.import_to_tensorboard(osp.join(output_dir,model_name),output_dir)
output_dir = osp.join(os.getcwd(),"trans_model")
h5_model = load_model(weight_file_path)
h5_to_pb(h5_model,output_dir = output_dir,model_name = output_graph_name)
print('model saved')