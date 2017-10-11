#include <stdio.h>
#include <stdlib.h>
#include "SM4Alg.h"
#include "sm4.h"
#include "DES.h"

CSM4Alg::CSM4Alg()
{
}

CSM4Alg::~CSM4Alg()
{
}

bool CSM4Alg::SM4CryptECB(char* szData, short sCryptFlag, char* szSM4Key, char* szResult)
{

		if(strlen(szSM4Key)!=32){
			return false;
		}

		int len = strlen(szData);
		if(len %32!=0){
			
			int count= len /32;
			count+=1;
			memset(&szData[len], '0', count*32-len);
			len = count * 32;
		}
		sm4_context ctx;
		char lpbKey[17];
		memset(lpbKey, 0x00, 17);
		DES des;
		des.STR2BCH(szSM4Key, lpbKey); //�������16���ƴ�תΪ�ֽڴ�

		if (sCryptFlag == 1)
			sm4_setkey_enc(&ctx,(unsigned char*)lpbKey); //����
		else
			sm4_setkey_dec(&ctx, (unsigned char*)lpbKey); //����
		unsigned char* lpbData = new unsigned char[len / 2 + 1];
		memset(lpbData, 0x00, len / 2 + 1);
		des.STR2BCH(szData, (char*)lpbData); //�������16���ƴ�תΪ�ֽڴ�
		 //lDataLengthӦΪ16��������
		unsigned char* pbyCryptResult = new unsigned char[len / 2 + 1];
		sm4_crypt_ecb(&ctx,sCryptFlag, len / 2, lpbData, pbyCryptResult);
		for (int i = 0; i < len/2; i++)
		{
			sprintf(&szResult[2 * i], "%02X", pbyCryptResult[i]);
		}
		delete[] lpbData;
		delete[] pbyCryptResult;
		lpbData = NULL;
		pbyCryptResult = NULL;
		return true;


}

//ÿ16�ֽڽ���SM4���ܣ�������16�ֽ������Ϊ��һ����������ݣ�����16�ֽ�β��0x00��ѭ��ֱ�����ݽ�����ɡ�
bool CSM4Alg::SM4MACGenerated(char* szData, char* szSM4Key, char* szResult)
{
	
		int len = strlen(szData);
		int count= len /32;
		if(len %32!=0)
		{			
			count += 1;
			memset(&szData[len], '0', count * 32 - len);
			len = count * 32;
		}
		
		for(int i=0;i<count;i++){
			char macString[33];
			memset(macString, 0x00, 33);
			memcpy(macString, &szData[32 * i], 32);
			if(i>0)
			{
				HEXStrXOR(macString,szResult);
			}
			SM4CryptECB(macString,1,szSM4Key,szResult);
		}
		return true;

}

void CSM4Alg::HEXStrXOR(char* str1, char* str2)
{
	char* pstr1=new char[17];
	memset(pstr1,0x00,17);
	char* pstr2=new char[17];
	memset(pstr2,0x00,17);

	DES des;
	des.STR2BCH(str1,pstr1);
	des.STR2BCH(str2,pstr2);

	unsigned char* pstr3=new unsigned char[17];
	memset(pstr3,0x00,17);
	for(int i=0;i<16;i++)
	{
		*(pstr3+i)=(unsigned char)((*(pstr1+i))^(*(pstr2+i)));
	}

	for (int i = 0; i < 16; i++)
	{
		sprintf(&str1[2 * i], "%02X", pstr3[i]);
	}

	delete []pstr1;
	delete []pstr2;
	delete []pstr3;
	pstr1=NULL;
	pstr2=NULL;
	pstr3=NULL;
}
