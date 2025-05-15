import numpy as np
import tensorflow as tf
import seaborn as sns
from matplotlib import pyplot as plt
from sklearn.metrics import confusion_matrix
import itertools

from IPython.core.display import display, HTML
from IPython.display import clear_output, Image, display, HTML

	
def execute_tf_graph(outputs, inputs=None):
    if type(outputs) not in {list, tuple}:
        outputs = [outputs]
    
    with tf.Session() as sess:
        init = tf.global_variables_initializer()
        sess.run(init)
        
        ret = sess.run(outputs, feed_dict=inputs)
        
    return ret
    
    
def strip_consts(graph_def, max_const_size=32):
    strip_def = tf.GraphDef()
    
    for n0 in graph_def.node:
        n = strip_def.node.add() 
        n.MergeFrom(n0)
        if n.op == 'Const':
            tensor = n.attr['value'].tensor
            size = len(tensor.tensor_content)
            if size > max_const_size:
                tensor.tensor_content = "<stripped %d bytes>"%size
    return strip_def
    

def show_graph(graph_def, width=600, height=300, max_const_size=32, ungroup_gradients=False):
    strip_def = strip_consts(graph_def, max_const_size=max_const_size)
    data = str(strip_def)
    
    if ungroup_gradients:
        data = data.replace('"gradients/', '"b_')
    code = """
        <script>
          function load() {{
            document.getElementById("{id}").pbtxt = {data};
          }}
        </script>
        <link rel="import" href="https://tensorboard.appspot.com/tf-graph-basic.build.html" onload=load()>
        <div style="height:600px">
          <tf-graph-basic id="{id}"></tf-graph-basic>
        </div>
    """.format(data=repr(data), id='graph'+str(np.random.rand()))

    iframe = """
        <iframe seamless style="width:{}px;height:{}px;border:0" srcdoc="{}"></iframe>
    """.format(width, height, code.replace('"', '&quot;'))
    display(HTML(iframe))
    

def plot_confusion_matrix(y, preds, title='Confusion matrix', cmap=plt.cm.Blues):   
    classes = [i for i in range(10)]
    cm = confusion_matrix(y, preds)
    cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
    cm = np.round(cm, decimals=2)
    
    plt.figure(figsize=(6, 6))
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    
    
