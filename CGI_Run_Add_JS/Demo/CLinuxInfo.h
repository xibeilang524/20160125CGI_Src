#ifndef CLINUXINFO_H
#define CLINUXINFO_H
/***************************************************************
*    @file:        statusinfo.c
*
*    @brief:        从linux系统获取cpu及内存使用情况
*
*    @version    1.0
*
***************************************************************/
typedef struct CPU_PACKED         //定义一个cpu occupy的结构体
 {
 char name[20];      //定义一个char类型的数组名name有20个元素
 unsigned int user; //定义一个无符号的int类型的user
 unsigned int nice; //定义一个无符号的int类型的nice
 unsigned int system;//定义一个无符号的int类型的system
 unsigned int idle; //定义一个无符号的int类型的idle
 }CPU_OCCUPY;

typedef struct MEM_PACKED         //定义一个mem occupy的结构体
 {
 char name[20];      //定义一个char类型的数组名name有20个元素
 unsigned long total;
 char name2[20];
 unsigned long free;
 }MEM_OCCUPY;
class CLinuxInfo
{
public:

    CLinuxInfo();
    static void get_memoccupy (MEM_OCCUPY *mem);
    static int cal_cpuoccupy(CPU_OCCUPY *o, CPU_OCCUPY *n);
    static int get_cpuoccupy(CPU_OCCUPY *cpust);
};

#endif // CLINUXINFO_H
