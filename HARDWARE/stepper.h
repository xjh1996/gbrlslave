#ifndef __STEPPER_H
#define __STEPPER_H

#include "include.h"

//¶Ë¿Ú¶¨Òå
#define DIR0 	PAout(2)
#define DIR1 	PAout(5)
#define STEP0 	PAout(4)
#define STEP1 	PAout(3)
#define LASER 	PAout(6)

void stepperInit(void);
void Yzheng(void);
void Yni(void);
void Xzheng(void);
void Xni(void);


#endif
