sed -n "/IntegrateBeamCrossing\/Luminosity/p" ./lumi_test/* > tmpfile
# 读取指定内容，并存入临时文件

sum=0
while read line
do
    # 第一个sed用于去除头部，第二个用于去除尾巴
    tmp=`echo $line | sed "s/<\(.*\)\">//" | sed "s/\ \(.*\)>//"`
    # 求和
    sum=`echo $tmp+$sum | bc`
done < tmpfile

echo $sum
rm tmpfile