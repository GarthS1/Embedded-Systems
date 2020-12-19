/*
 * File:   main.c
 * Author: Garth Slaney, Long Ta, Weitao Wu
 *
 * Created on October 1, 2020, 12:28 PM
 */


#include <p24F16KA101.h>
#include "xc.h"
#include "Ios.h"


// Used to call IOinit() and IOcheck()
int main(void) 
{ 
    IOinit();
  
    //Infinite while loop.
    while(1) 
    {  
        IOcheck(); 
    }
    return 0;
}
