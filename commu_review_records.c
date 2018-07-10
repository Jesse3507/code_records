route_ext() -> basedate_file_extr() -> basedata_parse()

static ROUTE_EXTR_RESULT basedata_parse(
	struct rt_extrRoadStationTime *train_sch,
	FILE *bd_file, 
	ROUTE_LIST *r_list, 
	STATION_ADDR_T *ss_addr, 
	STATION_ADDR_T *es_addr, 
	uint16_t limit_level);

参数传入：	
*train_sch, 列车时刻表的提取结果，在route_ext()中初始化
*bd_file, basedata.dat的文件流
*r_list, 在route_ext()中申请内存，调用basedate_file_extr()时传入，调用完后再
    route_ext()中free掉

-------------------------------------------------------------------------------
交路提取review记录：
1.limit_merge() -> limit_code_table_extr()；
    limit_code_table_extr()在route_extract.c中定义，定义类型为指针数组
2.limit_merge()函数中，if(cnt_node->value.tele->t_num == 1186)是啥意思；
3.limit_adjust()函数定义前的宏定义：#define AIRWALKDIS 384；
4.limit_adjust()函数里的宏定义：#define spli_dist 2500；
5.limit_merge()函数的参数reve_info在该函数中并没有用到，当时为什么要定义这个参
    数，后续有无作用，没有的话可以将其删除；

-------------------------------------------------------------------------------
通信板程序现状:
    1. 始发站为中间车站时，LKJ要先按开车键后，才会显示优化曲线；
    2. 下行支线3转移测试：后按开车键，支线转移后相应的优化没有出来，不知和后按开
        车键有无关系

-------------------------------------------------------------------------------
函数功能记录
    get_transport_type(): 获取本补客货类型
    set_transport_type(): 获取本补客货类型
    route_extr()：线路数据提取，在main()中调用，在主线程执行，没有开新的线程。在
        揭示信息核对状态JIESHI_INFO_CHECK_STATUS时执行，分上下行调用两次。调用前
        先判断上下行，再判断LKJ的起始车站号是否属于上下行线路，如果属于才会调用
        route_extr()，如果不属于，则置状态机为SYSTEM_ERROR_STATUS
    station_in_num26_route_up(): 判断LKJ输入的起始车站号是否是上行线路的车站
    station_in_num26_route_down(): 判断LKJ输入的起始车站号是否是下行线路的车站
    is_extracted():判断是否已经提取过交路数据

/*
 *@brief 判断是否已经提取过交路数据
 *@param [in] base
 *param [out] resultPath 交路数据提取结果的存放路径
 *@return 0: 已经有存放交路数据提取结果的目录，说明已经提取过，则返回0; 1:没有提
 *  取过
 *@discrimination
 *  生成时刻表xml文件的md5码;
 *  生成交路数据存放路径
 *  sbin/route_data/20190329/26012000_6f2997ad46dfabb1d9ab35be60369c6d,
 *  并通过参数resultPath向外输出;
 */
int is_extracted(char *base,char *resultPath);

-------------------------------------------------------------------------------
通信板数据结构
    

全局变量
    CONVERT_MILE_NODE *cm_node 连续公里标

-------------------------------------------------------------------------------
通信板状态机
    【MAINTENANCE_STATUS】//维护状态
        1. 给通信备板发送进入维护状态热备事件[BACKUP_MAINTENANCE_STATUS_EVENT];
        2. 等待接受表决板发送退出维护事件[EXIT_MAINTENANCE_EVENT],接收成功，进入
        【PATTERN_SELECT_STATUS】;//模式选择状态
        3. 接收主备控制盒发送的更新程序事件[UPDATE_APP_EVENT],同步时间，更新程序

        
-------------------------------------------------------------------------------
6.22
取route_extract目录里的交路提取，单独构建工程，用来调试交路提取功能：
为了单独构建工程编译通过，除了route_extract目录里的代码，还需对route_extract目录
里的代码做一些修改，现记录如下：
    1.在reve_extract.c中，增加get_trip_num(),set_trip_num()函数，已解决函数没有
        定义的编译问题
    2.

-------------------------------------------------------------------------------
SECURE_VERSION_CHECK_STATUS