import osmium as o
import sys

highway_tags = ['service', 'motorway','trunk', 'primary', 'secondary', 'tertiary', 'unclassified', 'residential']
service = None
motorway = None
trunk = None
primary = None
secondary = None
tertiary = None
unclassified = None
residential = None 

class WayFilter(o.SimpleHandler):

    def __init__(self):
        super(WayFilter, self).__init__()
        self.nodes = {}

    def way(self, w):
        if 'highway' in w.tags and w.tags['highway'] in highway_tags and len(w.nodes) in range(2,2000):
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
        if 'highway' in w.tags and w.tags['highway'] in highway_tags and len(w.nodes) in range(2,2000):
            self.f.write(str(w.id))
            self.f.write(", "+w.tags['highway'])

            temp = None
            if w.tags['highway'] == 'service':
            	temp = service
            elif w.tags['highway'] == 'motorway':
            	temp = motorway
            elif w.tags['highway'] == 'trunk':
            	temp = trunk
            elif w.tags['highway'] == 'primary':
            	temp = primary
            elif w.tags['highway'] == 'secondary':
            	temp = secondary
            elif w.tags['highway'] == 'tertiary':
            	temp = tertiary
            elif w.tags['highway'] == 'unclassified':
            	temp = unclassified
            else:
            	temp = residential

            temp.write(str(w.id)+", "+w.tags['highway'])

            for n in w.nodes:
                self.f.write(", "+str(self.nodes[n.ref][0])+", "+str(self.nodes[n.ref][1]))
                temp.write(", "+str(self.nodes[n.ref][0])+", "+str(self.nodes[n.ref][1]))
            self.f.write("\n")
            temp.write("\n")


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python filter_ways.py <infile> <outfile>")
        sys.exit(-1)


    # go through the ways to find all relevant nodes
    ways = WayFilter()
    ways.apply_file(sys.argv[1])

    nodesf = NodeFilter(ways.nodes)
    nodesf.apply_file(sys.argv[1])

    service = open(sys.argv[2]+'_service.csv', 'w')
    motorway = open(sys.argv[2]+'_motorway.csv','w')
    trunk = open(sys.argv[2]+'_trunk.csv','w')
    primary = open(sys.argv[2]+'_primary.csv','w')
    secondary = open(sys.argv[2]+'_secondary.csv','w')
    tertiary = open(sys.argv[2]+'_tertiary.csv','w')
    unclassified = open(sys.argv[2]+'_unclassified.csv','w')
    residential = open(sys.argv[2]+'_residential.csv','w') 
    f = open(sys.argv[2]+'.csv', 'w')
    WayWriter(nodesf.nodes,f).apply_file(sys.argv[1])
    f.close()
    service.close()
    motorway.close()
    trunk.close()
    primary.close()
    secondary.close()
    tertiary.close()
    unclassified.close()
    residential.close()