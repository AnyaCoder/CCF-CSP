# http://118.190.20.162/view.page?gpid=T141 
from collections import defaultdict, OrderedDict
n, m = map(int, input().split())
L = list(input().split())
L = [int(i) for i in L]
L_dict = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
area_dict = defaultdict(int)

for node, area in enumerate(L):
    L_dict[area][node + 1] = defaultdict(int)
    area_dict[node + 1] = area
    
g = int(input())
G = []

class DictToAttr:
    def __init__(self, dictionary):
        self.__dict = dictionary  # 将原始字典保存为私有属性
        for key, value in dictionary.items():
            setattr(self, key, value)

    def __str__(self):
        return str(self.__dict)  # 返回保存的字典的字符串表示
            
for _ in range(g):
    f, a, na, pa, paa, paar = map(int, input().split())
    G.append(DictToAttr(dict(f=f, a=a, na=na, pa=pa, paa=paa, paar=paar)))
  

for item in G:
    for _ in range(item.f):
        
        if (item.na > 0 and len(L_dict[item.na]) == 0):
            print(0, end=" ")
            continue
                        
        filtered_nodes = defaultdict(lambda: defaultdict(int))

        if item.na == 0:

            for area, nodes in L_dict.items():
                ok = False
                for node, tasks in nodes.items():
                    if item.pa == 0 or item.pa in tasks.keys():
                      ok = True
                      break
                if ok:
                    for node, tasks in nodes.items():
                        if item.paa not in tasks.keys():
                            filtered_nodes[node] = tasks
            
        else:
            ok = False
            for node, tasks in L_dict[item.na].items():
                if item.pa == 0 or item.pa in tasks.keys():
                    ok = True
                    break
            if ok:
                for node, tasks in L_dict[item.na].items():
                    if item.paa not in tasks.keys():
                        filtered_nodes[node] = tasks
                
        if len(filtered_nodes.items()) == 0:
             
            # import json
            # print()
            # print(json.dumps(L_dict, indent=4))
            # print("")
            if item.paar == 1:
                print(0, end=" ")
                continue
            else:
                if item.na == 0:
                    for area, nodes in L_dict.items():
                        for node, tasks in nodes.items():
                            if (item.pa == 0 or item.pa in tasks.keys()):
                                filtered_nodes[node] = tasks
                else:
                    for node, tasks in L_dict[item.na].items():
                        if (item.pa == 0 or item.pa in tasks.keys()):
                            filtered_nodes[node] = tasks
        
        if len(filtered_nodes.items()) == 0:
            print(0, end=" ")
            continue
        
        # 按插入顺序排列的字典
        filtered_nodes = OrderedDict(sorted(filtered_nodes.items(), 
                                     key=lambda x: (sum(x[1].values()), x[0])))

        
        for node, tasks in filtered_nodes.items():
            print(node, end=" ")
            if item.na != 0:
                L_dict[item.na][node][item.a] += 1
            else:
                L_dict[area_dict[node]][node][item.a] += 1
                
            break
    
    print("")
