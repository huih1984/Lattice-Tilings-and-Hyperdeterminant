//#include"stdafx.h"
typedef unsigned int size_t;

void qsort (
			void *base,
			size_t    num,//���������������
			size_t  width,//Ԫ�ؼ��
			size_t  element_size,//������Ԫ�ش�С
			size_t  element_interval_size,//������Ԫ�صļ��
			size_t  element_num,//������Ԫ�صĸ���
			int (*comp)(void *, void *, size_t&, size_t&)
			);