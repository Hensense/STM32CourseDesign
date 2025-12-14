#include "button4_4.h"

struct IO_PORT
{                                            
        GPIO_TypeDef *GPIO_x;                 
        unsigned short GPIO_pin;
};
static struct IO_PORT KEY_OUT[4] = {
        {BUTTON_ROW1_GPIO_PORT, BUTTON_ROW1_GPIO_PIN},
                {BUTTON_ROW2_GPIO_PORT, BUTTON_ROW2_GPIO_PIN},
        {BUTTON_ROW3_GPIO_PORT, BUTTON_ROW3_GPIO_PIN}, 
                {BUTTON_ROW4_GPIO_PORT, BUTTON_ROW4_GPIO_PIN}
};
static struct IO_PORT KEY_IN[4] = {
        {BUTTON_COL1_GPIO_PORT, BUTTON_COL1_GPIO_PIN}, 
                {BUTTON_COL2_GPIO_PORT, BUTTON_COL2_GPIO_PIN},
        {BUTTON_COL3_GPIO_PORT, BUTTON_COL3_GPIO_PIN}, 
                {BUTTON_COL4_GPIO_PORT, BUTTON_COL4_GPIO_PIN}
};
unsigned char button_key[4][4];
unsigned char button_key_pre[4][4];
void Button4_4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;

    RCC_APB2PeriphClockCmd(BUTTON_GPIO_CLK, ENABLE);      
    for(i=0;i<4;i++)
    {
        GPIO_InitStructure.GPIO_Pin = KEY_OUT[i].GPIO_pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(KEY_OUT[i].GPIO_x, &GPIO_InitStructure);
    }
    
    for(i=0;i<4;i++)
    {
        GPIO_InitStructure.GPIO_Pin = KEY_IN[i].GPIO_pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        
        GPIO_Init(KEY_IN[i].GPIO_x, &GPIO_InitStructure);
    }
      

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(KEY_OUT[i].GPIO_x, KEY_OUT[i].GPIO_pin);
    }
}

char Button4_4_Scan(void)
{
    unsigned char i, j;
    
    for(i = 0; i < 4; i++)            
    {
        GPIO_ResetBits(KEY_OUT[i].GPIO_x, KEY_OUT[i].GPIO_pin);   
        for(j = 0; j < 4; j++)            
        {
            if((GPIO_ReadInputDataBit(KEY_IN[j].GPIO_x, KEY_IN[j].GPIO_pin) == 0) &&  button_key_pre[i][j] != 1)
            {
                button_key[i][j] = 1;
                button_key_pre[i][j] = 1;
            }else if(GPIO_ReadInputDataBit(KEY_IN[j].GPIO_x, KEY_IN[j].GPIO_pin) == 1){
                button_key[i][j] = 0;
                button_key_pre[i][j] = 0;
            }
            else{
                button_key[i][j] = 0;
            }
        }
        GPIO_SetBits(KEY_OUT[i].GPIO_x, KEY_OUT[i].GPIO_pin);
    }
    if(button_key[0][0]==1)         /* 1 */
        return '1';
    else if(button_key[0][1]==1)    /* 4 */
        return '4';
    else if(button_key[0][2]==1)    /* 7 */
        return '7';
    else if(button_key[0][3]==1)    /* * */
        return '*';
    else if(button_key[1][0]==1)    /* 2 */
        return '2';
    else if(button_key[1][1]==1)    /* 5 */
        return '5';
    else if(button_key[1][2]==1)    /* 8 */
        return '8';
    else if(button_key[1][3]==1)    /* 0 */
        return '0';
    else if(button_key[2][0]==1)    /* 3 */
        return '3';
    else if(button_key[2][1]==1)    /* 6 */
        return '6';
    else if(button_key[2][2]==1)    /* 9 */
        return '9';
    else if(button_key[2][3]==1)    /* # */
        return '#';
    else if(button_key[3][0]==1)    /* A */
        return 'A';
    else if(button_key[3][1]==1)    /* B */
        return 'B';
    else if(button_key[3][2]==1)    /* C */
        return 'C';
    else if(button_key[3][3]==1)    /* D */
        return 'D';
    else return 0;
}

