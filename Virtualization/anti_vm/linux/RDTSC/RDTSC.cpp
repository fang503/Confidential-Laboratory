#include <stdio.h>
#include <unistd.h>// �����unixϵͳ����װ����ϵͳ���ýӿ� 

#define TRUE 1
#define FALSE 0
//������������,ִ������RDTSC������ret2-ret�����ڼ��CPU ʱ����������
//����Ϊ�ο�ֵ
static inline unsigned long long rdtsc_diff() {
    unsigned long long ret, ret2;
    unsigned eax, edx;
    __asm__ volatile("rdtsc" : "=a" (eax), "=d" (edx));//gcc���ָ�� 
    ret  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
    __asm__ volatile("rdtsc" : "=a" (eax), "=d" (edx));
    ret2  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
    printf("(%llu - %llu) ", ret, ret2);
    return ret2 - ret;
}
//ִ��ʱ�� 
static inline unsigned long long rdtsc_diff_vmexit() {
    unsigned long long ret, ret2;
    unsigned eax, edx;
    __asm__ volatile("rdtsc" : "=a" (eax), "=d" (edx));
    ret  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
    /* vm exit forced here. it uses: eax = 0; cpuid; */
    __asm__ volatile("cpuid" : /* no output */ : "a"(0x00));
    /**/
    __asm__ volatile("rdtsc" : "=a" (eax), "=d" (edx));
    ret2  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
    printf("(%llu - %llu) ", ret, ret2);
    return ret2 - ret;
}
//�����ظ�10�Σ�ȡƽ������1000Ϊ�ֽ��� 
int cpu_rdtsc() {
    int i;
    unsigned long long avg = 0, sub;
    for (i = 0; i < 10; i++) {
        sub = rdtsc_diff();
        avg = avg + sub;
        printf("rdtsc difference: %d\n", sub);
        usleep(500);
    }
    avg = avg / 10;
    printf("difference average is: %d\n", avg);
    return (avg < 1000 && avg > 0) ? FALSE : TRUE;
}

int cpu_rdtsc_force_vmexit() {
    int i;
    unsigned long long avg = 0, sub;
    for (i = 0; i < 10; i++) {
        sub = rdtsc_diff_vmexit();
        avg = avg + sub;
        printf("rdtsc_force_vmexit difference: %d\n", sub);
        usleep(500);
    }   
    avg = avg / 10; 
    printf("difference average is: %d\n", avg);
    return (avg < 1000 && avg > 0) ? FALSE : TRUE;
}

int main()
{
    printf("cpu_rdtsc: %d\n", cpu_rdtsc());
    printf("cpu_rdtsc_force_vmexit: %d\n", cpu_rdtsc_force_vmexit());

    return 0;
}
