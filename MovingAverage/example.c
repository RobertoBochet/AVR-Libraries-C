#include "MovingAverage.h"

int main()
{
	float smaArray[5], wmaArray[5];
	SMA sma;
	WMA wma;
	float a = 0;
	
	SMAInit(&sma, smaArray, 5);
	
	SMAAdd(&sma, 1);
	a = SMAGet(&sma);//1
	SMAAdd(&sma, 2);
	a = SMAGet(&sma);//1.5
	SMAAdd(&sma, 3);
	SMAAdd(&sma, 4);
	SMAAdd(&sma, 5);
	a = SMAGet(&sma);//3
	SMAAdd(&sma, 6);
	a = SMAGet(&sma);//4
	
	WMAInit(&wma, wmaArray, 5);
	
	WMAAdd(&wma, 1);
	a = WMAGet(&wma);//1
	WMAAdd(&wma, 2);
	a = WMAGet(&wma);//1.67
	WMAAdd(&wma, 3);
	WMAAdd(&wma, 4);
	WMAAdd(&wma, 5);
	a = WMAGet(&wma);//3.67
	WMAAdd(&wma, 6);
	a = WMAGet(&wma);//4.67
}