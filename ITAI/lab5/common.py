from graphviz import Digraph

def PrintGraph(graph,filename ='Graph',size=6,only_render=False,cleanup =True):
    dgraph = Digraph(format='png',filename=filename)
    dgraph.attr(size=str(size)+','+str(size))
    dgraph.node_attr.update()
    for v in graph:
        dgraph.node(str(v))                
        for v_2 in graph[v]:               
            dgraph.edge(str(v),str(v_2))

    if only_render:
        dgraph.render(cleanup=cleanup)
    else:
        dgraph.view(cleanup=cleanup)
        
        
def getCycle(graph, start, max_iter=2000):
    fringe = [(start, [])]
    step=0
    while fringe and step <max_iter:        
        step+=1
        vertex, path = fringe.pop()
        if path and vertex == start:
            return path

        for next_vertex in graph[vertex]:
            if next_vertex in path:
                continue
            fringe.append((next_vertex, path+[next_vertex]))
    return []