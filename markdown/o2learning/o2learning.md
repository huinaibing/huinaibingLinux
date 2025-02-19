## cmake

> ![image-20250122134455783](./img/image-20250122134455783.png)
>
> ${PROJECT_NAME}是一个宏
>
> ![image-20250122134612741](./img/image-20250122134612741.png)

> #### In-Place Build
>
> In-place builds generate all temporary build files in the same directory structure as the source code. This means that all Makefiles and object files are interspersed with your normal code. To create an in-place build target run the cmake command in your root directory. 
>
> #### Out-of-Source Build
>
> Out-of-source builds allow you to create a single build folder that can be anywhere on your file system. All temporary build and object files are located in this directory keeping your source tree clean. To create an out-of-source build run the cmake command in the build folder and point it to the directory with your root CMakeLists.txt file. Using out-of-source builds if you want to recreate your cmake environment from scratch, you only need to delete your build directory and then rerun cmake.
>
> 总结，其实这两都一样，你给cmake后面接上路径就行

> ![image-20250122140520585](./img/image-20250122140520585.png)
>
> ```cmake
> set(SOURCES
>  src/Hello.cpp
>  src/main.cpp
> )
> 
> add_executable(${PROJECT_NAME} ${SOURCES})
> 
> ```
> 用GLOB也行
>
> ![image-20250122141044063](./img/image-20250122141044063.png)
>
> ![image-20250122141245716](./img/image-20250122141245716.png)
>
> 这个target要改成project name

> ![image-20250122150028010](./img/image-20250122150028010.png)
>
> ![image-20250122153919616](./img/image-20250122153919616.png)

> ![image-20250122155014688](./img/image-20250122155014688.png)
>
> 大致意思：先定义一些libraries，这些包含一些CPP，然后呢把这个library的声明关联给这个library，然后生成可执行文件并把libraries链接给这个文件
>
> 像之前的代码就是：直接把所有文件变成可执行文件，然后再把headers链接给他



## c++

> ![image-20250118100606773](./img/image-20250118100606773.png)
>
> 总结：省内存

> ![image-20250118103142913](./img/image-20250118103142913.png)

> ![image-20250121131635410](./img/image-20250121131635410.png)
>
> ![image-20250121132508350](./img/image-20250121132508350.png)
>
> 这个命令的后面几个可以找到，但是为什么前面是analysistutorial-mm？，未找到

## data format

数据为AOD格式，但是存储的是root格式

The result of the asynchronous reconstruction is the Analysis Object Data (AOD) format, which is a set of tables which are stored to file (by default AO2D.root) as ROOT trees.

![image-20250117142720959](./img/image-20250117142720959.png)

![image-20250117142901886](./img/image-20250117142901886.png)

![image-20250117142923479](./img/image-20250117142923479.png)

> 总结：SQL的链接

![image-20250117143423529](./img/image-20250117143423529.png)

> 总结，不知道有啥用

对于表中的每个索引，都有一种方法可以获取相应的对象。例如，在表 aod::Tracks 中有一个索引 collisionId。因此，如果您有一个名为 track 的对象 aod::Track，则可以通过调用 track.collision() 直接访问此轨道的碰撞。您还可以在执行此操作之前检查您感兴趣的轨道是否发生碰撞，方法是使用模板函数 .has_smthg()，在此示例中使用 track.has_collision()，它返回一个布尔值。对象 track.collision() 的类型是 o2::aod::Collision。这适用于每个表中的每个索引，因此调用例如 collision.bc() 会为您提供您正在处理的碰撞的 bc。然后您可以调用例如 collision.bc().globalBC()。

> 总结：有用，但是现在暂时不明白

![image-20250117143902659](./img/image-20250117143902659.png)

> 总结：这是用一个缩写表示类型

## task基础

![image-20250117144155214](./img/image-20250117144155214.png)

> 总结：要用一种特殊格式写分析任务

![image-20250117145441888](./img/image-20250117145441888.png)

![image-20250117145500253](./img/image-20250117145500253.png)

类似于接口？

![image-20250117145541989](./img/image-20250117145541989.png)

license还要写，好专业。。

![image-20250117145620144](./img/image-20250117145620144.png)

![image-20250117145656849](./img/image-20250117145656849.png)

![image-20250117145745465](./img/image-20250117145745465.png)

上面这个图中的东西等效于cfgc（ConfigContext）

### histograms

![image-20250117150045627](./img/image-20250117150045627.png)

> 总结：这是创建一个histo，下面的histo注册可以方便地管理一批histo，但是我看不懂他再扯什么

> ![image-20250118104604335](./img/image-20250118104604335.png)
>
> ![image-20250118104614576](./img/image-20250118104614576.png)
>
> 这个语法是root的

