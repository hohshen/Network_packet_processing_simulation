#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
�B�z�ɶ� exp(1)
�ʥ]�X�ͮɶ�exp(lambda)
Ti���X�ͨ�B�z�n���ɶ�=�B�z�ɶ�+Queue�ɶ�
Ni����I�ӥX�ͪ��ʥ]�B�z�n��,���h�֭ӫʥ]�bQueue��
finish
*/

double gen_exp(double lambda)
{
    double cons=(-1)/lambda;
    double y = (double) rand() / (RAND_MAX + 1.0);//���F����0~1���ü�
    double a=cons*(log(1-y));

    return a;
}
int sim(double lambda)
{
    FILE *fp;//�N�ɮ׿�X�K��EXCEL����n�˵�
    fp = fopen("result.txt","w");
    if ( NULL == fp)
    {
        printf( "Open failure" );
        // return 1;
    }
    else
    {
        int p=100000;
        /*
        �L�k��F10�U����]�P�Ѫk
        https://www.ptt.cc/bbs/b02902HW/M.1381328026.A.2B7.html
        */

        double N=0.0, T=0.0;
        static double	interval[100000];
        memset(interval, 0, p);
        static double	service[100000];
        memset(service, 0, p);
        int i,j;

        interval[0]=0;
        for(i=1; i<p; i++)//�ʥ]�X�Ͷ��j  �Ĥ@�Ӧb0��ɥX��
            interval[i]=gen_exp((double)lambda);


        for(i=0; i<p; i++)//�ʥ]�B�z�ɶ�
            service[i]=gen_exp((double)1);


//��X�C�ӫʥ]���Ͳ��ɶ�   �e�@�ө�F(�X��)�ɶ�+���ʥ]���ͪ��ɶ�
        static double reach_time[100000];
        memset(reach_time, 0, p);
        reach_time[0]=0.0;
//��X�C��Queue���ɶ�  =�e�@�ӳB�z�n���ɶ�-�ۤv��F�ɶ�
        static double queue_time[100000];
        memset(queue_time, 0, p);
        queue_time[0]=0;
//��X�Q�B�z�����ɶ�   =�e�@�ө�F���ɶ�+�e�@��Queue���ɶ�+�e�@�ӳB�z�n���ɶ�+�ۤv�B�z�����ɶ�
        static double deal_time[100000];
        memset(deal_time, 0, p);
        deal_time[0]=service[0];
//(Ti)=�ۤv�B�z�����ɶ�-�ۤv��F�ɶ�
        static double deal_during[100000];
        memset(deal_during, 0, p);
        deal_during[0]=deal_time[0]-reach_time[0];

//(Ni)��X���ݫʥ]�� = i���F���ɶ� ~ i�B�z�n���ɶ� ~
        static int number_queue[100000];
        memset(number_queue, 0, p);
        number_queue[0]=0;
       // fprintf(fp,"during %lf\n",deal_during[0]);
       // fprintf(fp,"0reach=%lf,need deal=%lf,beforeborn=%lf,selfQueue=%lf,selfDown=%lf,during=%lf\n\n",reach_time[0],service[0],interval[0],queue_time[0],service[0],deal_during[0]);
        for(i=1; i<p; i++)
        {
            reach_time[i]=reach_time[i-1]+interval[i];

            queue_time[i]=deal_time[i-1]-reach_time[i];//�t��
            if(queue_time[i]<0)
            {
                queue_time[i]=0;
                //deal_time[i]=reach_time[i]+service[i];
            }
            deal_time[i]=reach_time[i]+queue_time[i]+service[i];
            //deal_time[i]=deal_time[i-1]+queue_time[i-1]+service[i];
            deal_during[i]=deal_time[i]-reach_time[i];



        }
        int Ncounter=0;
        double Tcounter=0;
        fprintf(fp,"%d,�P�e�@�ӥX�ͮt��,��F�ɶ�,�ۤv�bqueue�Ӯ�,�ݭn�Ӯ�,��F�짹���Ӯ�,�����ɶ�,,queue����\n");
        for(i=0; i<p; i++)//�p��C�Ӯɶ������X�ӳQQueue
        {
            fprintf(fp,"%d,%lf,%lf,%lf,%lf,%lf,%lf     "/*"%d,�P�e�@�ӥX�ͮt��=%lf,��F�ɶ�=%lf,�ۤv�bqueue�Ӯ�=%lf,�ݭn�Ӯ�=%lf,�����ɶ�=%lf,��F�짹���Ӯ�=%lf     "*/,
                                                            i,interval[i],         reach_time[i],queue_time[i],      service[i],   deal_during[i], deal_time[i] );

            int sum_package=0;
            fprintf(fp,",,{");
            for(j=i+1; j<p; j++)
            {
                if(deal_time[i]>reach_time[j])
                {
                    sum_package++;
                    fprintf(fp,"%d_",j);
                }
            }
            fprintf(fp,"}");
            number_queue[i]=sum_package;
            //printf("\nnumber of queue=%d\n",number_queue[i]);

            Ncounter+=number_queue[i];
            Tcounter+=deal_during[i];
            fprintf(fp,",�`�ɶ� T=%lf,�`queue�� N=%d,���ɳQQueue��=%d\n",Tcounter,Ncounter,sum_package);
            //printf("current Total N=%lf+ num=%d\n\n",N,number_queue[i]);
        }
        T=Tcounter/p;
        N=(double)Ncounter/p;
        fprintf(fp,"N= %lf, T= %lf\n", N, T);
        printf("N= %lf, T= %lf\n", N, T);
    }
    fclose(fp);
    return 0;
}


int main()
{
    double lambda;

    printf("Input lambda: ");
    scanf("%lf", &lambda);
    sim(lambda);

    return 0;
}
