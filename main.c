#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>           //调用不需要按回车键的读取输入函数getch()
#include <stdbool.h>

#define SNAKESIZE 100//蛇的身体最大节数
#define MAPWIDTH 118 //宽度
#define MAPHEIGHT 29//高度

int score=0;
int succeedEat=0;
int key=72;

struct FOOD{
    int x;
    int y;
}food;

struct{
    int speed;
    int length;
    int x[SNAKESIZE];
    int y[SNAKESIZE];
}snake={200,3};




void create_Map(int width, int height);   //创建地图
void init_Snake(void);                    //初始化蛇
void create_Food(void);                   //放置食物
void keyIn(void);                         //处理键盘输入
void gotoxy(int x, int y);                //移动光标
bool check(void);                         //检查蛇的状态

int main(void)
{
    gotoxy(MAPWIDTH/2+1, MAPHEIGHT/2);
    printf("欢迎来到贪吃蛇游戏");
    gotoxy(MAPWIDTH/2+1, MAPHEIGHT/2+1);
    printf("用↑.↓.←.→分别控制蛇的移动");
    gotoxy(MAPWIDTH/2+1, MAPHEIGHT/2+2);
    printf("按任意键开始");
    gotoxy(MAPWIDTH/2+1, MAPHEIGHT/2+3);
    system("pause");
    system("CLS");

    create_Map(MAPWIDTH, MAPHEIGHT);
    init_Snake();
    create_Food();

    while (check())
    {
        if(snake.x[0] == food.x && snake.y[0] == food.y)
        {
            score+=10;
            snake.length++;
            succeedEat = 1;
            keyIn();
            create_Food();
            succeedEat = 0;
            Sleep(snake.speed);
        }
        keyIn();
        Sleep(snake.speed);
    }
    system("CLS");
    gotoxy(MAPWIDTH/2+1, MAPHEIGHT/2);
    printf("游戏结束");
    gotoxy(MAPWIDTH/2+1, MAPHEIGHT/2+1);
    printf("你的得分为:%d",score);
    Sleep(5000);
    return 0;
}

void gotoxy(int x, int y)  //用来移动光标
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void create_Map(int width, int height)
{
    int i=0;

    for(i=0;i<=MAPWIDTH;i+=2) //打印上下边框，方框占两格
    {
        gotoxy(i,0);
        printf("■");
        gotoxy(i,MAPHEIGHT);
        printf("■");
    }
    for(i=0;i<=MAPHEIGHT;i++)//打印左右边框，方框占一个竖格
    {
        gotoxy(0,i);
        printf("■");
        gotoxy(MAPWIDTH,i);
        printf("■");
    }
}
void create_Food(void)
{
    int i;
    bool flag = 0;
    while(1)
    {
        srand((unsigned int) time(NULL));
        food.x = rand() % (MAPWIDTH - 4) + 2;  //设置食物位置
        food.y = rand() % (MAPHEIGHT - 2) + 1;

        for(i=1;i<=snake.length;i+=2)          //食物位置不能在蛇身，横坐标不能为奇数
        {
            if(food.x == snake.x[i-1] && food.y == snake.y[i-1])
            {
                break;
            }
            flag=1;
        }
        if(food.x%2 == 0&& flag)
            break;
    }

    gotoxy(food.x, food.y);                //打印食物
    printf("※");
}

void init_Snake(void)
{
    int i;

    snake.x[0] = MAPWIDTH/2+1;       //将蛇头放在屏幕中央并打印，方框占两格
    snake.y[0] = MAPHEIGHT/2;
    gotoxy(snake.x[0],snake.y[0]);
    printf("*");

    for(i=1; i<=snake.length; i++)     //打印蛇身，方框占两格
    {
        snake.x[i] = snake.x[i-1] + 2;
        snake.y[i] = snake.y[i-1];
        gotoxy(snake.x[i], snake.y[i]);
        printf("□");
    }

    gotoxy(0,0);                       //光标回到原点避免在最后一节蛇身闪烁
}

void keyIn(void)
{
    int pre_key = key;                 //记录前一个按键
    int i;
    if(kbhit())                        //如果用户按下一个键
    {
         fflush(stdin);                //清理缓冲区
         key = getch();                //getch()调用功能键时第一次返回的不是实际值，第二次才是
         key = getch();
    }

    if(succeedEat == 0)               //如果没吃到就是整体往前移动，先擦掉最后一节；吃到了就不用动最后一节
    {
        gotoxy(snake.x[snake.length],snake.y[snake.length]);     //打印空格来表现最后一格被擦掉
        printf(" ");
    }

    for(i=snake.length; i>0; i--)     //处理蛇身
    {
        snake.x[i] = snake.x[i-1];       //每一节都移到前一节的位置
        snake.y[i] = snake.y[i-1];
        gotoxy(snake.x[i], snake.y[i]);  //打印蛇身
        printf("□");
    }

	if (pre_key == 72 && key == 80)  	           //蛇当前移动的方向不能和前一次的方向相反，比如蛇往左走的时候不能直接按右键往右走
        key = 72;                                 //如果当前移动方向和前一次方向相反的话，把当前移动的方向改为前一次的方向
	if (pre_key == 80 && key == 72)
		key = 80;                                 //控制台按键代表的数字：“↑”：72 “↓”：80 “←”：75 “→”：77
	if (pre_key == 75 && key == 77)
		key = 75;
	if (pre_key == 77 && key == 75)
		key = 77;

    switch(key)                    //处理蛇头
    {
        case 75:
            snake.x[0] -= 2;//往左
            break;
        case 77:
            snake.x[0] += 2;//往右
            break;
        case 72:
            snake.y[0]--;//往上
            break;
        case 80:
            snake.y[0]++;//往下
            break;
    }
    gotoxy(snake.x[0],snake.y[0]);
    printf("*");
    gotoxy(0,0);
}

bool check(void)
{
    int i = 0;
    if(snake.x[0] == MAPWIDTH || snake.x[0] == 0 || snake.y[0] == 0 || snake.y[0] == MAPWIDTH)      //蛇头碰到边框
        return false;

    for(i=1; i<=snake.length-1; i++)       //如果蛇头碰到蛇身
    {
        if(snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
            return false;
    }

    return true;
}
