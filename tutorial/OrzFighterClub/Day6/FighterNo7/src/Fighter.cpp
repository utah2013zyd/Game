#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
        static int ForbackDirection = 0;//撤离方向
        static int Forback;//剩下的在撤离状态所要走的格数
        static Ogre::Vector2 st[100];//敌人最后100个回合的位置
        static int stBottom = 1;//栈底位置,暂时不用
        static int stTop = -1;//栈顶位置
        static bool stFirstRound = true;//是否是栈的第一圈
        static Ogre::Vector2 myLastPos(-1,-1);//自己上次的位置
        static float EnemyMaxPower = 100;//敌人的最大能量
        static float myMaxPower = 100;//估计自己的最大能量，有现成的不能用，郁闷
        static int powerEqCount = 0;//检测能量值相等的次数
        static int lastPower = 0;//上次的power
        static int hMaxPower = 100;//历史记录最大的power
        static float EnemyHealth = 500;//估计的敌人的生命值
        Ogre::Vector2 DestoryingWall(-1,-1);//本回合被攻击的墙的位置，未用
        static int RoundWall[4];//所在位置四个方向上要攻击的墙的数量
        static char needCalculateRW[16][16];//是否需要计算四个方向上的墙的数量，在移动时要重新计算
        static bool WallisEmpty = false;//所有墙是否已打完
        static bool IsFirstGetMap = true;
        static int WallCount = 0;//墙初始的总数
        int WallCountNow = 0;//现有墙的总数
        int Strategy;//战略1主动进攻2被动进攻3躲避伺机
        //战术1快速撤退2慢速撤退3提前攻击迅速撤退(2+步)4提前攻击并引诱而后撤退
        static int Tactic=0;
        static bool isFirstStep = false;//战术里使用的检测是否是走第一步
        int x = getX();
        int y = getY();
        int c=0,d=0,direction=0;
        Orz::FCMap* map = Orz::FCMap::getActiveMap();//得到地图x=0-15,y=0-15
        //得到墙的总数
        if(IsFirstGetMap)
        {
                for ( int i = 0;i < 16 ;i++)
                {
                        for (int j = 0;j<16 ;j++)
                        {
                                if (map->getBlocker(i,j))WallCount++;
                        }
                }
                IsFirstGetMap = false;
                if(WallCount>0)
                        WallisEmpty=false;
                else
                        WallisEmpty=true;
                memset(needCalculateRW,1,16*16*sizeof(char));
                myLastPos.x=x==15?x-1:x+1;
                myLastPos.y=y==15?y-1:y+1;
        }
        //得到现有墙的总数
        if(!WallisEmpty)
        {
                for ( int i = 0;i < 16 ;i++)
                {
                        for (int j = 0;j<16 ;j++)
                        {
                                if (map->getBlocker(i,j))WallCountNow++;
                        }
                }
        }
        //将敌人位置压入栈

        if (stTop==99)
        {
                stTop=-1;
                stFirstRound = false;
        }
        stTop++;
        st[stTop].x=map->getFighter(!_fighterID)->getX();
        st[stTop].y=map->getFighter(!_fighterID)->getY();
        std::cout<<"hMaxPower:"<<hMaxPower<<"  _MaxPower:"<<this->_maxPower<<std::endl;

        if(hMaxPower<power)hMaxPower=power;
        if(myMaxPower<power)myMaxPower = power;

        if(lastPower == power)powerEqCount++;
        else
                powerEqCount = 0;
        lastPower=power;
        if(powerEqCount>10)
        {
                if(power>=hMaxPower-25)
                        myMaxPower = power;
                powerEqCount=0;
                lastPower=0;
                //这时候能量值power偶尔出现power+25>this->_maxPower的情况
                //那么能量永远不再增长，必须消耗一下能量使power+25<this->_maxPower
                //这样才能避免永久睡眠，对敌人的攻击也无动于衷
                if(WallisEmpty && (hMaxPower-power)<25 && (abs(x-st[stTop].x)>1 && abs(y-st[stTop].y)>1) && power>50)
                {
                        std::cout<<"避免永久睡眠，向敌人发射炮弹______________________________________-"<<std::endl;
                        if(abs(x-st[stTop].x)>abs(y-st[stTop].y))
                        {
                                if(x>st[stTop].x)
                                {
                                        return getAction(Fire,West,10);
                                }
                                else
                                {
                                        return getAction(Fire,East,10);
                                }
                        }
                        else
                        {
                                if(y>st[stTop].y)
                                {
                                        return getAction(Fire,North,10);
                                }
                                else
                                {
                                        return getAction(Fire,South,10);
                                }
                        }
                }
        }

        EnemyMaxPower = 100 + (WallCount-WallCountNow-(myMaxPower-100)/3)*3;
        float CompMaxPower = myMaxPower - EnemyMaxPower;
        //float CompHealth = _health - EnemyHealth;//血量比较由于接口限制现在不能进行
        if((CompMaxPower>25 && EnemyHealth<200))//&& CompHealth>-100 && _health>100) || CompHealth>200)
                Strategy=1;
        else
                //if(CompHealth<-200 || CompMaxPower<-25)
                if(CompMaxPower<-50)
                        Strategy=3;
                else
                        if(WallisEmpty==true)
                                Strategy=(rand()%2)?1:3;
                        else
                                Strategy=2;        
        if(!WallisEmpty)Strategy=2;        
        std::cout<<"我的战略:"<<Strategy<<"   我的战术："<<Tactic<<std::endl;
        if(Tactic>0)
        {
                //除非战术结束，否则不会计算周围墙的数量
                //检测是否能移动
                switch(ForbackDirection)
                {
                case (int)East:
                        if(x==15)Tactic = 0;
                        else
                                if(map->getBlocker(x+1,y))Tactic = 0;
                        break;
                case (int)West:
                        if(x==0)Tactic = 0;
                        else
                                if(map->getBlocker(x-1,y))Tactic = 0;
                        break;
                case (int)South:
                        if(y==0)Tactic = 0;
                        else
                                if(map->getBlocker(x,y-1))Tactic = 0;
                        break;
                case (int)North:
                        if(y==15)Tactic = 0;
                        else
                                if(map->getBlocker(x,y+1))Tactic = 0;
                        break;
                }
                if(Forback<=0)//1为走1步，0为不走
                {
                        Tactic = 0;
                }
                Forback--;

                switch(Tactic)
                {
                case 1:
                        c = power;
                        return getAction(Move,Orz::FighterBase::DIRECTOR(ForbackDirection),c);
                        break;
                case 2:
                        c = 10;
                        return getAction(Move,Orz::FighterBase::DIRECTOR(ForbackDirection),c);
                        break;
                case 3:
                        if(isFirstStep)
                        {
                                c = 10;
                                isFirstStep = false;
                        }
                        else
                                c = power;
                        return getAction(Move,Orz::FighterBase::DIRECTOR(ForbackDirection),c);
                        break;
                case 4:
                        break;
                case 5:
                        if(abs(x-st[stTop].x)>0 && abs(y-st[stTop].y)>0)
                        {
                                return getAction(Sleep);
                        }
                        else
                        {
                                Tactic = 0;
                        }
                        break;
                }
                if(Tactic == 0)needCalculateRW[x][y] = 1;
        }
        //最大能量值可能估计小了，将有机会休息
        if(WallisEmpty && (myMaxPower-100)/3<(WallCount-WallCountNow)/2 && !(rand()%10)
                && abs(x-st[stTop].x)>0 && abs(y-st[stTop].y)>0)
        {
                std::cout<<"战术5：休息试图达到能量最大值"<<std::endl;
                Tactic=5;
                Forback=(int)((((float)(WallCount-WallCountNow)*3)/2-power)/25);
                return getAction(Sleep);
        }
        std::cout<<"最大能量："<<myMaxPower<<std::endl;
        std::cout<<"  power:"<<power<<"敌我能量比较："<<CompMaxPower<<std::endl;
        std::cout<<"好兵的生命:"<<_health<<std::endl;
        if((power<(myMaxPower-25) || (myMaxPower>150 && power<=100)) && power<150 && WallisEmpty && Strategy==1)
        {
                std::cout<<"能量不够"<<std::endl;
                return getAction(Sleep);
        }
        if(myMaxPower>150 && power<70 && Strategy==2)
        {
                return getAction(Sleep);
        }

        //不在某战术范围内与敌处于一直线的处理
        if(x==st[stTop].x)
        {
                if((abs(y-st[stTop].y)<10 || power > 100) || (x==0 && map->getBlocker(x+1,y))
                        || (x==15 && map->getBlocker(x-1,y))||(x!=15 && x!=0 && map->getBlocker(x+1,y) && map->getBlocker(x-1,y)))
                {   
                        d=0;
                        if(y>=st[stTop].y)
                        {
                                for(c=y;c>st[stTop].y;c--)
                                        if(map->getBlocker(x,c))
                                        {
                                                d++;
                                                direction=int(North);//应该向北发射一发炮弹
                                        }
                        }
                        else
                        {
                                for(c=y;c<st[stTop].y;c++)
                                        if(map->getBlocker(x,c))
                                        {
                                                d++;
                                                direction=int(South);
                                        }

                        }
                        if(d>0)
                        {
                                if(abs(c-y)>6 || d>1)
                                        return getAction(Fire,Orz::FighterBase::DIRECTOR(direction),10);
                                switch(rand()%4)
                                {
                                case 0:
                                        if(y>st[stTop].y && RoundWall[South]>0)
                                                return getAction(Fire,South,10);
                                        if(y<st[stTop].y && RoundWall[North]>0)
                                                return getAction(Fire,North,10);
                                case 1:
                                        if(RoundWall[East]>0 && x<15 && map->getBlocker(x+1,y))
                                                return getAction(Fire,East,10);
                                        else
                                                if(RoundWall[West]>0)
                                                        return getAction(Fire,West,10);
                                case 2:
                                        return getAction(Sleep);
                                        break;
                                case 3:
                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        needCalculateRW[x][y] = 1;
                                        if(rand()%2)
                                                return getAction(Move,West,10);
                                        else
                                                return getAction(Move,East,10);
                                        break;
                                }
                        }
                        ForbackDirection=rand()%2?West:East;
                        Forback=1+rand()%2;
                        if(abs(y-st[stTop].y)<3)
                        {
                                if(power>200)
                                        Tactic = 0;
                                else
                                        Tactic=1+rand()%2;
                                c=100;
                        }
                        else
                        {                        
                                Tactic=1+rand()%2;
                                c = power;
                        }
                        needCalculateRW[x][y] = 1;
                        if(y>=st[stTop].y)
                        {
                                if(abs(y-st[stTop].y)<=2)EnemyHealth-=c;
                                return getAction(Fire,North,c);
                        }
                        else
                        {
                                if(abs(y-st[stTop].y)<=2)EnemyHealth-=c;
                                return getAction(Fire,South,c);
                        }
                }
                else
                {

                        myLastPos.x = x;
                        myLastPos.y = y;
                        needCalculateRW[x][y] = 1;
                        if(x<15 && map->getBlocker(x+1,y) || x==15)
                                return getAction(Move,West,power);
                        else
                                return getAction(Move,East,power);
                }
        }
        if(y==st[stTop].y)
        {
                if((abs(x-st[stTop].x)<10 || power > 100) || (y==0 && map->getBlocker(x,y+1))
                        || (y==15 && map->getBlocker(x,y-1))||(y!=15 && y!=0 && map->getBlocker(x,y+1) && map->getBlocker(x,y-1)))
                {        
                        d=0;
                        direction=0;
                        if(x>=st[stTop].x)
                        {
                                for(c=x;c>st[stTop].x;c--)
                                        if(map->getBlocker(c,y))
                                        {
                                                d++;
                                                direction=int(West);
                                        }

                        }
                        else
                        {
                                for(c=x;c<st[stTop].x;c++)
                                        if(map->getBlocker(c,y))
                                        {
                                                d++;
                                                direction=int(East);
                                        }
                        }
                        if(d>0)
                        {
                                if(abs(c-x)>6 || d>1)
                                        return getAction(Fire,Orz::FighterBase::DIRECTOR(direction),10);
                                switch(rand()%4)
                                {

                                case 0:
                                        if(x>st[stTop].x && RoundWall[East]>0)
                                                return getAction(Fire,East,10);
                                        if(x<st[stTop].x && RoundWall[West]>0)
                                                return getAction(Fire,West,10);
                                case 1:
                                        if(RoundWall[South]>0 && y<15 && map->getBlocker(x,y+1))
                                                return getAction(Fire,South,10);
                                        else
                                                if(RoundWall[North]>0)
                                                        return getAction(Fire,North,10);
                                case 2:
                                        return getAction(Sleep);
                                        break;
                                case 3:
                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        needCalculateRW[x][y] = 1;
                                        if(rand()%2)
                                                return getAction(Move,South,10);
                                        else
                                                return getAction(Move,North,10);
                                        break;
                                }
                        }

                        ForbackDirection=rand()%2?South:North;
                        Forback=1+rand()%2;
                        if(abs(x-st[stTop].x)<3)
                        {
                                if(power>200)
                                        Tactic = 0 ;
                                else
                                        Tactic=1+rand()%2;
                                c = 100;
                        }
                        else 
                        {
                                Tactic=1+rand()%2;
                                c = power;
                        }

                        needCalculateRW[x][y] = 1;
                        if(x>=st[stTop].x)
                        {
                                if(abs(y-st[stTop].y)<=2)EnemyHealth-=c;
                                return getAction(Fire,West,c);
                        }
                        else
                        {
                                if(abs(y-st[stTop].y)<=2)EnemyHealth-=c;
                                return getAction(Fire,East,c);
                        }
                }
                else
                {

                        myLastPos.x = x;
                        myLastPos.y = y;
                        needCalculateRW[x][y] = 1;
                        if(y<15 && map->getBlocker(x,y+1) || y ==15)
                                return getAction(Move,South,power);
                        else
                                return getAction(Move,North,power);
                }
        }
        //不在某战术范围内与敌处于一直线的处理结束

        //计算四个方向上的墙的数量
        if(needCalculateRW[x][y] && !WallisEmpty && (Strategy==2 || Strategy==3))
        {
                needCalculateRW[x][y] = 0;
                memset(RoundWall,0,4*sizeof(int));
                if(y!=(int)myLastPos.y)
                {
                        for(c=x+1;c<16;c++)
                                if(map->getBlocker(c,y))
                                        RoundWall[(int)East]++;
                        for(c=x-1;c>-1;c--)
                                if(map->getBlocker(c,y))
                                        RoundWall[(int)West]++;
                }
                if(x!=(int)myLastPos.x)
                {
                        for(c=y+1;c<16;c++)
                                if(map->getBlocker(x,c))
                                        RoundWall[(int)South]++;
                        for(c=y-1;c>-1;c--)
                                if(map->getBlocker(x,c))
                                        RoundWall[(int)North]++;
                }
        }
        switch(Strategy)
        {
        case 1:
                //接近敌人,在最近时选择休息或不休息
                if(abs(x-st[stTop].x)==1||abs(y-st[stTop].y)==1)
                        if(rand()%2)return getAction(Sleep);
                //用最快的方式靠拢敌人
                if(abs(x-st[stTop].x)>abs(y-st[stTop].y))
                {
                        if(y>st[stTop].y)
                        {
                                if(map->getBlocker(x,y-1))
                                {
                                        return getAction(Fire,North,10);
                                }
                                else
                                {

                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        return getAction(Move,South,10);
                                }
                        }
                        else
                        {
                                if(map->getBlocker(x,y+1))
                                {
                                        return getAction(Fire,South,10);
                                }
                                else
                                {

                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        return getAction(Move,North,10);
                                }
                        }
                }
                else
                {
                        if(x>st[stTop].x)
                        {
                                if(map->getBlocker(x-1,y))
                                {
                                        return getAction(Fire,West,10);
                                }
                                else
                                {

                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        return getAction(Move,West,10);
                                }
                        }
                        else
                        {
                                if(map->getBlocker(x+1,y))
                                {
                                        return getAction(Fire,East,10);
                                }
                                else
                                {

                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        return getAction(Move,East,10);
                                }
                        }
                        //在某一方向拉远敌人，远程进攻
                        //....
                }

                break;
        case 2:
                if((abs(x-st[stTop].x)>1 || abs(y-st[stTop].y)>1) && power<(myMaxPower-25))
                {
                        return getAction(Sleep);
                }
                std::cout<<"四周要打的墙的数量"<<RoundWall[0]<<RoundWall[1]<<RoundWall[2]<<RoundWall[3]<<std::endl;
                //所有墙打完后理论上没有战略2
                if(!(RoundWall[0] || RoundWall[1] || RoundWall[2] || RoundWall[3]))//本地四周墙已打完的处理
                {
                        std::cout<<"本地四周墙已打完的处理"<<std::endl;        
                        needCalculateRW[x][y] = 0;

                        if(y>7)
                        {
                                if(y==0)
                                {
                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        needCalculateRW[x][y+1] = 1;
                                        return getAction(Move,North,10);

                                }
                                for(c=1;c<16;c++)
                                {
                                        for(d=0;d<16;d++)
                                        {
                                                if(y+c<16 && map->getBlocker(d,y+c))
                                                {
                                                        if(x==myLastPos.x && y<myLastPos.y)
                                                        {
                                                                myLastPos.x = x;
                                                                myLastPos.y = y;
                                                                needCalculateRW[x][y-1] = 1;
                                                                return getAction(Move,South,10);
                                                        }
                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        needCalculateRW[x][y+1] = 1;
                                                        return getAction(Move,North,10);
                                                }
                                                if(y-c>-1 && map->getBlocker(d,y-c))
                                                {
                                                        if(x==myLastPos.x && y>myLastPos.y)
                                                        {
                                                                myLastPos.x = x;
                                                                myLastPos.y = y;
                                                                needCalculateRW[x][y+1] = 1;
                                                                return getAction(Move,North,10);
                                                        }
                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        needCalculateRW[x][y-1] = 1;
                                                        return getAction(Move,South,10);
                                                }
                                        }
                                }
                        }
                        else
                        {
                                if(y==15)
                                {
                                        myLastPos.x = x;
                                        myLastPos.y = y;
                                        needCalculateRW[x][y-1] = 1;
                                        return getAction(Move,South,10);
                                }
                                for(c=1;c<16;c++)
                                {
                                        for(d=0;d<16;d++)
                                        {                                                
                                                if(y-c>-1 && map->getBlocker(d,y-c))
                                                {
                                                        if(x==myLastPos.x && y>myLastPos.y)
                                                        {
                                                                myLastPos.x = x;
                                                                myLastPos.y = y;
                                                                needCalculateRW[x][y+1] = 1;
                                                                return getAction(Move,North,10);
                                                        }
                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        needCalculateRW[x][y-1] = 1;
                                                        return getAction(Move,South,10);
                                                }
                                                if(y+c<16 && map->getBlocker(d,y+c))
                                                {
                                                        if(x==myLastPos.x && y<myLastPos.y)
                                                        {
                                                                myLastPos.x = x;
                                                                myLastPos.y = y;
                                                                needCalculateRW[x][y-1] = 1;
                                                                return getAction(Move,South,10);
                                                        }
                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        needCalculateRW[x][y+1] = 1;
                                                        return getAction(Move,North,10);
                                                }
                                        }
                                }
                        }
                        WallisEmpty = true;
                }
                break;
				        case 3:
                if((abs(x-st[stTop].x)>1 || abs(y-st[stTop].y)>1) && power<(myMaxPower-25))
                {
                        return getAction(Sleep);
                }
                //如果血量不多就远离敌人
                //if(_health < 200)
                if(rand()%2)//改成随机吧
                {
                        if(abs(x-st[stTop].x)<3 )
                        {
                                if(x>=st[stTop].x)
                                {
                                        if(x==15)
                                        {
                                                //采取战术3或者什么也不做直接到毁墙部分
                                                if(abs(y-st[stTop].y)>8 && st[stTop].x==14)
                                                {
                                                        Tactic=3;
                                                        Forback=2+rand()%7;
                                                        ForbackDirection=(int)West;
                                                        isFirstStep = true;
                                                        if(y>=st[stTop].y)
                                                                return getAction(Fire,North,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,South,power/2>100?100:power/2);
                                                }
                                        }
                                        else
                                        {
                                                //如果被敌追，采取战术2
                                                if(abs(y-st[stTop].y)>8 && abs(x-st[stTop].x)==1 &&
                                                        ((st[stTop].x>st[stTop-1].x && st[stTop-1].x>st[stTop-2].x)
                                                        ||(st[stTop].x<st[stTop-1].x && st[stTop-1].x<st[stTop-2].x)))
                                                {
                                                        Tactic=2;
                                                        Forback=2;
                                                        if(st[stTop].x>st[stTop-1].x && st[stTop-1].x>st[stTop-2].x)
                                                                ForbackDirection=(int)East;
                                                        else
                                                                ForbackDirection=(int)West;
                                                        if(y>=st[stTop].y)
                                                                return getAction(Fire,North,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,South,power/2>100?100:power/2);
                                                }
                                                if(map->getBlocker(x+1,y))
                                                        return getAction(Fire,East,10);
                                                else
                                                {

                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        return getAction(Move,East,10);
                                                }
                                        }

                                }
                                else
                                {
                                        if(x==0)
                                        {
                                                //采取战术3或者什么也不做直接到毁墙部分
                                                if(abs(y-st[stTop].y)>8 && st[stTop].x==1)
                                                {
                                                        Tactic=3;
                                                        Forback=2+rand()%7;
                                                        ForbackDirection=(int)East;
                                                        isFirstStep = true;
                                                        if(y>=st[stTop].y)
                                                                return getAction(Fire,North,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,South,power/2>100?100:power/2);
                                                }
                                        }
                                        else
                                        {
                                                if(abs(y-st[stTop].y)>8 && abs(x-st[stTop].x)==1 &&
                                                        ((st[stTop].x>st[stTop-1].x && st[stTop-1].x>st[stTop-2].x)
                                                        ||(st[stTop].x<st[stTop-1].x && st[stTop-1].x<st[stTop-2].x)))
                                                {
                                                        Tactic=2;
                                                        Forback=2;
                                                        if(st[stTop].x>st[stTop-1].x && st[stTop-1].x>st[stTop-2].x)
                                                                ForbackDirection=(int)East;
                                                        else
                                                                ForbackDirection=(int)West;
                                                        if(y>=st[stTop].y)
                                                                return getAction(Fire,North,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,South,power/2>100?100:power/2);
                                                }
                                                if(map->getBlocker(x-1,y))
                                                        return getAction(Fire,West,10);
                                                else
                                                {

                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        return getAction(Move,West,10);
                                                }
                                        }
                                }
                        }
                        if(abs(y-st[stTop].y)<3 )
                        {
                                if(y>=st[stTop].y)
                                {
                                        if(y==15)
                                        {
                                                //采取战术3或者什么也不做直接到毁墙部分
                                                if(abs(x-st[stTop].x)>8 && st[stTop].y==14)
                                                {
                                                        Tactic=3;
                                                        Forback=2+rand()%7;
                                                        ForbackDirection=(int)South;
                                                        isFirstStep = true;
                                                        if(x>=st[stTop].x)
                                                                return getAction(Fire,West,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,East,power/2>100?100:power/2);
                                                }
                                        }
                                        else
                                        {
                                                if(abs(x-st[stTop].x)>8 && abs(y-st[stTop].y)==1 &&
                                                        ((st[stTop].y>st[stTop-1].y && st[stTop-1].y>st[stTop-2].y)
                                                        ||(st[stTop].y<st[stTop-1].y && st[stTop-1].y<st[stTop-2].y)))
                                                {
                                                        Tactic=2;
                                                        Forback=2;
                                                        if(st[stTop].y>st[stTop-1].y && st[stTop-1].y>st[stTop-2].y)
                                                                ForbackDirection=(int)North;
                                                        else
                                                                ForbackDirection=(int)South;
                                                        if(x>=st[stTop].x)
                                                                return getAction(Fire,West,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,East,power/2>100?100:power/2);
                                                }
                                                if(map->getBlocker(x,y+1))
                                                        return getAction(Fire,South,10);
                                                else
                                                {

                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        return getAction(Move,North,10);
                                                }
                                        }
                                }
                                else
                                {
                                        if(y==0)
                                        {
                                                //采取战术3或者什么也不做直接到毁墙部分
                                                if(abs(x-st[stTop].x)>8 && st[stTop].y==1)
                                                {
                                                        Tactic=3;
                                                        Forback=2+rand()%7;
                                                        ForbackDirection=(int)North;
                                                        isFirstStep = true;
                                                        if(x>=st[stTop].x)
                                                                return getAction(Fire,West,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,East,power/2>100?100:power/2);
                                                }
                                        }
                                        else
                                        {
                                                if(abs(x-st[stTop].x)>8 && abs(y-st[stTop].y)==1 &&
                                                        ((st[stTop].y>st[stTop-1].y && st[stTop-1].y>st[stTop-2].y)
                                                        ||(st[stTop].y<st[stTop-1].y && st[stTop-1].y<st[stTop-2].y)))
                                                {
                                                        Tactic=2;
                                                        Forback=2;
                                                        if(st[stTop].y>st[stTop-1].y && st[stTop-1].y>st[stTop-2].y)
                                                                ForbackDirection=(int)North;
                                                        else
                                                                ForbackDirection=(int)South;
                                                        if(x>=st[stTop].x)
                                                                return getAction(Fire,West,power/2>100?100:power/2);
                                                        else
                                                                return getAction(Fire,East,power/2>100?100:power/2);
                                                }
                                                if(map->getBlocker(x,y-1))
                                                        return getAction(Fire,North,10);
                                                else
                                                {

                                                        myLastPos.x = x;
                                                        myLastPos.y = y;
                                                        return getAction(Move,South,10);
                                                }
                                        }
                                }
                        }
                }
                else
                {
                        if(WallisEmpty != true)
                        {
                                myLastPos.x = x;
                                myLastPos.y = y;
                                if(!(RoundWall[0] || RoundWall[1] || RoundWall[2] || RoundWall[3]))//本地四周墙已打完的处理
                                {                                                                        

                                        //先搜索自己角落的墙
                                        if(y>=st[stTop].y)
                                        {
                                                for(c=y+1;c<16;c++)
                                                {
                                                        for(d=0;d<16;d++)
                                                                if(map->getBlocker(d,c))
                                                                {

                                                                        return getAction(Move,North,10);
                                                                }
                                                }
                                                if(abs(x-st[stTop].x)>8 && abs(y-st[stTop].y)==1 && EnemyMaxPower<130 && y>3)
                                                {
                                                        Tactic=1;
                                                        Forback=3;
                                                        ForbackDirection=(int)South;
                                                }
                                                return getAction(Move,South,10);
                                        }
                                        else
                                        {
                                                for(c=y-1;c>-1;c--)
                                                {
                                                        for(d=0;d<16;d++)
                                                                if(map->getBlocker(d,c))
                                                                {
                                                                        return getAction(Move,South,10);
                                                                }
                                                }
                                                if(abs(x-st[stTop].x)>8 && abs(y-st[stTop].y)==1 && EnemyMaxPower<130 && y<12)
                                                {
                                                        Tactic=1;
                                                        Forback=3;
                                                        ForbackDirection=(int)North;
                                                }
                                                return getAction(Move,North,10);
                                        }
                                }
                        }
                }

                break;
        }
        //战略结束

        //战略2，3共同部分
        //攻击墙
        if(RoundWall[0]>0||RoundWall[1]>0 ||RoundWall[2]>0 ||RoundWall[3]>0)
        {
                for(;;)
                {
                        c = (int)rand()%4;
                        if(RoundWall[c]>0)
                        {                
                                RoundWall[c]--;
                                myMaxPower+=3;
                                break;
                        }
                }

                d= 10+abs(y-st[stTop].y)>abs(x-st[stTop].x)?abs(x-st[stTop].x)*4*myMaxPower/200:abs(y-st[stTop].y)*4*myMaxPower/200;
                switch(c)
                {
                case 0:
                        return getAction(Fire,East,d);        
                        break;
                case 2:
                        return getAction(Fire,West,d);
                        break;
                case 1:
                        return getAction(Fire,South,d);
                        break;
                case 3:
                        return getAction(Fire,North,d);
                        break;
                }
        }
        std::cout<<"攻击墙结束"<<std::endl;
        //战略3：没有墙可打就向敌人方向发射炮弹
        if(Tactic==3 && (abs(x-st[stTop].x)>1 && abs(y-st[stTop].y)>1))
        {
                if(abs(x-st[stTop].x)>abs(y-st[stTop].y))
                {
                        if(x>st[stTop].x)
                        {
                                return getAction(Fire,West,10);
                        }
                        else
                        {
                                return getAction(Fire,East,10);
                        }
                }
                else
                {
                        if(y>st[stTop].y)
                        {
                                return getAction(Fire,North,10);
                        }
                        else
                        {
                                return getAction(Fire,South,10);
                        }
                }
        }
        return getAction(Sleep);
}

