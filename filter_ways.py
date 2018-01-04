import osmium as o
import sys

highway_tags = ['service', 'motorway','trunk', 'primary', 'secondary', 'tertiary', 'unclassified', 'residential']

class WayFilter(o.SimpleHandler):

    def __init__(self):
        super(WayFilter, self).__init__()
        self.nodes = {}

    def way(self, w):
        if 'highway' in w.tags and w.tags['highway'] in highway_tags and len(w.nodes) > 1:
            for n in w.nodes:
                self.nodes[n.ref] = []


class NodeFilter(o.SimpleHandler):

    def __init__(self, nodes):
        super(NodeFilter, self).__init__()
        self.nodes = nodes

    def node(self, n):
        if n.id in self.nodes.keys():
            self.nodes[n.id] = [n.location.lat,n.location.lon]



class WayWriter(o.SimpleHandler):

    def __init__(self,nodes,f):
        super(WayWriter, self).__init__()
        self.nodes = nodes
        self.f = f

    def way(self,w):
        if 'highway' in w.tags and w.tags['highway'] in highway_tags and len(w.nodes) > 1:
            self.f.write(str(w.id))
            self.f.write(", "+w.tags['highway'])
            for n in w.nodes:
                self.f.write(", "+str(self.nodes[n.ref][0])+", "+str(self.nodes[n.ref][1]))
                #self.f.write(str(self.nodes[n.ref][0]))
                #self.f.write(", ")
                #self.f.write(str(self.nodes[n.ref][1]))
            self.f.write("\n")


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python filter_ways.py <infile> <outfile>")
        sys.exit(-1)


    # go through the ways to find all relevant nodes
    ways = WayFilter()
    ways.apply_file(sys.argv[1])

    nodesf = NodeFilter(ways.nodes)
    nodesf.apply_file(sys.argv[1])

    f = open(sys.argv[2], 'w')
    WayWriter(nodesf.nodes,f).apply_file(sys.argv[1])
    f.close()