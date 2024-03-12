# http://118.190.20.162/view.page?gpid=T111
Q = int(input())

class Device:
    def __init__(self, FUNC, L):
        self.func = FUNC
        self.input = L
        self.val = None
        self.loop = False
    def NOT(self, _in):
        return 1 if _in[0] == 0 else 0

    def AND(self, _in):
        res = 1
        for item in _in:
            res = res & item
        return res

    def OR(self, _in):
        res = 0
        for item in _in:
            res = res | item
        return res

    def XOR(self, _in):
        res = 0
        for item in _in:
            res = res ^ item
        return res

    def NAND(self, _in):
        return 1 if self.AND(_in) == 0 else 0

    def NOR(self, _in):
        return 1 if self.OR(_in) == 0 else 0

    def compute(self, _in):
        if self.func == "NOT":
            self.val = self.NOT(_in)
        elif self.func == "AND":
            self.val = self.AND(_in)
        elif self.func == "OR":
            self.val = self.OR(_in)
        elif self.func == "XOR":
            self.val = self.XOR(_in)
        elif self.func == "NAND":
            self.val = self.NAND(_in)
        elif self.func == "NOR":
            self.val = self.NOR(_in)
    def recur_solve(self, device_map, input_map, route):  # 所有仪器， 所有输入信号
        if self.loop:
            return 0
        tmp = []
        if self.val is None:
            for name in self.input:
                if name in route:
                    self.loop = True
                    return 0
                if name[0] == "I":
                    tmp.append(input_map[int(name[1:])])
                elif name in device_map.keys():
                    if device_map[name].val is None:
                        route.add(name)
                        device_map[name].recur_solve(device_map, input_map, route)
                        route.remove(name)
                    if device_map[name].val is None:
                        self.loop = True
                        return 0
                    tmp.append(device_map[name].val)
            self.compute(tmp)
        return self.val

    def clear(self):
        self.val = None

for _ in range(Q):
    DeviceMap = {}

    M, N = map(int, input().split())
    for i in range(N):
        desc = input().split()
        FUNC = desc[0]
        k = int(desc[1])
        L = desc[2:]
        DeviceMap[f"O{i + 1}"] = Device(FUNC, L)

    S = int(input())
    InputMapList = []
    for i in range(S):
        I = input().split()
        InputMap = {}
        for j in range(len(I)):
            InputMap[j + 1] = int(I[j]) # 输入
        InputMapList.append(InputMap)

    OutputMapList = []
    for i in range(S):
        sO = input().split()
        s = int(sO[0])
        Output_lst = []
        O = sO[1:]
        for j in range(len(O)):
            Output_lst.append(int(O[j]))
        OutputMapList.append(Output_lst)

    for i in range(S):
        for j in DeviceMap.values():
            j.clear()

        output_lst = OutputMapList[i]
        input_map = InputMapList[i]

        is_loop = False
        for j in range(1, N + 1):
            DeviceMap[f"O{j}"].recur_solve(DeviceMap, input_map, set())
            if DeviceMap[f"O{j}"].loop:
                is_loop = True
                break
        if is_loop:
            print("LOOP")
            break
        else:
            for j in output_lst:
                print(DeviceMap[f"O{j}"].val, end=' ')
            print("")
