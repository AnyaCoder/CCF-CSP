# http://118.190.20.162/view.page?gpid=T131

"""
# 我的答案:
Neuron 0: -65.000 and 30.0
Neuron 0: -35.616 and 30.0

# 标准答案:
-35.608 -35.608
2 2

就你妈离谱

"""
from decimal import getcontext, Decimal

getcontext().prec = 17  # 设置全局精度为小数点后16位

next = 1
def myrand():
    global next
    next = (next * 1103515245 + 12345) % (2 ** 64)
    return (next // 65536) % 32768

NeuronImpulseList = []
TuchuList = []

N, S, P, T = map(int, input().split())
delta_t = Decimal(input())
_ = N

class Neuron:
    def __init__(self, id, v, u, a, b, c, d):
        self.id = id
        self.v = Decimal(v)
        self.u = Decimal(u)
        self.a, self.b, self.c, self.d = Decimal(a), Decimal(b), Decimal(c), Decimal(d)
        self.In, self.Out = [], []
        self.I_k, self.act = 0, 0
    
    def accumulate(self, w):
        self.I_k += w
        

    def forward(self):
        print(f"Neuron {self.id}: {self.v:.3f} and {self.I_k}")
        self.v = self.v + delta_t * (Decimal(0.04) * (self.v * self.v) + Decimal(5) * self.v 
                                     + Decimal(140) - self.u) + self.I_k
        self.u = self.u + delta_t * self.a * (self.b * self.v - self.u)
        self.I_k = 0

        if self.v >= 30:
            self.act += 1
            for (t_id, n_id) in self.Out:
                TuchuList[t_id].activate()

            self.v = self.c
            self.u = self.u + self.d


class Tuchu:
    def __init__(self, id, s, t, w, D):
        self.id, self.w, self.D = id, w, D
        self.pair = (s, t)
        NeuronImpulseList[s].Out.append((id, t))
        self.pulse = []

    def forward(self):
        # print(f"Tuchu qian{self.id}: {self.pulse}")

        tmp = list(reversed(self.pulse))
        for idx in range(len(tmp)):
            tmp[idx] -= 1
        while tmp and tmp[-1] == 0:
            NeuronImpulseList[self.pair[1]].accumulate(self.w)
            tmp.pop()
        self.pulse = list(reversed(tmp))
        del tmp
        # print(f"Tuchu hou{self.id}: {self.pulse}")

    def activate(self):
        self.pulse.append(self.D)


class Impulse:
    def __init__(self, id, r):
        self.id = id
        self.r = r
        self.Out = []
    
    def forward(self):
        randnum = myrand()
        if self.r > randnum:
            # print(f"Impulse {self.id}: {self.Out}")
            for (t_id, n_id) in self.Out:
                TuchuList[t_id].activate()
    

cnt = 0
while _ > 0:
    nums = input().split()
    R_N = int(nums[0])
    v, u, a, b, c, d = map(float, nums[1:])
    # print(R_N)
    # print(v, u, a, b, c, d)
    NeuronImpulseList += [Neuron(cnt, v, u, a, b, c, d) for __ in range(R_N)]
    _ -= R_N
    cnt += 1


for _ in range(P): # 脉冲源
    r = int(input())
    NeuronImpulseList.append(Impulse(_ + N, r))


for t_id in range(S): # 突触
    nums = input().split()
    s, t = map(int, nums[0:2])
    w = Decimal(nums[2])
    D = int(nums[3])
    TuchuList.append(Tuchu(t_id, s, t, w, D))
    # print(s, t, w, D)


for _t in range(T):
    # print(f"Tick: {_t + 1}")
    
    for impulse in NeuronImpulseList[N:]:
        impulse.forward()

    for tuchu in TuchuList:
        tuchu.forward()

    for neuron in NeuronImpulseList[:N]:
        neuron.forward()

min_v, max_v = 2**64, -(2**64)
min_times, max_times = 2**64, 0
# ans
for neuron in NeuronImpulseList[:N]:
    min_v = min(neuron.v, min_v)
    max_v = max(neuron.v, max_v)
    min_times = min(neuron.act, min_times)
    max_times = max(neuron.act, max_times)

print("%.3f %.3f" % (min_v, max_v))
print(min_times, max_times)
