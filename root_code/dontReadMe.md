写这个readme.md完全是因为git不识别空文件夹，但是现在我要把它变成root笔记了

> 本root代码只供正直诚实的人使用

# root笔记（新）
> 旧的去看我其他仓库,当然那些仓库都是private的你看不到哈哈
> 
> [root的document官网，虽然看不懂](https://root.cern/doc/v632/)
> 
> [震惊！ 某人竟用短短一行代码将复杂的root代码简化得如此简单， 实现方法竟是这些](./XQY_Utils.h)

## 简单的脚本 对应root教程13
[draw.c 包括画布、图片函数](./simple_script/draw.c)

[draw_1.c 包括线的样子的设置 marker的相关设置 各种标题](./simple_script/draw_1.c)

[draw_2.c TLatex](./simple_script/draw_2.c)

[draw_3.c 画线 保存成文件](./simple_script/draw_3.c)

## 产生随机数 对应教程14
[random_test.c 产生简单的随机数， 种子问题](./rootTutorial14/random_test.c)

[uniform_random.c 产生随机数并且填涂， 有多种分布](./rootTutorial14/uniform_random.c)

[random_seed_legend.c 用带seed的产生器 并且使用legend区分不同图线，将多个图画在一起](./rootTutorial14/random_seed_legend.c)

[diy_function 自定义一个分布函数并且抽样](./rootTutorial14/diy_function.c)

[fit.md 使用鼠标操作去拟合函数](./rootTutorial14/fit.md)

[simple_fit.c 使用代码拟合函数](./rootTutorial14/simple_fit.c)

[gauss_2d.c 二维图形 以及二维图形的拟合](./rootTutorial14/gauss_2d.c)

[uniform_eff.c 效率图 涉及文件的IO](./rootTutorial14/uniform_eff.c)
> 25 03 02 由于不明白效率图意义，上面的部分未完善

## TTree 对应教程15

[简单的TTREE 注意，cmake的时候要手动加上root的library](./RootTutorial15/simple_TTree/CMakeLists.txt)

[makeClass](./RootTutorial15/makeClass/makeClass.md)

