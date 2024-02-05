# 读取角色数量、角色关联数量、待检查的操作数量
# http://118.190.20.162/view.page?gpid=T146
n, m, q = map(int, input().split())

# 读取角色信息
roles = {}
for _ in range(n):
    role_info = input().split()
    name = role_info[0]
    num_operations = int(role_info[1])
    operations = set(role_info[2:2 + num_operations])
    num_resources = int(role_info[2 + num_operations])
    resources = set(role_info[3 + num_operations:3 + num_operations + num_resources])
    num_resource_names = int(role_info[3 + num_operations + num_resources])
    resource_names = set(role_info[4 + num_operations + num_resources:])
    roles[name] =  { 
                    "operations": operations, 
                    "resources": resources, 
                    "resource_names": resource_names
                }


G = {}
U = {}
# 读取角色关联信息
role_relations = []
for _ in range(m):
    relation_info = input().split()
    role_name = relation_info[0]
    num_authorizations = int(relation_info[1])
    authorizations = [(relation_info[i], relation_info[i + 1]) for i in range(2, 2 + 2 * num_authorizations, 2)]
    if role_name not in G.keys():
        G[role_name] = set()
    if role_name not in U.keys():
        U[role_name] = set()
    for (ty, name) in authorizations:
        if ty == 'g':
            G[role_name].add(name)
        else:
            U[role_name].add(name)
    role_relations.append((role_name, authorizations))

# print(G)
# print(U)
# 读取待授权的行为信息
actions = []
for _ in range(q):
    action_info = input().split()
    user_name = action_info[0]
    num_groups = int(action_info[1])
    groups = action_info[2:2 + num_groups]
    operation_name = action_info[2 + num_groups]
    resource_type = action_info[3 + num_groups]
    resource_name = action_info[4 + num_groups]
    actions.append((user_name, groups, operation_name, resource_type, resource_name))


# 打印读取的数据，你可以根据需要进行进一步处理
# print("角色信息:", roles)
# print("角色关联信息:", role_relations)
# print("待授权的行为信息:", actions)

for _ in range(q):
    user_name = actions[_][0]
    num_groups = len(actions[_][1])
    groups = actions[_][1]
    operation_name = actions[_][2]
    resource_type = actions[_][3]
    resource_name = actions[_][4]

    agree = False

    for char_name, related_users in U.items():
        if agree:
            break
        if user_name in related_users:
            # print(operation_name, resource_type, resource_name)
            if operation_name not in roles[char_name]["operations"] \
                and '*' not in roles[char_name]["operations"]:
                continue
            if resource_type not in roles[char_name]["resources"] \
                and '*' not in roles[char_name]["resources"]:
                continue
            if resource_name not in roles[char_name]["resource_names"] \
                and len(roles[char_name]["resource_names"]) > 0:
                continue
            agree = True
            break


    for char_name, char_group in G.items():
        # print(char_name, char_group)
        if agree:
            break     
        for g in groups: # 当前待查角色的所在的组中
            
            if g in char_group: # 找到待查角色在某个预设角色的组里
                # print(operation_name, resource_type, resource_name)
                if operation_name not in roles[char_name]["operations"] \
                    and '*' not in roles[char_name]["operations"]:
                    continue
                
                if resource_type not in roles[char_name]["resources"] \
                    and '*' not in roles[char_name]["resources"]:
                    continue

                if resource_name not in roles[char_name]["resource_names"] \
                    and len(roles[char_name]["resource_names"]) > 0:
                    continue
                
                agree = True
                break
       
        
    print(1 if agree else 0)
