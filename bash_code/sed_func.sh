sed_files()
{
    flag=0
    for i in $@
    do
        # 如果遇到了传入参数的第一个，这个是匹配串，
        # 显然要跳过它
        if [ $flag -eq 0 ]; then
            flag=1
            continue
        fi

        # 使用nl打印出文件每一行并附带行号，
        # 使用pipe传给sed指令
        echo "searching for file $i"
        nl $i | sed -n "/$1/p"
    done
}

sed_files $@