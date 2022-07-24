/**
* Copyright (c) 2022, RYue All rights reserved.
*
* @Description: 运行体接口
* @author: RYue
* @date: 2022-07-01 11:56:41
*/

Class IRunnable
{
public:
    /**
    * @Description: 启动运行
    */
    virtual void Run() = 0;

    /**
    * @Description: 停止运行
    */
    virtual void Stop() = 0;
}