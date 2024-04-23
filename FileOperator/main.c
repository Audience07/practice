#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
//��������
void _FileBuffer(char* str);
void _VirtualAlloc(size_t SizeofImage);
void _ReadFileSign();
void _WriteData();
void _ReadSectionTable(size_t NumberOfSection);

//ȫ�ֱ���
long long filesize = 0;
char*  FileBuffer = NULL;
LPVOID vFileBuffer = NULL;

//�ӻ�������ȡ������ֵ
DWORD NTHeader;
DWORD OPointOfSectionTable;

//PEͷ
WORD Machine;
WORD NumberOfSection;
WORD SizeOfOptionHeader;

//��ѡPEͷ
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







//main�����������
int main()
{
	_FileBuffer("E:/SpaceSniffer.exe");
	_ReadFileSign();
	_ReadSectionTable(NumberOfSection);
	_VirtualAlloc(SizeOfImage);
	_WriteData();
	printf("%x", SectionTable.VirtualAddress[0]);

}


//Ϊ�ļ������ڴ棬��ָ�뷵�ظ�FileBuffer
void _FileBuffer(char* str)
{
	//���ļ�
	char a;
	FILE* pFile;
	pFile = fopen(str, "rb");
	if (!pFile) {
		perror("���ļ�ʧ��");
		return;
	}

	//��ȡ�ļ���С
	fseek(pFile, 0, SEEK_END);
	filesize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	//���仺����
	FileBuffer = (char*)malloc(filesize);
	if (!FileBuffer) {
		perror("�����ڴ�ʧ��");
		fclose(pFile);
		return;
	}

	//���ļ����뻺�������ر��ļ����
	fread(FileBuffer, 1, filesize, pFile);
	fclose(pFile);
}




//��ȡPE�ļ����
void _ReadFileSign() {
	//��peͷд���ڴ�
	//���λ��
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


//ѭ����ȡ�ڱ��е�����
void _ReadSectionTable(size_t NumberOfSection) {
	int i = 0;
	
	for (i = 0; i < NumberOfSection; i++) {
		SectionTable.SizeOfRawData[0] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0x10 + (0x28 * i));
		SectionTable.VirtualAddress[0] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0xc + (0x28 * i));
		SectionTable.SizeOfRawData[i] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0x10 + (0x28*i));
		SectionTable.VirtualAddress[i] = *(DWORD*)(FileBuffer + OPointOfSectionTable + 0xc + (0x28*i));
	}


}




//��ϵͳ�����ڴ�
//����ɹ��󣬽�������ڴ�ͷ������vFileBuffer
void _VirtualAlloc(size_t SizeofImage) {
	//malloc������ڴ�û��ִ�е����ԣ���˲���ִ��
	//�����ϵͳ����ɶ���д�ڴ�,����headerͷ
	vFileBuffer = VirtualAlloc(NULL, SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!vFileBuffer) {
		printf("�����ڴ�ʧ��");
		return;
	}
}


//��FileHeaderд��vFileBuffer,��Ϊ���������ļ��������ڴ棬���ǵĴ�С����һ����
void _WriteData() {
	memcpy(vFileBuffer, FileBuffer, SizeOfHeader);
}















//��ת�������ִ��
/*
void _call() {
	__asm {
		mov eax, PointofCode;
		call eax;
	}
}
*/





