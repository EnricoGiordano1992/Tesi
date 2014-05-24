#include "stdint.h"


#include "Includes.h"


void blink(int j)
{
    int i;
    int k;
    for(k = 0; k < j; k++)
    {
        RC1 = 1;
        for(i = 0; i < 5000; i ++)
            ;
        for(i = 0; i < 5000; i ++)
            ;

        RC1 = 0;

        for(i = 0; i < 5000; i ++)
            ;
        for(i = 0; i < 5000; i ++)
            ;

    }

}

