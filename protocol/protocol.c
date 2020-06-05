#include<stdio.h>
#pragma warning(disable:4996)

int frame_num;//帧序号
int luminance = 150;//亮度，范围0x00~0xFF
int lumin_trantime = 0;//亮度过渡时间，单位50ms，0表示使用默认的过渡效果，1表示50ms
int colortemp_degree = 0;//色温等级
int colortemp_trantime = 0;//过渡时间，单位50ms，0表示使用默认的过渡效果，1表示50ms
int status = 0x00;//系统状态
int normal = 0;
int overheat = 1;//过温，1为过温，0没有过温
int overpressure = 0;//过压，1为过压，0没有过压
int overcurrent = 1;//过流，1为过流， 0没有过流
double version = 0.1;//协议版本
int type_modulate = 0;//调光电源调制类型，恒流电源
int type_setting = 0;//调光电源设置类型，恒压电源
int memory = 0;
int main()
{
	void e1();
	int Hex_val_1[100] = { 0x00, 0x5d, 0x5A, 0xA5, 0x01, 0x00, 0x01, 0x03, 0x64, 0x00, 0x64, 0x01, 0xCC, 0x16, 0x8E };
	int Hex_val_2[100] = { 0x00, 0x5A, 0xA5, 0x01, 0x00, 0x02, 0x03, 0xC8, 0x01, 0x2C, 0x01, 0xFA, 0x16, 0x43, 0x23, 0x16 };
	int Hex_val_3[100] = { 0x5A, 0xA5, 0x00, 0x00, 0x03, 0x01, 0x00, 0x01, 0x03, 0x16 };
	int Hex_val_4[100] = { 0x67, 0x2B, 0x3F, 0x5A, 0xA5, 0x02, 0x00, 0x04, 0x01, 0x00, 0x01, 0x06, 0x16, 0x4A, 0x5B };
	int Hex_val_5[100] = { 0x5A, 0xA5, 0x00, 0x00, 0x05, 0x01, 0x00, 0x01, 0x05, 0x16 };
	int Hex_val_6[100] = { 0x5A, 0xA5, 0x00, 0x00, 0x06, 0x01, 0x00, 0x01, 0x06, 0x16 };
	int Hex_val_7[100] = { 0x5A, 0xA5, 0x00, 0x00, 0x07, 0x01, 0x00, 0x01, 0x07, 0x16 };
	int Hex_val_8[100] = { 0x5A, 0xA5, 0x00, 0x00, 0x08, 0x01, 0x00, 0x01, 0x08, 0x16 };
	int Hex_val_9[100] = { 0x5A, 0xA5, 0x00, 0x00, 0x09, 0x01, 0x01, 0x01, 0x0A, 0x16 };
	e1(Hex_val_1);
	e1(Hex_val_2);
	e1(Hex_val_3);
	e1(Hex_val_4);
	e1(Hex_val_5);
	e1(Hex_val_6);
	e1(Hex_val_7);
	e1(Hex_val_8);
	e1(Hex_val_9);
	getchar();
	return 0;
}


