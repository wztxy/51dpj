#include <REGX52.H>
#include "Delay.h"
#include "MatrixLED.h"
#include "Timer0Init.h"
#include "StartGame.h"
#include "WIN1.h"
#include "WIN2.h"

sbit k2=P3^0;		//按键K2
sbit k1=P3^1;    //按键K1
int ball_column=6,ball_LeftRight=-1,ball_UpDown=-1,Player1Win=0,Player2Win=0,Score=0,start_flag=0;
unsigned char ball_position=0x80;


int racket1[]={224,112,56,28,14,7},Place1=3;//用于表示球拍1位置
int racket2[]={224,112,56,28,14,7},Place2=0;//用于表示球拍2位置
//分别为1110 0000,0111 0000,0011 1000,0001 1100,0000 1110,0000 0111

void Gameinit()//游戏初始化函数
{
	Place1=3;//表示初始球拍1的位置为0001 1100
	Place2=0;//表示初始球拍2的位置为1110 0000
	ball_column=6;//ball_column=6表示球在第七列(从0算起)
	ball_position=0x80;//1000 0000 表示球所在的行数为8
	ball_LeftRight=-1;//ball_LeftRight表示球移动的左右方向，-1表示向左
	ball_UpDown=-1;//ball_UpDown表示球移动的上下方向，-1表示向下
  Player1Win=0;//Player1Win=0表示玩家1未取得胜利
	Player2Win=0;//Player2Win=0表示玩家2未取得胜利
	start_flag=0;//start_flag是游戏开始的符号，等于0说明游戏尚未开始

}

void Showpicture()
{
    unsigned char k=1;
    MatrixLED_ShowColumn(0,racket1[Place1]);
	  MatrixLED_ShowColumn(7,racket2[Place2]);
		for(k=1;k<7;k++)	
		{
			if(k!=ball_column)				
			   MatrixLED_ShowColumn(k,0x00);
			else
				MatrixLED_ShowColumn(k,ball_position);
		}
}

void Check()//判断游戏胜利或失败
{

	if(ball_column==1)//当球在第二列时
	{
	   if(ball_position==0x01)//0000 0001 如果球在第一行
		 {
			 if(Place1!=5)//球拍一位置不是0000 0111，说明玩家一没接住球
			 Player2Win=1;//玩家二胜利
		 }
		 if(ball_position==0x04)
		 {
			 if((Place1!=3)&&(Place1!=4)&&(Place1!=5))
			 Player2Win=1;
		 }
		 if(ball_position==0x10)
		 {
			 if((Place1!=1)&&(Place1!=2)&&(Place1!=3))
			 Player2Win=1;
		 }
		 if(ball_position==0x40)
		 {
			 if((Place1!=0)&&(Place1!=1))
			 Player2Win=1;
		 }
	}
	
	if(ball_column==6)
	{
	   if(ball_position==0x80)
		 {
			 if(Place2!=0)
			 Player1Win=1;
		 }
		 if(ball_position==0x20)
		 {
			 if((Place2!=0)&&(Place2!=1)&&(Place2!=2))
			 Player1Win=1;
		 }
		 if(ball_position==0x08)
		 {
			 if((Place2!=2)&&(Place2!=3)&&(Place2!=4))
			 Player1Win=1;
		 }
		 if(ball_position==0x02)
		 {
			 if((Place2!=4)&&(Place2!=5))
			 Player2Win=1;
		 }
	}	
	
	
}

void BallMove()//球移动
{
	Check();
	ball_column+=ball_LeftRight;//球左右移动
	if(ball_position==0x80)//如果球在最上面一行
	{
		ball_UpDown=-1;//球的移动方向为向下
	}
	if(ball_position==0x01)//如果球在最下面一行
	{
		ball_UpDown=1;//球的移动方向为向上
	}
	if(ball_UpDown==1)
	{
		ball_position=ball_position<<1;//球向上移动
	}
	if(ball_UpDown==-1)
	{
		ball_position=ball_position>>1;//球向下移动
	}
  if(ball_column==1)//如果球到了第二列
  {
	  ball_LeftRight=1;//球的移动方向变为向右
	}		
	if(ball_column==6)//如果球到了第七列
	{
	  ball_LeftRight=-1;//球的移动方向变为向左
	}
}

void MoveUP(int player)//球拍上移
{
	if(player==1)
	{
	Place1-=1;
	if(Place1<0)
		Place1=0;
  }
	
	if(player==2)
	{
	Place2-=1;
	if(Place2<0)
		Place2=0;
  }
	
}

void MoveDOWN(int player)//球拍下移
{
	if(player==1)
	{
	Place1+=1;
	if(Place1>5)
		Place1=5;
  }
	
	if(player==2)
	{
	Place2+=1;
	if(Place2>5)
		Place2=5;
  }	
	
}

void main()
{
	MatrixLED_Init();
  while(1)
  {	 
     while(start_flag==0)			//播放K1 TO START,如果K1按键按下，游戏开始
	   {
		    StartGame(start_flag);
		    start_flag=1;
	   }
	   Timer0Init();
	   while((Player2Win==0)&&(Player1Win==0))
	   {
		    Showpicture();
		    if(k1==0)			//如果K1按键按下
		    {
			     Delay(20);
			     while(k1==0);
			     Delay(20);
					MoveUP(1);//拍子1上移
	      }
		
		    if(k2==0)			//如果K2按键按下
		    {
			     Delay(20);
			     while(k2==0);
			     Delay(20);
           MoveDOWN(1);//拍子1下移
		    }
				
				P1=0xFF;
	      P1_1=0;
	      if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);MoveUP(2);}//按下S15，玩家2球拍上移
	
	      P1=0xFF;
	      P1_0=0;
	      if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);MoveDOWN(2);}//按下S16，玩家2球拍下移
	   }
	   if (Player2Win==1)//如果玩家2赢了
	   {
        TR0=0;//定时器暂停
			  WIN2();//播放2 WIN！
        Gameinit();//游戏初始化
	   }
	   if (Player1Win==1)//如果玩家1赢了
	   {
        TR0=0;
		    WIN1();//播放1 WIN！
        Gameinit();
	   }
   }
}

void Timer0() interrupt 1//定时器中断程序
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if((T0Count>=500)&&(k1==1)&&(k2==1))	//定时器分频，500ms
	{
		T0Count=0;
    BallMove();//每500ms球移动一次
	}
}

