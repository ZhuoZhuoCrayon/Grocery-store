import re
from codeFormat import remove_comment as rc

# 基本关键字
varTypes = ['void','bool','char','int','float','double',
            'long','long long','class','struct',
            'set','queue','stack','map','iterator',
            'list','deque','vector','multiset',
            'multimap','string']

# 变量位置
variable_pattern = r'[\W]{}[\W]'
# 变量声明位置
findVar_pattern = r'{}([\w\t ,\*]+)'


#define var value
define_pattern = re.compile(r'#define(.+)')
#typedef value var
typedef_pattern = re.compile(r'#typedef(.+)')


default_path = '../test_code/code_1.cpp'

# 预定义变量代换
def sub_predef(code:str):
    # 获得define或typedef 替换的关键字
    res_1 = define_pattern.findall(code)
    res_2 = typedef_pattern.findall(code)

    # (var,value)
    predef_list = []

    #将关键字和自定义关键字/值找出
    for seg in res_1:
        # print(seg)
        vars = re.split(r'[\s]+',seg)
        try:
            vars.remove('') #去除空值
            predef_list.append((vars[0],vars[1]))
        except:pass

    for seg in res_2:
        vars = re.split(r'[\s]+',seg)
        vars.remove('')
        predef_list.append((vars[1], vars[0]))

    # 将代码中的自定义关键字/值 替换成标准的关键字或数值
    for var,value in predef_list:
        # 变量名替换
        re_var = re.compile(variable_pattern.format(var))
        """
            不直接使用replace的原因：
            如果自定义关键字是 a，直接替换会导致a为首的其他变量也被替换
        """
        for p in re_var.findall(code):
            new = p.replace(var,value, 1)
            code = code.replace(p, new, 1)

    #删除define和typedef字段
    code = define_pattern.sub('',code)
    code = typedef_pattern.sub('',code)
    return code

# 统计变量
def findall_variable(code:str):
    # 用于存变量
    variable_set = set()

    # 在代码中查找每个数据类型的变量
    for type in varTypes:
        #正则表达式代入关键字
        re_p = re.compile(findVar_pattern.format(type))
        segs = re_p.findall(code)

        # 变量组进一步拆分
        for seg in segs:
            var_type = type
            # 按非字母数字下划线进行划分变量
            vars = re.split('[,\s]',seg)
            try:vars.remove('')
            except:pass
            # print(vars)
            for idx in range(len(vars)):
                var = vars[idx]
                if len(var)==0: # 跳过空字符串
                    pass
                elif var in varTypes:   #var是关键字
                    var_type = var  #下一遍历到的遍历是该关键字所属的数据类型
                else:
                    if var[0] == '*':
                        var_type = '*'+var_type
                    if len(var)>1 and var[1] == '*':
                        var_type = '*'+var_type
                    var = var.replace('*','')
                    variable_set.add(var+":"+var_type)
    # print(variable_set)
    var_list = []
    for var in variable_set:
        var,type = var.split(':')
        if var == '*' or var == '':continue
        # 清除变量前的指针标记
        del_pointerVal = var.replace('*','')
        # print(del_pointerVal)
        # 统计变量出现次数
        re_var = re.compile(variable_pattern.format(del_pointerVal))
        var_cnt = len(re_var.findall(code))
        if var_cnt != 0:
            var_list.append((var_cnt,type,var))
    var_list.sort()  #按出现变量次数进行排序
    return var_list


# 集成上述函数，实现去除注释，变量统计分析
def analysis_var(code_path = default_path):
    code = rc.remove_comment(code_path)
    code = sub_predef(code)
    var_list = findall_variable(code)
    # 去主函数名
    try:var_list.remove((1,'int','main'))
    except:pass
    return code,var_list


import difflib

#  变量相似度计算
def string_similar(var1:str,var2:str):
    var1 = var1.lower()
    var2 = var2.lower()
    return difflib.SequenceMatcher(None,var1,var2).quick_ratio()

# 数量相似度
def num_similar(num1:int,num2:int):
    rate = 1 - (abs(num1-num2)/max(num1,num2))
    return rate


#----------------public----------------------------

# filter //  weight rate of num,type,var
# 变量代换主函数
def sub_var_main(code_path_1=default_path,code_path_2=default_path,filter = [1,1.5,1.5]):
    code1,var_in_code1 = analysis_var(code_path=code_path_1)
    code2,var_in_code2 = analysis_var(code_path=code_path_2)

    """
    if len(var_in_code1) < len(var_in_code2):
        var_in_code2,var_in_code1 = var_in_code1,var_in_code2
    """

    len1 = len(var_in_code1)
    len2 = len(var_in_code2)
    similar_matrix = [[i for i in range(len2)] for j in range(len1)]
    for i in range(len1):
        for j in range(len2):
            var1 = var_in_code1[i]
            var2 = var_in_code2[j]
            num_rate = num_similar(var1[0],var2[0])*filter[0]
            type_rate = int(var1[1]==var2[1])*filter[1]
            var_rate = string_similar(var1[2],var2[2])*filter[2]
            # 对三个条件进行加权求值
            similar_matrix[i][j] = num_rate + type_rate + var_rate

    #对代码一和代码二的变量进行匹配，将代码一的变量替换成最近似的代码二变量
    for i in range(len(similar_matrix)):
        closed = max(similar_matrix[i])
        idx = similar_matrix[i].index(closed)
        # print(var_in_code1[i][2],var_in_code2[idx][2])

        re_var = re.compile(variable_pattern.format(var_in_code1[i][2]))
        """
            不直接使用replace的原因：
            如果自定义关键字是 a，直接替换会导致a为首的其他变量也被替换
        """
        for p in re_var.findall(code1):
            new = p.replace(var_in_code1[i][2],var_in_code2[idx][2])
            # print(new)
            code1 = code1.replace(p, new, 1)
    code1 = rc.remove_bracket(code1)
    code2 = rc.remove_bracket(code2)
    return code1,code2

#-----------------Main-------------------------------------------


def format_codeMain(code_path_1=default_path,code_path_2=default_path,subVar=True,filter = [1,1.5,1.5]):
    if subVar:
        code1,code2 = sub_var_main(code_path_1,code_path_2,filter=filter)
    else:
        code1,code2=rc.format_code_main(code_path_1),rc.format_code_main(code_path_2)
    return code1,code2

"""
path1 = '../test_code/实验4测试题/A1.c++.txt'
path2 = '../test_code/实验4测试题/B1.c++.txt'
# sub_var_main(code_path_1=path1,code_path_2=path2)

sub_var_main()
"""
