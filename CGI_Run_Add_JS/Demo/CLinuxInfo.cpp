#include "CLinuxInfo.h"
#include <stdio.h>

CLinuxInfo::CLinuxInfo()
{
}
/** *
 * void CLinuxInfo::get_memoccupy(MEM_OCCUPY *mem)=SwapCached:            0 kB

int CLinuxInfo::get_cpuoccupy (CPU_OCCUPY *cpust) cpu  7139 0 3678 79101 3 0 46196 0 0 0

int CLinuxInfo::get_cpuoccupy (CPU_OCCUPY *cpust) cpu  7139 0 3678 79101 3 0 46196 0 0 0

cpu: 0
 *
 *
 * */
void CLinuxInfo::get_memoccupy(MEM_OCCUPY *mem)
{
    FILE *fd;
//    int n;
    char buff[256];
    MEM_OCCUPY *m;
    m=mem;

    fd = fopen ("/proc/meminfo", "r");

    fgets (buff, sizeof(buff), fd);
    fgets (buff, sizeof(buff), fd);
    fgets (buff, sizeof(buff), fd);
    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %lu %s", m->name, &m->total, m->name2);

    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    sscanf (buff, "%s %lu %s", m->name2, &m->free, m->name2);
    printf("void CLinuxInfo::get_memoccupy(MEM_OCCUPY *mem)=%s\n",buff);

    fclose(fd);     //关闭文件fd
}
int CLinuxInfo::cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    unsigned long id, sd;
    int cpu_use = 0;

    od = (unsigned long) (o->user + o->nice + o->system +o->idle);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long) (n->user + n->nice + n->system +n->idle);//第二次(用户+优先级+系统+空闲)的时间再赋给od

    id = (unsigned long) (n->user - o->user);    //用户第一次和第二次的时间之差再赋给id
    sd = (unsigned long) (n->system - o->system);//系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
    cpu_use = (int)((sd+id)*10000)/(nd-od); //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else cpu_use = 0;
    printf("cpu: %u\n",cpu_use);
    return cpu_use;
}
int CLinuxInfo::get_cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
//    int n;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;

    fd = fopen ("/proc/stat", "r");
    fgets (buff, sizeof(buff), fd);

    sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);
    printf("int CLinuxInfo::get_cpuoccupy (CPU_OCCUPY *cpust) %s\n",buff);
    fclose(fd);
    return 0;
}
