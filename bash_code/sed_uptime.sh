uptime |
sed -e "s/,/\n/g" | # 使用sed将逗号替换为换行， 
sed -e "/up/d" | # 删除含有up词汇的行
sed -e "/users/d" | # 删除含有users词汇的行
sed -e "/min/d" > tmpfile 
# 删除含有min词汇的行并把结果放进tmpfile

flag=0

for line in `cat tmpfile`
# 读取tmpfile中的内容并打印
do
    # 在指定的位置需要加上逗号，使用case和flag去筛选位置
    echo -n $line
    case "$flag" in
        "0"|"1"|"4") 
            echo -n " " 
            flag=$(($flag+1))
            continue
        ;;
        *) 
            echo -n ", "
            flag=$(($flag+1))
        ;;
    esac
done
echo
rm tmpfile