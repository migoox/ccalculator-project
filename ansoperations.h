#ifndef ANSOPERATIONS_H_
#define ANSOPERATIONS_H_

void ANS_sum_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container);

void ANS_sum(ANS_Num* num1, ANS_Num* num2);

void ANS_mult_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container);

void ANS_pow_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container);

void ANS_sub(ANS_Num* num1, ANS_Num* num2);

void ANS_vdivide_withc(ANS_Num* num1, unsigned long long value, ANS_Num* container);

void ANS_divide_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container);

void ANS_increment(ANS_Num* num);

#endif
