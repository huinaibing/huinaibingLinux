for line in `cat input.txt`
# 把input.txt的东西全部读进tmpfile里面
# 由于cat的特性，原文件里的每个空格都变成换行了
do
    echo $line >> tmpfile
done

while read line
# 读取tmpfile
do
    if [[ $line =~ ^/(.*) ]]; then
    # 使用正则表达式提取目标字符，并存储
        echo "LFN:"$line >> output.txt
    fi
done < tmpfile

rm tmpfile