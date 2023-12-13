from IPython.display import display
from graphviz import Digraph
import pandas as pd

def getErrorRate(root,data):
    correct = 0
    for i in range(len(data)):
        row = data.iloc[i]
        correct+=root(row)==row['cl']
    return 1-float(correct)/float(len(data))


def addNode(dgraph, node,data):
    if not data is None:
        clZeroCount = len(data[data['cl']==0])
        clOneCount = len(data[data['cl']==1])
        stats = "\ncl 0: {0}\ncl 1: {1}".format(clZeroCount,clOneCount)
    else:
        stats = ""
    
    if not node.value is None:
        nodeName = str(node.nodeId)
        nodeLabel = (
            "node id: "
            + nodeName
            + "\n"
            + "cl = "
            + str(node.value)
            + stats
        )
        if node.value == 0:
            dgraph.node(nodeName, label=nodeLabel, fillcolor="green", style="filled")
        else:
            dgraph.node(nodeName, label=nodeLabel, fillcolor="red", style="filled")

    else:
        nodeName = str(node.nodeId)
        nodeLabel = (
            "node id: "
            + nodeName
            + "\n"
            + str(node.attr)
            + stats
        )
        dgraph.node(nodeName, label=nodeLabel, fillcolor="yellow", style="filled")
        if not data is None:
            leftNode = addNode(dgraph, node.left, data[data[node.attr]==0])
            rightNode = addNode(dgraph, node.right,data[data[node.attr]==1])
        else:
            leftNode = addNode(dgraph, node.left, None)
            rightNode = addNode(dgraph, node.right, None)
        dgraph.edge(nodeName, leftNode, label="0")
        
        
        dgraph.edge(nodeName, rightNode, label="1")

    return nodeName


def addId(root):
    nodes = [root]
    nodeId = 0
    while len(nodes) > 0:
        node = nodes.pop(0)
        node.nodeId = nodeId
        nodeId += 1
        if  node.value is None:
            nodes.append(node.left)
            nodes.append(node.right)


def printGraph(root,data=None, size=10, fileName="DecisionTree"):
    dgraph = Digraph(format="png", filename=fileName)
    dgraph.attr(size=str(size) + "," + str(size))
    dgraph.node_attr.update()
    addId(root)
    addNode(dgraph, root,data)
    display(dgraph)
    
    
def getTrainingDataSet():
    attributesName = ["attr 1", "attr 2", "attr 3", "attr 4", "attr 5", "attr 6"]
    data = pd.DataFrame([[0, 0, 1, 1, 0, 1, 0],
       [1, 1, 1, 1, 0, 1, 0],
       [0, 1, 1, 0, 1, 0, 1],
       [1, 1, 0, 0, 0, 0, 1],
       [0, 1, 1, 1, 0, 1, 1],
       [1, 1, 0, 1, 1, 1, 0],
       [1, 1, 1, 1, 1, 1, 0],
       [0, 1, 0, 0, 0, 0, 1],
       [1, 0, 1, 0, 1, 1, 1],
       [1, 1, 1, 0, 0, 0, 0],
       [0, 0, 1, 1, 0, 0, 0],
       [0, 0, 1, 1, 0, 1, 1],
       [1, 1, 1, 0, 1, 1, 0],
       [1, 1, 0, 1, 1, 1, 1],
       [0, 1, 0, 0, 0, 0, 0],
       [1, 1, 0, 1, 1, 0, 0],
       [0, 1, 1, 0, 1, 1, 0],
       [1, 0, 1, 0, 1, 1, 1],
       [1, 1, 0, 0, 0, 0, 0],
       [0, 1, 0, 1, 0, 1, 0]], columns = attributesName+["cl"])
    
    return attributesName, data


def getValidationDataSet():
    attributesName = ["attr 1", "attr 2", "attr 3", "attr 4", "attr 5", "attr 6"]
    data = pd.DataFrame([[0, 0, 0, 0, 0, 0, 0],
       [0, 0, 0, 0, 0, 1, 0],
       [0, 0, 0, 1, 1, 1, 1],
       [0, 1, 1, 0, 0, 1, 0],
       [0, 1, 0, 1, 0, 1, 0],
       [1, 0, 0, 1, 1, 1, 1],
       [1, 1, 1, 1, 0, 1, 1],
       [1, 1, 0, 1, 0, 1, 1],
       [1, 1, 0, 0, 1, 0, 0],
       [0, 1, 1, 1, 1, 1, 1]], columns = attributesName+[ "cl"])
    
    return attributesName, data