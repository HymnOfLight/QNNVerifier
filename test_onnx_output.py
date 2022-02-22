import onnxruntime as rt
import numpy as np
import sys
import os

def read_onnx_files(filepath):
    sess = rt.InferenceSession(filepath)
    print("The model expects input shape: ", sess.get_inputs()[0].shape)
    return sess

def make_inferences_from_folder(filepath, count):
    g = os.walk(filepath)
    for path,dir_list,file_list in g:  
        for file_name in file_list:         
            with open(os.path.join(path, file_name), encoding="utf8", errors='ignore') as f:
                print(os.path.join(path, file_name))
                if '.onnx' in file_name: 
                    if 'acas' in file_name:
                        input = generate_acas_input(count)
                        make_inferences(os.path.join(path, file_name), input, count, file_name)
                    elif 'cifar' in file_name:
                        input = generate_cifar_input(count)
                        make_inferences(os.path.join(path, file_name), input, count, file_name)
                    elif 'mnist' in file_name:
                        input = generate_mnist_input(count)
                        make_inferences(os.path.join(path, file_name), input, count, file_name)

def make_inferences(sess, input, count, outputfile):
    input_name = sess.get_inputs()[0].name
    label_name = sess.get_outputs()[0].name
    i = 0
    output = list()
    while i<int(count):
        result = sess.run(None, {input_name: input[i]})
        #print(result)
        output.append(result)
        i=i+1
    print(output)
    np.save(outputfile + count + '.npy', np.array(output))

def generate_random_input(outputpath, count, type):
    if 'acas' in type:
        acasxu_input = np.random.rand(int(count),1,1,1,5).astype(np.float32)
        print('The generated input is: ', acasxu_input)
        np.save(outputpath + 'random_acas_input_' + count + '.npy', acasxu_input)
    elif 'cifar' in type:
        cifar_input = np.random.rand(int(count),1,3,32,32).astype(np.float32)
        print('The generated input is: ', cifar_input)
        np.save(outputpath + 'random_cifar_input_' + count + '.npy', cifar_input)
    elif 'mnist' in type:
        mnist_input = np.random.rand(int(count),1,784,1).astype(np.float32)
        print('The generated input is: ', mnist_input)
        np.save(outputpath + 'random_mnist_input_' + count + '.npy', mnist_input)

def generate_acas_input(count):
    if not os.path.exists('./random_acas_input_' + count + '.npy'):
        generate_random_input('./', count, 'acas')
    else:
        print('Use saved input')
    acasxu_input = np.load('./random_acas_input_' + count + '.npy')
    return acasxu_input

def generate_cifar_input(count):
    if not os.path.exists('./random_cifar_input_' + count + '.npy'):
        generate_random_input('./', count, 'cifar')
    else:
        print('Use saved input')
    cifar_input = np.load('./random_cifar_input_' + count + '.npy')
    return cifar_input

def generate_mnist_input(count):
    if not os.path.exists('./random_mnist_input_' + count + '.npy'):
        generate_random_input('./', count, 'mnist')
    else:
        print('Use saved input')
    mnist_input = np.load('./random_mnist_input_' + count + '.npy')
    return mnist_input

def main(argv):
    path = sys.argv[2]
    count = sys.argv[3]
    model = read_onnx_files(path)
    outputfolder = path
    if sys.argv[1] == '-r':
        read_onnx_files(path)
    elif sys.argv[1] == '-f':
        if 'acas' in path:
            acas_input = generate_acas_input(count)
            make_inferences(model, acas_input, count, './')
        elif 'cifar' in path:
            cifar_input = generate_cifar_input(count)
            make_inferences(model, cifar_input, count, './')
        elif 'mnist' in path:
            mnist_input = generate_mnist_input(count)
            make_inferences(model, mnist_input, count, './')
        else:
            print('Unsupported types of model')
    else:
        print("Invalid parameters")
       
if __name__ == "__main__":
    main(sys.argv[1:])
