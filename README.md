# tetramer and four index system matrix
此篇文章研究内容为在一个长方形的栅格里面用1X4的小方格去覆盖，有多少种覆盖方法？<br/><br/>

1X2的情形,覆盖数的研究起始于
1961年，荷兰物理学家Pieter_Kasteleyn，和英国物理学家Temperley, H. N. V.; Fisher, Michael E.同时独立发表了Dimer覆盖的计算公式，并探讨在统计力学上的应用。他们的理论后来被统称为FKT algorithm。
[Pieter_Kasteleyn](https://en.wikipedia.org/wiki/Pieter_Kasteleyn)
有很多后续的研究，包括conway，
[Andrei_Okounkov](https://en.wikipedia.org/wiki/Andrei_Okounkov)
等一些著名数学家。
科普级别的讲一下，就是在一个固定边界的容器里面，分子做布朗运动，但是无论如何运动都相当于一个dimer覆盖，那么分子有多少的自由活动的空间，就和这个覆盖数紧密相关。Andrei_Okounkov等人在这个领域结合统计学和代数几何等理论做了研究，并因此获得了菲尔兹奖。 <br/><br/>
 ![image](https://github.com/huih1984/tetramer-and-four-index-matrix/blob/master/dimer.png)
 <br/><br/>
 
这篇文章就是试图通过类似的方法解决1X4的计算问题，并且非常有趣的地方还在于对矩阵概念做了延伸。在计数的表示上得到了一个比较完美的结果。<br/><br/>
但虽然如此，就结果而言，目前的文章还是处在一个比较初级的阶段，还需要有更进一步的研究。
