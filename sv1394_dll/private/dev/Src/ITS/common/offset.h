//File: offset.h

#ifndef __OFFSET_H__
#define __OFFSET_H__
///////////////////////////////////////////////////////////////////////////////

#define SENSOR_START_REG						0xF0F00300L		//设置操作Sensor的起始寄存器序号

#define SENSOR_I2C_OPERATE						0xF0F003A0L		//对SENSOR寄存器操作的地址

#define FORMAT7_DCAM_ADDR						0xF1F00000L

#define REG_CAMERA_COLUMN_SKIP_CTL				0xF0F008DCL //列跳 
#define REG_CAMERA_ROW_SKIP_CTL					0xF0F008E0L //行跳  
#define REG_CAMERA_ROW_BIN_MODE					0xF0F00350L      //行调和
#define REG_CAMERA_COL_BIN_MODE					0xF0F00354L      //列调和 

#define TRIGGER_ON_OFF				0x02000000
#define TRIGGER_MODE				0xF0F00830L
#define TRIGGER_POLARITY			0x01000000
#define ALL_TRIGGER_MODE			0x000F0000L		//TRIGGER mode:0x830[12:15]
#define TRIGGER_MODE_0			    0x00000000L		//
#define TRIGGER_MODE_1			    0x00010000L		//电平触发
#define TRIGGER_MODE_14             0x000E0000L		//自定义的高精度触发	//QiaoJH,2006.2.15

#define CCD_STROBE_POLARITY_START_ADDR	 0xF3F00300L		//Strobe极性设置的起始地址,偏移0处查询；偏移100处查询Strobe0;偏移200处设置Strobe0
#define CCD_LUT_ADDR					 0xF5F00000L		//LUT操作地址
#define CCD_STROBE0_SET_ADDR					0xF3F00500L    //闪光灯极性
#define CCD_Strobe_0_Cnt_On_Off					0x02000000L
#define CCD_Strobe_0_Cnt_Signal_Polarity		0x01000000L

//单色黑电平调节
#define REG_CAMERA_GREEN1_BRIGHTNESS_CTL		0xF0F008E4L     //绿一
#define REG_CAMERA_GREEN2_BRIGHTNESS_CTL		0xF0F008E8L     //绿二
#define REG_CAMERA_BLUE_BRIGHTNESS_CTL			0xF0F008ECL     //蓝色
#define REG_CAMERA_RED_BRIGHTNESS_CTL			0xF0F008F0L     //红色 


#define GREEN1_GAIN								0xF0F008CCL     //绿一
#define GREEN2_GAIN								0xF0F008D0L     //绿二
#define BLUE_GAIN								0xF0F008D4L     //蓝色
#define RED_GAIN								0xF0F008D8L     //红色

#define SHUTTER_UNIT							0xF0F0036CL

#define CCD_SOFT_TRIGGER			 			0xF0F0062CL		//DICAM1.31标准中定义
#define NOISE_CONTROL							0xF0F008C8L		//相机10选8控制


#define PRODUCT_ENCRYPT							0xF2F00000L		//产品加密字,CCD Camera
#define CCD_FIRM_VERSION						0xF2F00028L		//偏移28处保存DSP Version
#define CCD_SENSOR_ID							0xF2F00040L		//相机使用的Sensor的ID，比如1205
#define CCD_TEST_IMG_ADDR						0xF2F0081CL     //摄像机内部测试图像地址
#define CCD_STROBE_ON_OFF_ADDR					0xF3F00500L     //摄像机闪光灯可开关功能地址
#define CCD_8or12BitMode                        0xF1F00010L     //8位12位图像可选地址
#define CCD_OUTPUTIO_0                          0xF3F00000L     //输出IO_0地址
#define CCD_OUTPUTIO_1                          0xF3F00014L     //输出IO_1地址
#define CCD_OUTPUTIO_2                          0xF3F00028L     //输出IO_2地址
#define CCD_OUTPUTIO_CONTROL                    0xF3F00010L     //用户自定义输出IO控制地址
#define CCD_OUTPUTI1_CONTROL                    0xF3F00024L
#define CCD_OUTPUTI2_CONTROL                    0xF3F00038L
#define CCD_DELAY_TIME_ADDR                     0xF0F00834L     //触发后延迟曝光地址
#define CCD_FPGA_VERSION                        0xF2F00038L		//保存FPGA Version
#define CCD_TRANSFERSDELAY_ADDR                 0xF2F00824L     //延迟传输地址
#define CCD_FILTER_TIME_ADDR	                0xF2F00850L     //滤波时间寄存器地址
#define CCD_LE_BE_MODE_ADDR						0xF2F00854L     //big-endian little-endian 地址

