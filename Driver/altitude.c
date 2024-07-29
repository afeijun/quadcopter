#include "altitude.h"
#include "structconfig.h"
#include "math.h"
#include "spl06.h"

float height;
nav_t nav;		    //NED frame in earth


void altitude_get(void)
{
	float __height;
//		altitude = ( 101400 - press ) / 1000.0f;
//    height = 0.82f * altitude * altitude * altitude + 0.09f * ( 101400 - press ) * 100.0f ;
	
	__height = 44330.f * (powf((1015.7f / (press/100.0f)), 0.190295f) - 1.0f);
	height = (int32_t)(__height*100);

//	printf("height = %d\r\n", (int32_t)(height*100));

//    alt_high = ( height - baro_Offset ) ; //cm +
}

