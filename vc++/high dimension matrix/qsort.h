//#include"stdafx.h"
typedef unsigned int size_t;

void qsort (
			void *base,
			size_t    num,//快速排序数组个数
			size_t  width,//元素间距
			size_t  element_size,//交换的元素大小
			size_t  element_interval_size,//交换的元素的间距
			size_t  element_num,//交换的元素的个数
			int (*comp)(void *, void *, size_t&, size_t&)
			);