#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//函数声明
void _FileBuffer(char* str);
void _VirtualAlloc(size_t SizeofImage);
void _ReadFileSign();
void _WriteData();
void _ReadSectionTable(size_t NumberOfSection);

//全局变量
long long filesize = 0;
char*  FileBuffer = NULL;
LPVOID vFileBuffer = NULL;

//从缓冲区读取出来的值
DWORD NTHeader;
DWORD OPointOfSectionTable;

//PE头
WORD Machine;
WORD NumberOfSection;
WORD SizeOfOptionHeader;

//可选PE头
WORD Magic;					//10B x86	20B x86_64
DWORD AddressOfEntryPoint;
DWORD ImageBase;
DWORD SectionAlignment;
DWORD FileAlignment;
DWORD SizeOfHeader;
DWORD SizeOfImage;


struct {
	char* name;
	DWORD VirtualAddress[100];
	DWORD SizeOfRawData[100];
	DWORD PointerOfRawData[100];
}SectionTable;







//main函数负责调用
int main()
{
	_FileBuffer("E:/SpaceSniffer.exe");
	_ReadFileSign();
	_ReadSectionTable(NumberOfSection);
	_VirtualAlloc(SizeOfImage);
	_WriteData();
	printf("%x", SectionTable.VirtualAddress[0]);

}


//为文件分配内存，将指针返回给FileBuffer
void _FileBuffer(char* str)
{
	//打开文件
	char a;
	FILE* pFile;
	pFile = fopen(str, "rb");
	if (!pFile) {
		perror("打开文件失败");
		return;
	}

	//获取文件大小
	fseek(pFile, 0, SEEK_END);
	filesize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	//分配缓冲区
	FileBuffer = (char*)malloc(filesize);
	if (!FileBuffer) {
		perror("分配内存失败");
		fclose(pFile);
		return;
	}

	//将文件读入缓冲区并关闭文件句柄
	fread(FileBuffer, 1, filesize, pFile);
	fclose(pFile);
}




//读取PE文件标记
void _ReadFileSign() {
	//将pe头写入内存
	//标记位置
	NTHeader = *(DWORD*)(FileBuffer + 0x3c);
	

	Machine = *(WORD*)(FileBuffer + NTHeader + 0x4);
	NumberOfSection = *(WORD*)(FileBuffer + NTHeader + 0x6);
	SizeOfOptionHeader = *(WORD*)(FileBuffer + NTHeader + 0x14);


	OPointOfSectionTable = NTHeader + 0x18 + SizeOfOptionHeader;


	Magic = *(WORD*)(FileBuffer + NTHeader + 0x18); 
	AddressOfEntryPoint = *(DWORD*)(FileBuffer + NTHeader + 0x28);
	ImageBase = *(DWORD*)(FileBuffer + NTHeader + 0x34);
	SectionAlignment = *(DWORD*)(FileBuffer + NTHeader + 0x38);
	FileAlignment = *(DWORD*)(FileBuffer + NTHeader + 0x3c);
	SizeOfImage = *(DWORD*)(FileBuffer + NTHeader + 0x50);
	SizeOfHeader = *(DWORD*)(FileBuffer + NTHeader + 0x54);


	
}


//循环读取节表中的数据
void _ReadSectionTable(size_t NumberOfSection) {
	int i = 0;
	
	for (i = 0; i < NumberOfSection; i++) {
		SectionTable.SizeOfRawData[0] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0x10 + (0x28 * i));
		SectionTable.VirtualAddress[0] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0xc + (0x28 * i));
		SectionTable.SizeOfRawData[i] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0x10 + (0x28*i));
		SectionTable.VirtualAddress[i] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0xc + (0x28*i));
	}


}




//像系统申请内存
//申请成功后，将申请的内存头部放在vFileBuffer
void _VirtualAlloc(size_t SizeofImage) {
	//malloc分配的内存没有执行的属性，因此不能执行
	//像操作系统申请可读可写内存,拷贝header头
	vFileBuffer = VirtualAlloc(NULL, SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!vFileBuffer) {
		printf("申请内存失败");
		return;
	}
}


//将FileHeader写入vFileBuffer,因为不管是在文件还是在内存，它们的大小都是一样的
void _WriteData() {
	memcpy(vFileBuffer, FileBuffer, SizeOfHeader);
}















//跳转至代码块执行
/*
void _call() {
	__asm {
		mov eax, PointofCode;
		call eax;
	}
}
*/





