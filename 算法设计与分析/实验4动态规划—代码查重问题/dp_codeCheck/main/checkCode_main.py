from codeFormat import remove_comment as rc
from codeFormat import  sub_variable as sv
from algorithm_codeCheck import code_check as cc
import timeit

default_path = {
    0:'../test_code/实验4测试题/{}0.c++.txt',
    1:'../test_code/实验4测试题/{}1.c++.txt',
    2:'../test_code/实验4测试题/{}3.c++.txt',
    3:'自定义'
}
varsub_method = {
    0:'进行变量代换',
    1:'不进行变量代换'
}
match_method = {
    0:'LCS',
    1:'Levenshtein',
    2:'LCS-nlogn'
}
# ('../test_code/实验4测试题/A0.c++.txt', '../test_code/实验4测试题/B0.c++.txt', False, [], 1, 1, 0.88)
compare_method = {
    0:'LCS',
    1:'Levenshtein',
    2:'hungarain',
    3:'dinic',
}

def win():
    for key,value in default_path.items():
        print(str(key) + "-" + value)
    path_key = int(input('请选择文件载入方式:'))
    if path_key==3:
        code1_path = input('请输入代码文件1路径:')
        code2_path = input('请输入代码文件2路径:')
    else:
        code1_path = default_path[path_key].format('A')
        code2_path = default_path[path_key].format('B')
    print("载入路径[1]:"+code1_path)
    print("载入路径[2]:"+code2_path)
    print("=======================================")

    for key,value in varsub_method.items():
        print(str(key) + "-" + value)
    sub_key = int(input("请选择变量代换方式："))
    if sub_key == 0:
        isVar_sub = True
        num, type, var = map(float,input("变量出现次数，数据类型，变量名的匹配权重(单项不超过100):").strip().split())
        Filter = [num,type,var]
    else:
        isVar_sub = False
        Filter = []

    print("======================================")

    for key,value in match_method.items():
        print(str(key) + '-' + value)
    match_key = int(input("请选择代码行匹配算法:"))
    print("=======================================")

    for key,value in compare_method.items():
        print(str(key) + '-' + value)
    compare_key = int(input("请选择代码整体匹配算法"))
    print("=======================================")

    parms = float(input("请输入代码行匹配阈值:"))
    print("=======================================")

    return code1_path,code2_path,isVar_sub,Filter,match_key,compare_key,parms

def do_check(code1_path:str,code2_path:str,is_sub:bool,Filter:list,
             match_key:int,compare_key:int,parms:float):

    start = timeit.default_timer()

    if is_sub:
        code1,code2 = sv.sub_var_main(code1_path,code2_path,filter=Filter)
    else:
        code1,code2 = rc.format_code_main(code_path=code1_path),rc.format_code_main(code_path=code2_path)

    if compare_key == 0 or compare_key == 1:
        similar_matrix = cc.get_similarMatrix(
                code_1=code1,code_2=code2,is_ad=False,parms=parms,match_method=match_key)
    else:
        similar_matrix = cc.get_similarMatrix(
                code_1=code1,code_2=code2,is_ad=True, parms=parms,match_method=match_key)

    if compare_key==0:
        check_rate = cc.lcs_matrix_optimize(matrix=similar_matrix)
    elif compare_key==2:
        check_rate = cc.hungarain_matrix(ad_matrix=similar_matrix,row=len(code1),column=len(code2))
    elif compare_key==1:
        check_rate = cc.levenshtein_matrix(matrix=similar_matrix)
    else:
        check_rate = cc.dinic(matrix_ad=similar_matrix,row=len(code1),column=len(code2))
    check_rate*=100
    cost_time = timeit.default_timer() - start

    print('查重比例：%.2f'%check_rate)
    #print('查重用时：%.2f'%cost_time)

if __name__ == '__main__':
    # print(win())
    # code1_path, code2_path, isVar_sub, Filter, match_key, compare_key, parms = win()
    # print(code1_path, code2_path, isVar_sub, Filter, match_key, compare_key, parms)
    """
    do_check(
        code1_path = code1_path,
        code2_path = code2_path,
        is_sub = isVar_sub,
        Filter = Filter,
        match_key = match_key,
        compare_key = compare_key,
        parms = parms
    )
    """
    do_check(
        code1_path='../test_code/实验4测试题/A2.c++.txt',
        code2_path='../test_code/实验4测试题/B2.c++.txt',
        is_sub=False,
        Filter=[1, 1.5, 1.5],
        match_key=2,
        compare_key=0,
        parms=0.9
    )

    do_check(
        code1_path='../test_code/实验4测试题/A2.c++.txt',
        code2_path='../test_code/实验4测试题/B2.c++.txt',
        is_sub=True,
        Filter=[1, 1.5, 1.5],
        match_key=2,
        compare_key=0,
        parms=0.9
    )
    #----------------------------------------
    do_check(
        code1_path='../test_code/实验4测试题/A2.c++.txt',
        code2_path='../test_code/实验4测试题/B2.c++.txt',
        is_sub=False,
        Filter=[1, 1.5, 1.5],
        match_key=2,
        compare_key=1,
        parms=0.9
    )

    do_check(
        code1_path='../test_code/实验4测试题/A2.c++.txt',
        code2_path='../test_code/实验4测试题/B2.c++.txt',
        is_sub=True,
        Filter=[1, 1.5, 1.5],
        match_key=2,
        compare_key=1,
        parms=0.9
    )
    do_check(
        code1_path='../test_code/实验4测试题/A2.c++.txt',
        code2_path='../test_code/实验4测试题/B2.c++.txt',
        is_sub=False,
        Filter=[1, 1.5, 1.5],
        match_key=2,
        compare_key=2,
        parms=0.9
    )

    do_check(
        code1_path='../test_code/实验4测试题/A2.c++.txt',
        code2_path='../test_code/实验4测试题/B2.c++.txt',
        is_sub=True,
        Filter=[1, 1.5, 1.5],
        match_key=2,
        compare_key=2,
        parms=0.9
    )
    """
    for match_key in range(2):
        print("method:" + match_method[match_key])
        parms = 0.8
        step = 0.01
        while parms<0.95:
            print('parms = '+str(parms))
            do_check(
                code1_path='../test_code/实验4测试题/A1.c++.txt',
                code2_path='../test_code/实验4测试题/B1.c++.txt',
                is_sub=True,
                Filter=[1,1.5,1.5],
                match_key=match_key,
                compare_key=0,
                parms=parms
            )
            print("不进行变量代换的结果：")
            do_check(
                code1_path='../test_code/实验4测试题/A1.c++.txt',
                code2_path='../test_code/实验4测试题/B1.c++.txt',
                is_sub=False,
                Filter=[1, 1.5, 1.5],
                match_key=match_key,
                compare_key=0,
                parms=parms
            )
            parms+=step
            print("=================")
        print("================================================")
    """

