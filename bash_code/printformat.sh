#! /bin/bash
# 由于使用了字符串切片操作，这个玩意是bash才有的
# 所以这里要改成使用bash

if [ $# -lt 2 ]; then
# 若传入的参数不足两个则退出
    echo "insufficient args"
    exit 1
fi


convert2oct()
# 转换数为八进制数
# 实测sh的内部函数无法读取传给文件的位置变量，所以还需要给函数传参数，
# 这个传递方法和位置变量是一样的
{
    echo "convert to oct, origin num $1"

    # 这里初始化一个空串，以方便后续的拼接，并且用一个tmp拷贝$1的值
    str=""
    tmp=$1

    while [ $tmp -ne 0 ]
    # 这个求八进制数的算法大家都知道，不赘述了
    do
        str="$(($tmp % 8))"$str
        tmp=$(($tmp / 8))
    done
    echo "the num cvt2 oct is $str"
}

convert2hex()
# 转换为16进制数
# 由于16进制中有ABCDEF，所以加了个CASE，其他地方和八进制没区别
{
    echo "convert to hex, origin num is $1"
    str=""
    tmp=$1
    while [ $tmp -ne 0 ]
    do
        rest_num="$(($tmp % 16))"
        case $rest_num in 
            "10") str="A"$str
            ;;
            "11") str="B"$str
            ;;
            "12") str="C"$str
            ;;
            "13") str="D"$str
            ;;
            "14") str="E"$str
            ;;
            "15") str="F"$str
            ;;
            *) str=$rest_num$str
        esac
        tmp=$(($tmp / 16))
    done
    echo "the num cvt2 hex is $str"
}

convert2sci()
# 转换为科学计数法
# 这里要字符串切片，所以要用bash
{
    echo "convert2sci, origin number is $1"

    tmp=$1
    strlen=0
    while [ $tmp -ne 0 ]
    # 这个while用于计算字符串的长度
    do
        strlen=$(($strlen + 1))
        tmp=$(($tmp / 10))
    done

    if [ $strlen -eq 1 ]; then
    # 如果字符串的长度为1，那就没有后面的事情
        echo "the num cvrt2 sci is $1"
        return 0
    fi

    str=""

    tmp=$1

    exact_num=2
    # 这个是精确到小数点后几位

    for i in `seq $exact_num`
    # 使用切片去取小数
    do
        if [ $i -eq $strlen ]; then
        # 如果字符串不够长就跳出循环
            break
        fi
        str=$str"${tmp:$i:1}"
    done

    tmp=$1
    echo -n "the num cvrt2 sci is " 
    echo -n ${tmp:0:1}
    echo -n ".$str X 10^" 
    echo $(($strlen-1))

}

case $1 in
    -o) # 转换8进制
        convert2oct $2
    ;;
    -x) # 转换16进制
        convert2hex $2
    ;;
    -e) # 科学技术法
        convert2sci $2
    ;;
    *) 
        echo  "error"
        exit 2
esac