![image-20250117150526003](./img/image-20250117150526003.png)

![image-20250117202036791](./img/image-20250117202036791.png)

> 看起来，histo的registry这里一开始是空的，后面需要再init里面add一些histo，如下

![image-20250117202314979](./img/image-20250117202314979.png)

![image-20250118110002436](./img/image-20250118110002436.png)

![image-20250117202727391](./img/image-20250117202727391.png)

> 上图为定义图的坐标的

![image-20250117202816366](./img/image-20250117202816366.png)

![image-20250117151618324](./img/image-20250117151618324.png)

> 总结：上面那个是一次性获得所有track的，下面是一个一个获得，下面的方法可以使用并行

![image-20250117152004326](./img/image-20250117152004326.png)

> 总结，当需要分析特定碰撞的时候使用这个
> ![alt text](./img/image1.png)

![image-20250117203116456](./img/image-20250117203116456.png)

![image-20250118112410865](./img/image-20250118112410865.png)

> 定义一个config
>
> ![image-20250117203150600](./img/image-20250117203150600.png)
> 这样的好处是可以在json里面修改这个变量
> ![alt text](./img/image.png)

> ![image-20250118102004753](./img/image-20250118102004753.png)
>
> 下面的代码写露了init
>
> ![image-20250118102039478](./img/image-20250118102039478.png)

### 分区/过滤

![image-20250117152853884](./img/image-20250117152853884.png)

> 总结：按照数据大小分区数据

![image-20250117153059829](./img/image-20250117153059829.png)

> 总结：过滤数据，注意：process里面的类型不一样了

![image-20250117153648346](./img/image-20250117153648346.png)

> 总结：含有config的filter

![image-20250117203627084](./img/image-20250117203627084.png)

> 链接两个表
>
> ![image-20250118095913817](./img/image-20250118095913817.png)
>
> ![image-20250118095958589](./img/image-20250118095958589.png)
>
> 仅两个表的大小相同才可以链接

> ![image-20250118112623448](./img/image-20250118112623448.png)
>
> 注意filter里面的类型为Tracks，一定要记得process里面要写Filtered

> ![image-20250118114838759](./img/image-20250118114838759.png)
>
> 需要一个cache来帮助slice，还需要传入collision的globalIndex（问题，如果我process无需collision的话该如何分片）
>
> （猜想：若没有指定collision的需求，那么可以直接用for去读取leftTracks，若有指定collision的需求则需要去用slice）

![image-20250118115230848](./img/image-20250118115230848.png)

分片加过滤的操作是一个逻辑and

> ![image-20250118115739677](./img/image-20250118115739677.png)
>
> join，这里把Tracks和TracksExtras join一起了，但是我不知道他具体指的是啥

> 筛选，注意需要一个头文件
> ![alt text](image.png)
> 运行时还要加一点东西
> ![alt text](image-1.png)

![image-20250118120148425](./img/image-20250118120148425.png)

## 附录
```
// 简单的代码（没join）
// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
/// \brief This task is an empty skeleton that fills a simple eta histogram.
///        it is meant to be a blank page for further developments.
/// \author everyone

#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"

using namespace o2;
using namespace o2::framework;

struct myExampleTask {
  // Histogram registry: an object to hold your histograms
  HistogramRegistry histos{"histos", {}, OutputObjHandlingPolicy::AnalysisObject};

  Configurable<int> nBinsEta{"nBinsEta", 30, "N bins in eta histo"};
  Configurable<int> nBinsPt{"nBinsPt", 100, "N bins in pT histo"};

  Configurable<double> axisEtaStart{"axisEtaStart", -1.5, "axisEtaStart"};
  Configurable<double> axisEtaEnd{"axisEtaEnd", +1.5, "axisEtaend"};

  Configurable<double> axisPtStart{"axisPtStart", 0.0, "axisEtaStart"};
  Configurable<double> axisPtEnd{"axisPtEnd", +10.0, "axisEtaend"};

  void init(InitContext const&)
  {
    // define axes you want to use
    const AxisSpec axisEta{nBinsEta, axisEtaStart, axisEtaEnd, "#eta"};
    const AxisSpec myaxisPt{nBinsPt, axisPtStart, axisPtEnd, "P_{t}"};

    // create histograms
    histos.add("etaHistogram", "etaHistogram", kTH1F, {axisEta});
    histos.add("ptHistogram", "ptHistogram", kTH1F, {myaxisPt});
  }

  void process(aod::TracksIU const& tracks)
  {
    for (auto& track : tracks) {
      histos.fill(HIST("etaHistogram"), track.eta());
      histos.fill(HIST("ptHistogram"), track.pt());
    }
  }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<myExampleTask>(cfgc)};
}

```