void e1(int Hex_val_1[]) {//加上怎么检验
	int recstatu = 0;//是否处于正在接受数据的状态
	int ccnt = 0;//计数
	int packerflag = 0;//是否接受到一个完整的数据帧
	int i = 0, j = 0;
	int Hex[12] = { 0 };//接受的完整数据帧
	int Hex_val[100];


#if(1)
	for (int i = 0; i < 100; i++)
	{
		Hex_val[i] = Hex_val_1[i];
	}
#endif
#if(0)
	while (1) {
		scanf("%02X", &Hex_val[i]);
		i++;
		if (getchar() == '\n') {
			break;
		}
	}
#endif
	i = 0;
	while (i < 100) {//抽取一串数据中的完整数据帧
		if (Hex_val[i] == 0x5A && Hex_val[i + 1] == 0xA5) {//检测到帧头时，开始接受数据
			recstatu = 1;
			ccnt = 0;
		}
		if (Hex_val[i] == 0x16) {//检测到帧尾时，结束接受数据并离开循环
			recstatu = 0;
			break;
		}
		if (recstatu == 1) {//当处于接受数据的状态时，将数据帧赋予Hex数组
			Hex[ccnt++] = Hex_val[i];
		}
		i++;
	}
	Hex[ccnt] = 0x16;//给Hex数据帧数组填上帧尾
	int sum = 0;//定义为帧头到数据所有字节的和
	for (i = 0; i < ccnt - 2; i++)//发送码从帧头到数据所有字节的和
	{
		sum += Hex[i];
	}
	if (sum == (Hex[ccnt - 2] << 8 | Hex[ccnt - 1])) {//若校验值与帧头到数据所有字节和等值，则认定接收到完整的数据帧
		packerflag = 1;
	}
	printf("ccnt=%d, packerflag=%d\n", ccnt, packerflag);

	int Hex_reply1[10] = { 0x5A, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16 };//第一种应答码，应答亮度，色温，系统状态
	int Hex_reply2[11] = { 0x5A, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16 }; //第二种应答码，应答协议版本，调光电源调制类型。

	printf("SEND FRAME:\t");//展示发送码
	for (i = 0; i <= ccnt; i++)
	{
		printf("%02X ", Hex[i]);
	}
	printf("\n");
	if (packerflag == 1) {
		frame_num = Hex[2];
		if (Hex[4] == 01) {//设置亮度
			printf("Set the luminance: ");
			luminance = Hex[6];//将发送码数据位高位数据赋予亮度。
			lumin_trantime = (Hex[7] << 8 | Hex[8]) * 50;//将发送码数据位后二位数据赋予过渡时间
			printf("Turn on the LED, luminance degree = %d, lumin trantime = %dms, Frame number=%d.\n\n", luminance, lumin_trantime, frame_num);
		}
		if (Hex[4] == 0x02) {
			printf("Set the color temperature: ");//设置色温
			colortemp_degree = Hex[6];//将发送码数据位高位数据赋予色温。
			colortemp_trantime = (Hex[7] << 8 | Hex[8]) * 50;//将发送码数据位后二位数据赋予过渡时间
			printf("colortemp degree = %d, colortemp trantime = %dms, Frame number = %d.\n\n", colortemp_degree, colortemp_trantime, frame_num);
		}
		if (Hex[4] == 0x03) {
			int sum = 0;
			Hex_reply1[2] = Hex[2];//将发送码帧序号赋予应答码帧序号
			Hex_reply1[3] = Hex[4];//将发送码中控制码低位赋予应答码中控制码高低位
			Hex_reply1[4] = Hex[4];
			Hex_reply1[5] = 0x01;//应答码数据长度赋予1
			Hex_reply1[6] = luminance;//将亮度等级赋予应答码数据位
			sum = Hex_reply1[0] + Hex_reply1[1] + Hex_reply1[2] + Hex_reply1[3] + Hex_reply1[4] + Hex_reply1[5] + Hex_reply1[6];
			Hex_reply1[7] = sum >> 8;////将帧头到数据所有字节的和的十六进制高位赋予应答码校验值高位
			Hex_reply1[8] = sum & 0xFF;//将帧头到数据所有字节的和的十六进制低位赋予应答码校验值低位
			printf("RELPY FRAME:\t");
			for (i = 0; i < 10; i++) {//输出应答码
				printf("%02X ", Hex_reply1[i]);
			}
			printf("\nInquery the luminance:\n");//查询亮度
			printf("The current luminance level is %d.\n\n", luminance);//输出亮度
		}
		if (Hex[4] == 0x04) {
			int sum = 0;
			Hex_reply1[2] = Hex[2];//将发送码帧序号赋予应答码帧序号
			Hex_reply1[3] = Hex[4];//将发送码中控制码低位赋予应答码中控制码高低位
			Hex_reply1[4] = Hex[4];
			Hex_reply1[5] = 0x01;//应答码数据长度赋予1
			Hex_reply1[6] = colortemp_degree;//将色温等级赋予应答码数据位
			sum = Hex_reply1[0] + Hex_reply1[1] + Hex_reply1[2] + Hex_reply1[3] + Hex_reply1[4] + Hex_reply1[5] + Hex_reply1[6];
			Hex_reply1[7] = sum >> 8;//将帧头到数据所有字节的和的十六进制高位赋予应答码校验值高位
			Hex_reply1[8] = sum & 0xFF;//将帧头到数据所有字节的和的十六进制低位赋予应答码校验值低位
			printf("RELPY FRAME:\t");
			for (i = 0; i < 10; i++) {//输出应答码
				printf("%02X ", Hex_reply1[i]);
			}
			printf("\nInquery the color temperature: \n");//查询色温
			printf("The current color temperature is %d.\n\n", colortemp_degree);
		}
		if (Hex[4] == 0x05) {
			int sum = 0;
			Hex_reply1[2] = Hex[2];//将发送码帧序号赋予应答码帧序号
			Hex_reply1[3] = Hex[4];//将发送码中控制码低位赋予应答码中控制码高低位
			Hex_reply1[4] = Hex[4];
			Hex_reply1[5] = 0x01;//应答码数据长度赋予1
			if (overheat == 1) {//多种状态按位或整合
				status |= 0x01;
			}
			if (overpressure == 1) {
				status |= 0x02;
			}
			if (overheat == 1) {
				status |= 0x04;
			}
			if (normal == 1) {
				status = 0x00;
			}
			Hex_reply1[6] = status;//将系统状态赋予应答码数据位
			sum = Hex_reply1[0] + Hex_reply1[1] + Hex_reply1[2] + Hex_reply1[3] + Hex_reply1[4] + Hex_reply1[5] + Hex_reply1[6];
			Hex_reply1[7] = sum >> 8;//将帧头到数据所有字节的和的十六进制高位赋予应答码校验值高位
			Hex_reply1[8] = sum & 0xFF;//将帧头到数据所有字节的和的十六进制低位赋予应答码校验值低位
			printf("RELPY FRAME:\t");
			for (i = 0; i < 10; i++) {//输出应答码
				printf("%02X ", Hex_reply1[i]);
			}
			printf("\nThe status of current system is ");//回复系统状态
			if (normal == 1) {
				printf("%s\n", "normal");
			}
			else {
				if (overheat == 1) {
					printf("%s", "overheat ");
				}
				if (overpressure == 1) {
					printf("%s", "overpressure ");
				}
				if (overcurrent == 1) {
					printf("%s", "overcurrent ");
				}
			}
			printf("\n\n");
		}
		if (Hex[4] == 0x06) {
			int sum = 0;
			Hex_reply2[2] = Hex[2];//将发送码帧序号赋予应答码帧序号
			Hex_reply2[3] = Hex[4];//将发送码中控制码低位赋予应答码中控制码高低位
			Hex_reply2[4] = Hex[4];
			Hex_reply2[5] = 0x02;//应答码数据长度赋予2
			Hex_reply2[6] = (int)(version / 0.1) >> 8;//将协议版本高位赋予数据位高位
			Hex_reply2[7] = (int)(version / 0.1) & 0xFF;//将协议版本低位赋予数据位低位
			sum = Hex_reply2[0] + Hex_reply2[1] + Hex_reply2[2] + Hex_reply2[3] + Hex_reply2[4] + Hex_reply2[5] + Hex_reply2[6] + Hex_reply2[7];
			Hex_reply2[8] = sum >> 8;//将帧头到数据所有字节的和的十六进制高位赋予应答码校验值高位
			Hex_reply2[9] = sum & 0xFF;//将帧头到数据所有字节的和的十六进制低位赋予应答码校验值低位
			printf("RELPY FRAME:\t");
			for (i = 0; i < 11; i++) {//输出应答码
				printf("%02X ", Hex_reply2[i]);
			}
			printf("\nThe current version of the protocol is %g.\n\n", version);
		}
		if (Hex[4] == 0x07) {
			int sum = 0;
			Hex_reply2[2] = Hex[2];//将发送码帧序号赋予应答码帧序号
			Hex_reply2[3] = Hex[4];//将发送码中控制码低位赋予应答码中控制码高低位
			Hex_reply2[4] = Hex[4];
			Hex_reply2[5] = 0x02;//应答码数据长度赋予2
			Hex_reply2[6] = type_modulate;
			Hex_reply2[7] = type_setting;
			sum = Hex_reply2[0] + Hex_reply2[1] + Hex_reply2[2] + Hex_reply2[3] + Hex_reply2[4] + Hex_reply2[5] + Hex_reply2[6] + Hex_reply2[7];
			Hex_reply2[8] = sum >> 8;//将帧头到数据所有字节的和的十六进制高位赋予应答码校验值高位
			Hex_reply2[9] = sum & 0xFF;//将帧头到数据所有字节的和的十六进制低位赋予应答码校验值低位
			printf("RELPY FRAME:\t");
			for (i = 0; i < 11; i++) {//输出应答码
				printf("%02X ", Hex_reply2[i]);
			}
			printf("\nThe power supply is ");
			if (type_setting == 0) {
				printf("luminance ");
			}
			else {
				printf("luminance and color temprature ");
			}
			if (type_modulate == 1) {
				printf("constant current.");
			}
			else {
				printf("constant voltage.");
			}
			printf("\n\n");
		}
		if (Hex[4] == 0x08) {//复位亮度与色温
			luminance = 0xFF;//将0xFF赋予亮度等级，恢复亮度等级为255。
			colortemp_degree = 0xFF;//将0xFF赋予色温等级，恢复色温等级为255。			
			printf("Renew the degree of luminance and color temperature,  luminance degree is %d, color temperature is %d.\n\n", luminance, colortemp_degree);
		}
		if (Hex[4] == 0x09) {//开启关闭记忆功能
			memory = Hex[6];//开启记忆功能。
			if (memory == 1) {
				printf("turn on memory mode.");
			}
			else {
				printf("turn off memory mode.");
			}
		}
	}
	else {
		printf("error packerage\n");
	}
}
