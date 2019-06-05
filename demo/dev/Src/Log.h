//日志文件

//2006.8.17 HYL建


//2006.8.17 版本升级为v2.4.0.7
//去掉了黑白摄像机的白平衡调节属性
//添加宏 IS_NOT_SV_COLOR
////	m_View.SetNewAOI(); //去掉双击效果 HYL 2006.8.17 

//2006.8.21 版本升级为v2.4.0.8
//统一SV系列的属性参数的默认值及可调范围
/*
DH-SV1300FC/FM:
参数	
DH-SV1300FM黑白像机								DH-SV1300FC彩色像机
				默认值			取值范围		默认值			取值范围
快门速度		133ms			20us~1s			133ms			20us~1s
采集方式		continuation	continuation、	continuation	continuation
								trigger、						trigger
								trigger level					trigger level
外触发信号极性	High			High、Low		High			High、Low
闪光灯信号极性	High			High、Low		High			High、Low
增益			296				0~1023			296				0~1023
LUT文件			3~10			0~7、1~8、		3~10			0~7、1~8、
								2~9、3~10、						2~9、3~10、
								4~11							4~11
亮度调节		16				0~255			16				0~255
白平衡U/B		无				无				28				0~31
白平衡V/R		无				无				14				0~31
包长			1600			1600~4092		1600			1600~4092
帧冻结			off				off、on			off				off、on
*/

/*
DH-SV1310FC/FM:
参数	
DH-SV1300FM黑白像机								DH-SV1300FC彩色像机
				默认值			取值范围		默认值			取值范围
快门速度		60ms			20us~1s			60ms			20us~1s
采集方式		continuation	continuation、	continuation	continuation
								trigger、						trigger
								trigger level					trigger level
外触发信号极性	High			High、Low		High			High、Low
闪光灯信号极性	High			High、Low		High			High、Low
增益			296				0~1023			296				0~1023
LUT文件			3~10			0~7、1~8、		3~10			0~7、1~8、
								2~9、3~10、						2~9、3~10、
								4~11							4~11
亮度调节		16				0~255			16				0~255
白平衡U/B		无				无				28				0~31
白平衡V/R		无				无				14				0~31
包长			2500			2500~4092		2500			2500~4092
帧冻结			off				off、on			off				off、on
*/


//2006.9.11 版本升级为v2.4.0.9
//添加宏 IS_HV1300
//支持HV1300FC