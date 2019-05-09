#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
處理時間 exp(1)
封包出生時間exp(lambda)
Ti為出生到處理好的時間=處理時間+Queue時間
Ni為第I個出生的封包處理好後,有多少個封包在Queue內
finish
*/

double gen_exp(double lambda)
{
    double cons=(-1)/lambda;
    double y = (double) rand() / (RAND_MAX + 1.0);//為了產生0~1的亂數
    double a=cons*(log(1-y));

    return a;
}
int sim(double lambda)
{
    FILE *fp;//將檔案輸出貼到EXCEL比較好檢視
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
        無法到達10萬的原因與解法
        https://www.ptt.cc/bbs/b02902HW/M.1381328026.A.2B7.html
        */

        double N=0.0, T=0.0;
        static double	interval[100000];
        memset(interval, 0, p);
        static double	service[100000];
        memset(service, 0, p);
        int i,j;

        interval[0]=0;
        for(i=1; i<p; i++)//封包出生間隔  第一個在0秒時出生
            interval[i]=gen_exp((double)lambda);


        for(i=0; i<p; i++)//封包處理時間
            service[i]=gen_exp((double)1);


//算出每個封包的生產時間   前一個抵達(出生)時間+此封包產生的時間
        static double reach_time[100000];
        memset(reach_time, 0, p);
        reach_time[0]=0.0;
//算出每個Queue的時間  =前一個處理好的時間-自己抵達時間
        static double queue_time[100000];
        memset(queue_time, 0, p);
        queue_time[0]=0;
//算出被處理完的時間   =前一個抵達的時間+前一個Queue的時間+前一個處理好的時間+自己處理完的時間
        static double deal_time[100000];
        memset(deal_time, 0, p);
        deal_time[0]=service[0];
//(Ti)=自己處理完的時間-自己到達時間
        static double deal_during[100000];
        memset(deal_during, 0, p);
        deal_during[0]=deal_time[0]-reach_time[0];

//(Ni)算出等待封包數 = i後抵達的時間 ~ i處理好的時間 ~
        static int number_queue[100000];
        memset(number_queue, 0, p);
        number_queue[0]=0;
       // fprintf(fp,"during %lf\n",deal_during[0]);
       // fprintf(fp,"0reach=%lf,need deal=%lf,beforeborn=%lf,selfQueue=%lf,selfDown=%lf,during=%lf\n\n",reach_time[0],service[0],interval[0],queue_time[0],service[0],deal_during[0]);
        for(i=1; i<p; i++)
        {
            reach_time[i]=reach_time[i-1]+interval[i];

            queue_time[i]=deal_time[i-1]-reach_time[i];//差分
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
        fprintf(fp,"%d,與前一個出生差時,抵達時間,自己在queue耗時,需要耗時,抵達到完成耗時,完成時間,,queue內有\n");
        for(i=0; i<p; i++)//計算每個時間內有幾個被Queue
        {
            fprintf(fp,"%d,%lf,%lf,%lf,%lf,%lf,%lf     "/*"%d,與前一個出生差時=%lf,抵達時間=%lf,自己在queue耗時=%lf,需要耗時=%lf,完成時間=%lf,抵達到完成耗時=%lf     "*/,
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
            fprintf(fp,",總時間 T=%lf,總queue數 N=%d,此時被Queue數=%d\n",Tcounter,Ncounter,sum_package);
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
