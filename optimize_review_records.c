-------------------------------------------------------------------------------
优化计算代码工程调试：
    调试时用目录optimeze_3.0_branch3 - copy里的代码工程，调试成功后
    Beyond_compare到目录optimeze_3.0_branch3

-------------------------------------------------------------------------------
在优化程序RawData文件夹中的optimize_strategy.xml，对坡度类型做了定义
	roadCategory结构体中flag的含义:
	3分相区
	2陡上坡
	0缓坡
	-2陡下坡

-------------------------------------------------------------------------------
侧线正线起停车、降级起车，贯通试验
	降级起车tmp_limit_start_strgy_from_the_first_station()
	贯通试验thro_expert_optimize()
	侧线起车tmp_limit_start_strgy()
	侧线停车tmp_limit_stop_strgy()

-------------------------------------------------------------------------------
起停车，过分相测算，贯通试验策略里用到的子操作
    限速起车：
-------------------------------------------------------------------------------
侧线正线起停车、降级起车，贯通试验
	降级起车tmp_limit_start_strgy_from_the_first_station()
	贯通试验thro_expert_optimize()
	侧线起车tmp_limit_start_strgy()
    侧线停车tmp_limit_stop_strgy()

-------------------------------------------------------------------------------
函数功能记录
	DoCaculateByDistance() 牵引计算
	DoCaculateByTime() 牵引计算
	post_process() 将结构体OPTSCH转换成OPTSCHCURVE, 在优化曲线输出前之前调用
    pre_process()将结构体OPTSCHCURVE转换成OPTSCH, 在优化曲线计算之前调用
	reverse_tmp_optimize() 反求曲线主入口
	dead_zone_const_gear_reverse() 求进入分相区前的最慢行驶与最快行驶优化曲线
    const_gr_til_***() 惰行正求
    air_break_process_imp 空气制动正求
    air_break_process_rvrs 空气制动反求
    tmp_rvrs_cal_til_post() 恒档反求，函数实现里挡位gear为恒值
    back_set_tmp_optSch() 反向存储
    merge_two_curve() 拼接曲线
    extrDeadzoneInfo() 提取分相区信息

-------------------------------------------------------------------------------
函数内部实现
    thro_expert_strgy()贯通实验策略函数实现：
        确定贯通实验起始点所在坡段的下标rcIndex;
        确定贯通实验起始点所在原始优化曲线的下标start_opt_index;
        给贯通曲线点元素分配空间；
        给贯通曲线第一个点赋值；
        若挡位gear > 0, 调用dec_gr_til_posGp_or_post()降到0档，若到所在第一个坡段
            结束未降到0档，则继续在下一个坡段调用dec_gr_til_posGp_or_post()，直到
            降到0档；
        若挡位gear < 0, 调用dec_gr_til_negGp_or_post()降到-120档，若到所在第一个坡
            段结束未降到-120档，则继续在下一个坡段调用dec_gr_til_negGp_or_post()，直到
            降到-120档；
        维持0档跑5s;
        调用空气制动air_break_process_imp()；
        确定调用完air_break_process_imp()后输出的曲线结束点所对应的目标曲线的下标；
        调用dwn_to_up_fwd_cal_til_post()追原始曲线，若遇到分相区，则调用pass_split_phase_strgy().找到交点后返回
    
    air_break_process_imp()空气制动
        根据坡度算减压值；
        计算空走时间；

-------------------------------------------------------------------------------
数据结构记录
    优化计算用到的数据结构定义在opt_definition.h文件中
    OPTSCHCURVE结构体 用来描述优化曲线
    新定义了结构体用来表示过分相测算的输出曲线
        struct deadzone_base {
        OPTSCHCURVE **access_deadzone_min;
        OPTSCHCURVE **access_deadzone_max;
        DEADZONE *deadzone;
        };
        typedef struct deadzone_base DEADZONE_BASE;

-------------------------------------------------------------------------------
全局变量
    在extractData.c文件中，定义了一些全局变量；

-------------------------------------------------------------------------------
tmp_limit_start_strgy()原来代码中的起车曲线函数
计算起车点对应的分段坡度的下标startRcIndex；
计算起车点对应的原始优化的下标startOptIndex；
给起车曲线第一个点赋值；
调用const_gr_til_time_or_post()恒档跑5s;
判断恒档5s后的速度，若小于零，则取最后一个速度大于零的点；
若flag == -1,则更改下次调用const_gr_til_time_or_post()的入参time;
判断恒档5s后是否跑过10m, 若没有，则更改档位，循环调用const_gr_til_post(),直至跑过10m;
追原始曲线；
疑点
1.maxV为什么赋了两次值，后面也没有用到，是否可注销掉
2.变量mc也没有用到

dead_zone_const_gear_reverse()计算过分相前的最低限速曲线和最高限速曲线