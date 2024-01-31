// https://blog.csdn.net/qq_51594008/article/details/135006254
#include "bits/stdc++.h"
using namespace std;
#define For(i, j, n) for(int i=j;i<=n;i++)
#define Fol(i, j, n) for(int i=j;i>=n;i--)
typedef long long LL;
const int N = 2e3 + 5;
struct node {
    int w;//自身权重
    LL wt;//自己与后代的权重
    unordered_set<int> son;//孩子们
} tr[N];
bool st[N];//判断这个类别是否被去除
set<int> seg;//待查询的类别集和
 
//更新以root为根的树上所有节点的wt
LL dfs(int root, set<int> &seg) {
    seg.insert(root);
    LL res = 0;
    for (auto child: tr[root].son) {
        if (st[child]) continue;
        res += dfs(child, seg);
    }
    tr[root].wt = res + tr[root].w;
    return tr[root].wt;
}
//查询wsigma最小的节点
int query(int root, set<int> &seg) {
    LL wmin = LONG_LONG_MAX, pos = -1;
    for (auto x: seg) {
        LL wsigma = abs(tr[root].wt - 2 * tr[x].wt);
        if (wmin > wsigma) {
            wmin = wsigma;
            pos = x;
        }
    }
    return pos;
}
//用户询问判断ch是否被归类fa或者fa的后代
bool judge(int fa, int ch) {
    if (fa == ch) return true;
    bool flag = false;
    for (auto x: tr[fa].son) {
        flag |= judge(x, ch);
        if(flag) break;
    }
    return flag;
}
int main() {
    int n, m, fa;
    scanf("%d %d", &n, &m);
    For(i, 1, n) scanf("%d", &tr[i].w);
    For(i, 2, n) {
        scanf("%d", &fa);
        tr[fa].son.insert(i);
    }
    For(i, 1, m) {
        memset(st, false, sizeof st);
        int root = 1, x;
        scanf("%d", &x);
        while (1) {
            seg.clear();
            dfs(root, seg);
            if (seg.size() == 1)break;//直到只剩下一个类别，此时即可确定名词的类别
            int id = query(root, seg);
            printf("%d ", id);
            if (judge(id, x)) root = id;//如果用户回答是，保留该类别及其后代类别
            else st[id] = true;//否则仅保留其余类别
        }
        puts("");
    }
    return 0;
}
