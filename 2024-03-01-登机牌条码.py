# 90分题解
# http://118.190.20.162/view.page?gpid=T136
w, s = map(int, input().split())
k = 2**(s+1)

data = input()

def encode(data:str) -> list:
    mode = "big"
    tmp = []
    for c in data:
        
        if c.isalpha():
            if c.isupper():
                if mode == "small":
                    tmp.append(28) # num
                    tmp.append(28) # big
                elif mode == "num":
                    tmp.append(28) # big
                mode = "big"
                tmp.append(ord(c) - ord('A'))
            else:
                if mode == "big":
                    tmp.append(27) # small
                elif mode == "num":
                    tmp.append(27) # small
                mode = "small"
                tmp.append(ord(c) - ord('a'))
        elif '0' <= c <= '9':
            if mode == "big":
                tmp.append(28) # num
            elif mode == "small":
                tmp.append(28) # num
            mode = "num"
            tmp.append(int(c))
            
    if len(tmp) % 2 == 1:
        tmp.append(29)
 
    res = []
    for i in range(0, len(tmp), 2):
        res.append(tmp[i] * 30 + tmp[i + 1])
    return res
                

class PolyMod:
    def __init__(self, coef: list, mod: int):
        self.coef = [c % mod for c in coef]  # 初始化时对每个系数取模
        self.mod = mod
        # 移除高次项的0系数
        while len(self.coef) > 1 and self.coef[-1] == 0:
            self.coef.pop()

    def __repr__(self):
        return f"PolyMod({self.coef}, mod={self.mod})"

    def __add__(self, other):
        max_len = max(len(self.coef), len(other.coef))
        res_coef = [0] * max_len
        for i in range(max_len):
            if i < len(self.coef):
                res_coef[i] += self.coef[i]
            if i < len(other.coef):
                res_coef[i] += other.coef[i]
            res_coef[i] %= self.mod
        return PolyMod(res_coef, self.mod)

    def __sub__(self, other):
        max_len = max(len(self.coef), len(other.coef))
        res_coef = [0] * max_len
        for i in range(max_len):
            if i < len(self.coef):
                res_coef[i] += self.coef[i]
            if i < len(other.coef):
                res_coef[i] -= other.coef[i]
            res_coef[i] %= self.mod
        return PolyMod(res_coef, self.mod)

    def __mul__(self, other):
        res_coef = [0] * (len(self.coef) + len(other.coef) - 1)
        for self_i, self_val in enumerate(self.coef):
            for other_i, other_val in enumerate(other.coef):
                res_coef[self_i + other_i] += self_val * other_val
                res_coef[self_i + other_i] %= self.mod
        return PolyMod(res_coef, self.mod)

    def __floordiv__(self, other):
        dividend = self.coef[:]
        divisor = other.coef[:]
        quotient = [0] * (len(dividend) - len(divisor) + 1)
        while len(dividend) >= len(divisor):
            mult = (dividend[-1] * pow(divisor[-1], -1, self.mod)) % self.mod
            degree_diff = len(dividend) - len(divisor)
            quotient[degree_diff] = mult
            for i in range(len(divisor)):
                dividend[degree_diff + i] -= divisor[i] * mult
                dividend[degree_diff + i] %= self.mod
            while dividend and dividend[-1] == 0:
                dividend.pop()
        return PolyMod(quotient, self.mod), PolyMod(dividend, self.mod)
        
ans = encode(data)
n = 1 + len(ans)
if s == -1:
    if n % w != 0:
        fill = w - n % w
        n += fill
else:
    if (n + k) % w != 0:
        fill = w - (n + k) % w
        n += fill
        
ans = [n] + ans + [900] * fill
MOD = 929
if s != -1:

    ans = list(reversed(ans))

    x_k = PolyMod([0] * k + [1], MOD)

    d_x = PolyMod(ans, MOD)
    a = x_k * d_x
    # print("left: ", a.coef)

    b = PolyMod([1], MOD)
    for i in range(1, k + 1):
        b = b * PolyMod([-(3**i), 1], MOD)

    c, rem = a // b
    rem = rem * PolyMod([-1], MOD)
    # print(rem.coef)
    ans = list(reversed(ans))
    for i in range(len(rem.coef) - 1, -1, -1):
        ans.append(rem.coef[i] % 929)

for i in ans:
    print(i)