#define CCD_BLACK_CLAMP_LEVEL                   0xF3F00060L     //  for 插件 test    
#define CCD_H1DRV                               0xF3F00064L
#define CCD_H2DRV                               0xF3F00068L
#define CCD_H3DRV                               0xF3F0006CL
#define CCD_H4DRV                               0xF3F00070L
#define CCD_RGDRV                               0xF3F00074L
#define CCD_H1POL                               0xF3F00078L
#define CCD_H1POSLOC                            0xF3F0007CL
#define CCD_RGPOL                               0xF3F00080L
#define CCD_RGPOSLOC                            0xF3F00084L
#define CCD_AD9849_SHPPOSLOC                    0xF3F00088L
#define CCD_AD9849_SHDPOSLOC                    0xF3F0008CL


//自动功能段  AF(Automatic Function)
#define AF_CTRL_INQ                             0xFAF00000L
#define AF_AUTO_GAIN_CTRL_INQ                   0xFAF00100L     //自动增益控制查询
#define AF_AUTO_SHUTTER_CTRL_INQ                0xFAF00104L     //自动曝光控制查询
#define AF_AREA_MAX_SIZE_INQ                    0xFAF00110L     //自动功能感兴趣区域 最大范围
#define AF_AREA_UNIT_SIZE_INQ                   0xFAF00114L     //自动功能感兴趣区域 步长

#define AF_AUTO_STROBE_INQ                      0xFAF00120L
#define AF_AUTO_STROBE_SHUTTER_INQ              0xFAF00124L
#define AF_AUTO_STROBE_GAIN_INQ                 0xFAF00128L
#define AF_AUTO_STROBE_STAT_INQ                 0xFAF0012CL


#define AF_AUTO_GAIN_CTRL                       0xFAF00800L     //自动增益控制
#define AF_AUTO_SHUTTER_CTRL                    0xFAF00804L     //自动曝光控制
#define AF_AUTO_SHUTTER_LO                      0xFAF00808L     //Minimum auto shutter value
#define AF_AUTO_SHUTTER_HI                      0xFAF0080CL     //Maximum auto shutter value
#define AF_AUTO_FNC_AOI                         0xFAF00810L     //自动功能AOI
#define AF_AF_AREA_POSITION                     0xFAF00814L     //自动功能AOI位置
#define AF_AF_AREA_SIZE                         0xFAF00818L     //自动功能AOI大小

#define AF_AUTO_STROBE                          0xFAF00820L
#define AF_AUTO_STROBE_SHUTTER                  0xFAF00824L
#define AF_AUTO_STROBE_GAIN                     0xFAF00828L
#define AF_AUTO_STROBE_STAT                     0xFAF0082CL



///////自动功能上下限//////////////
#define  SPT_AUTO_GAIN_MAX_VALUE       1023
#define  SPT_AUTO_GAIN_MIN_VALUE       0

#define  SPT_AUTO_SHUTTER_MAX_VALUE     4000
#define  SPT_AUTO_SHUTTER_MIN_VALUE     20





//与Sensor相关的寄存器的定义：
/* Micron MI-1300 SCCB registers */
#define	CHIPID			0x00


__inline DWORD BE2LE(DWORD x)
{
    return ((DWORD)( \
                     (((DWORD)(x) & (DWORD)0x000000ffUL) << 24) | \
                     (((DWORD)(x) & (DWORD)0x0000ff00UL) <<  8) | \
                     (((DWORD)(x) & (DWORD)0x00ff0000UL) >>  8) | \
                     (((DWORD)(x) & (DWORD)0xff000000UL) >> 24) ));\
}

//
// Reverse a word low 8 bits, and move to high 8 bits.
// Move original 8 bits to low 8 bits.
__inline WORD MAKEPASS(WORD wValue)
{
    WORD temp = ~(wValue & 0x00FF);
    return (wValue>>8) | (temp<<8);
}

///////////////////////////////////////////////////////////////////////////////
#endif //__OFFSET_H__