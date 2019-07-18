from codeFormat import sub_variable
from codeFormat import remove_comment as rc
path1 = '../test_code/实验4测试题/A2.c++.txt'
path2 = '../test_code/实验4测试题/B2.c++.txt'

#----------------------------------------------------------------
# 字符串lcs：用于测试行最大匹配
def lcs(code_1:str,code_2:str):
    len_1 = len(code_1)
    len_2 = len(code_2)

    dp = [[0 for i in range(len_2+1)]for j in range(len_1+1)]

    for i in range(len_1+1):
        for j in range(len_2+1):
            if i==0 or j==0:
                dp[i][j] = 0
            elif code_1[i-1] == code_2[j-1]:
                dp[i][j]=dp[i-1][j-1]+1
            else:
                dp[i][j]=max(dp[i-1][j],dp[i][j-1])
    # print(dp)
    return dp[len_1][len_2]/min(len_1,len_2)

# 字符串lcs：空间优化为On
def lcs_optimize(code_1:str,code_2:str):
    if len(code_1) < len(code_2):
        code_1,code_2 = code_2,code_1
    row,column = len(code_1),len(code_2)
    dp = [0 for i in range(column+1)]
    for i in range(1,row+1):
        record = 0
        for j in range(1,column+1):
            tmp = dp[j]
            if code_1[i-1]==code_2[j-1]:
                dp[j]=record+1
            else:
                dp[j]=max(dp[j-1],dp[j])
            record = tmp
    return dp[column]/min(row,column)

def levenshtein(code_1:str,code_2:str):
    len_1,len_2=len(code_1),len(code_2)
    dp = [[0 for i in range(len_2+1)]for j in range(len_1+1)]
    for i in range(1,len_1+1):
        for j in range(1,len_2+1):
            if code_1[i-1] == code_2[j-1]:
                tmp=0
            else:
                tmp=1
            dp[i][j]=min(dp[i-1][j-1]+tmp,dp[i][j-1]+1,dp[i-1][j]+1)
    return 1-(dp[len_1][len_2]/max(len_1,len_2))
# 矩阵最大匹配子序列，用于匹配代码整体近似度

def levenshtein_matrix(matrix:list):
    row,column = len(matrix),len(matrix[0])
    dp = [[0 for i in range(column+1)] for j in range(row + 1)]
    for i in range(1,row+1):
        for j in range(1,column+1):
            if matrix[i-1][j-1]==1:
                tmp=0
            else:
                tmp=1
            dp[i][j]=min(dp[i-1][j-1]+tmp,dp[i][j-1]+1,dp[i-1][j]+1)
    return 1-(dp[row][column]/max(row,column))

def lcs_matrix(matrix:list):
    row,column = len(matrix),len(matrix[0])

    dp = [[0 for i in range(column + 1)] for j in range(row + 1)]
    for i in range(row + 1):
        for j in range(column + 1):
            if i == 0 or j == 0:
                dp[i][j] = 0
            elif matrix[i - 1][j - 1] == 1:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
    return dp[row][column]/min(row,column)

# 矩阵最大匹配lcs，空间优化为On
def lcs_matrix_optimize(matrix:list):
    row,column = len(matrix),len(matrix[0])
    dp = [0 for i in range(column+1)]
    for i in range(1,row+1):
        record = 0
        for j in range(1,column+1):
            tmp = dp[j]
            if matrix[i-1][j-1]==1:
                dp[j]=record+1
            else:
                dp[j]=max(dp[j-1],dp[j])
            record=tmp
    return dp[column]/min(row,column)

#二分查找，返回第一个大于key的下标，否则返回数组结尾
def lower_bound(seq:list,length:int,key:int):
    left,right = 0,length-1
    idx = length
    while left<right:
        mid = int((left+right)/2)
        if seq[mid]<key:
            left = mid+1
        else:
            right=mid
    if seq[left]>=key:
        idx = left
    return idx
# lcs nlogn优化：转成LIS
def lcs_nlogn(code_1:str,code_2:str):
    len_1,len_2 = len(code_1),len(code_2)
    map = {}    # 记录code1每个字符出现的索引
    seq = []    # 索引序列
    # 构建code1索引关系
    for i in range(len_1)[::-1]:
        if code_1[i] in map:
            map[code_1[i]].append(i)
        else:
            map[code_1[i]] = [i]
    # 找出code2存在的键值，将索引加入seq
    for j in range(len_2):
        if code_2[j] in map:
            seq = seq + map[code_2[j]]
    #----LIS------
    cnt=0
    length = len(seq)
    if length == 0: # 长度为0
        return 0
    res = [0 for i in range(length)]
    res[0]=seq[0]
    for i in range(1,length):
        if seq[i] > res[cnt]:
            res[cnt+1]=seq[i]
            cnt+=1
        else:
            res[lower_bound(res,length=cnt,key=seq[i])]=seq[i]
    # 匹配率
    return (cnt+1)/min(len_1,len_2)
#-----------------------------------------------------------------