const std::string & FCFighterFactory::getTypeName() const
{
	static const std::string typeName(PROJECT_NAME);
	return typeName;
}


FCFighterFactory::pointer_type FCFighterFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	int pos = 0;
	int x = 0;
	int y = 0;
	int id= 0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("pos");
		if(it!= parameter->end())
		{
			pos = boost::get<int>(it->second);
			x = pos/16;
			y = pos%16;
		}

		
		it = parameter->find("id");
		if(it!= parameter->end())
		{
			id = boost::get<int>(it->second);
		}
	}


	return pointer_type(new FCFighter(instanceName, x, y, id));



}
void FCFighter::radar(ActionReturnPtr action)
{
	std::stringstream ss;

	
	if(action->get<3>()==this->_fighterID)
		ss<<"我";
	else
		ss<<"我的对手(id="<<action->get<3>()<<")，";
	
	if(action->get<0>() == Fire || action->get<0>() == Move)
	{
		ss<<"正在以("<<action->get<2>()<<")能量，向";
		
		switch(action->get<1>())
		{
		case East:
			ss<<"东，";
			break;
		case West:
			ss<<"西，";
			break;
		case North:
			ss<<"北，";
			break;
		case South:
			ss<<"南，";
			break;

		}
	}
	switch(action->get<0>())
	{
	case Fire:
		ss<<"开火。";
		break;
	case Move:
		ss<<"移动。";
		break;
	case Sleep:
		ss<<"休息。";
		break;

	case Die:
		ss<<"被摧毁。";
		break;

	case Hurt:
		ss<<"被击中。";
		break;

	}
	std::cout<<ss.str()<<std::endl;
	
}