# 获取相似矩阵/邻接矩阵
def get_similarMatrix(code_1:list,code_2:list,is_ad:bool,parms:float,match_method:int):
    row,column = len(code_1),len(code_2)
    similar_matrix = [[]for i in range(row)]    # init

    for i in range(row):
        for j in range(column):
            if match_method==0:
                value = int(lcs_optimize(code_1[i], code_2[j]) > parms)
            elif match_method==1:
                value = int(levenshtein(code_1[i], code_2[j]) > parms)
            else:
                value = int(lcs_nlogn(code_1[i], code_2[j]) > parms)

            # 大于设定阈值可看出近似，为1，否则为0
            if is_ad :
                if value==1: similar_matrix[i].append(j)
            else:
                similar_matrix[i].append(value)
    return similar_matrix
#-----------------------------------------------------------------

MAX_SIZE = 2000
visit = [0 for i in range(MAX_SIZE)]
match = [-1 for i in range(MAX_SIZE)]

# 查找是否能进行匹配
def hungarain_find(idx:int,ad_matrix:int):

    for edge in range(len(ad_matrix[idx])):
        col = ad_matrix[idx][edge]
        if not visit[col]:
            visit[col]=True
            if match[col]==-1 or hungarain_find(match[col],ad_matrix):
                match[col]=idx
                return True
    return False

# 匈牙利算法，用于求解两代码最大近似匹配
def hungarain_matrix(ad_matrix:list,row:int,column:int):
    cnt = 0
    for col in range(column):match[col]=-1
    for idx in range(row):
        for col in range(column):visit[col]=False
        if hungarain_find(idx,ad_matrix):
            cnt+=1
    # print(match)
    return cnt/min(row,column)


#---------------------------------------------------------------------
# 切割匹配---弃之不用
def divide_check(code_1:list,code_2:list,parms:float,parms_m:float):
    row,column = len(code_1),len(code_2)
    similar_matrix = [[] for i in range(row)]  # init
    for i in range(row):
        for j in range(column):
            m = get_similarMatrix(code_1[i],code_2[j],True,parms)
            # print(m)
            value = int(hungarain_matrix(m,len(code_1[i]),len(code_2[j]))/min(len(code_1[i]),len(code_2[j]))>parms_m)
            if value==1:
                similar_matrix[i].append(j)
    """
    for i in range(row):
        print(similar_matrix[i])
    """
    return  hungarain_matrix(similar_matrix,row,column)/min(row,column)

#-----------------------------------------------------------------------
import networkx as nw
# 最大流求解二分图最大匹配
def dinic(matrix_ad:list,row:int,column:int):
    net = nw.DiGraph()
    """
        在原二分图的基础上需增加起始点和终点
        分别与code1和code2的结点互连
        总结点数为 col+row+2个
        其中，code1结点编号为1-row
        code2结点编号为row+1-row+col
    """
    start = 0   # 起始点
    end = row+column+1  #终点

    # 添加点,col+row，及两个端点
    for i in range(row+column+2):
        net.add_node(i)
    # 将邻接图译为边集
    for i in range(row):
        for j in matrix_ad[i]:
            u,v,c = i+1,j+row+1,1
            net.add_edge(u,v,capacity=c)
    #两端端点分别连接code1和code2的结点
    for i in range(row):
        u,v,c =start,i+1,1
        net.add_edge(u,v,capacity=c)
    for j in range(column):
        u,v,c=j+row+1,end,1
        net.add_edge(u,v,capacity=c)
    # 此时得到的最大流便为code1和code2的最大匹配行数
    max_flow =  nw.algorithms.flow.maximum_flow(net,start,end)[0]
    return max_flow/min(row,column)

#------------------------------------------------------------------------
def check_byLCS():
    # 变量代换
    code1, code2 = sub_variable.sub_var_main(code_path_1=path1, code_path_2=path2)
    #不进行变量代换
    # code1,code2 = rc.format_code_main(code_path=path1),rc.format_code_main(code_path=path2)
    len_1 = len(code1)
    len_2 = len(code2)
    import timeit
    """
    for line in code1:
        print(line)
    """
    """
        计时
        start = timeit.default_timer()
        ... do something
        elapsed = (timeit.default_timer() - start)
    """
    # 生成混淆矩阵
    start = timeit.default_timer()
    similar_Matrix = get_similarMatrix(code1,code2,False,0.88,match_method=2)
    print('time:' + str(timeit.default_timer() - start))

    # 生成混淆邻接矩阵
    start = timeit.default_timer()
    similar_Matrix_ad = get_similarMatrix(code1,code2,True,0.88,match_method=2)
    print('time:' + str(timeit.default_timer() - start))

    print(similar_Matrix)
    #
    start = timeit.default_timer()
    print(lcs_matrix(matrix=similar_Matrix))
    print('time:'+str(timeit.default_timer() - start))
    """
    for vec in range(len(similar_Matrix_ad)):
        print(similar_Matrix_ad[vec])
    print(len_1,len_2)
    """
    start = timeit.default_timer()
    print(lcs_matrix_optimize(matrix=similar_Matrix))
    print('time:' + str(timeit.default_timer() - start))


    start = timeit.default_timer()
    print(hungarain_matrix(ad_matrix=similar_Matrix_ad,row=len_1,column=len_2))
    print('time:' + str(timeit.default_timer() - start))

    start = timeit.default_timer()
    print(dinic(matrix_ad=similar_Matrix_ad,row=len_1,column=len_2))
    print('time:' + str(timeit.default_timer() - start))
    """
    new_code1 = rc.code_slicing(code=code1,step=5)
    new_code2 = rc.code_slicing(code=code2,step=5)

    print(divide_check(new_code1,new_code2,0.88,0.5))
    """
# check_byLCS()