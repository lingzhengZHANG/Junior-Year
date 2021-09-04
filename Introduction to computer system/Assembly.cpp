#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h> 
#include<iostream>
using namespace std;

char str[50];
char Binary[50];
void BinaryTrans(int t); 
int Adress();
FILE *fp,*fp1,*fw1,*fw2;
char AllOperators[24][6]={"ADD","AND","BRz","BRp","BRn","BRnz","BRnp","BRzp","BRnzp","JMP","JSR","JSRR","LD","LDI","LDR","LEA","NOT","RET","RTI","ST","STI","STR","TRAP","HALT"};
char ALLPseudo_ops[9][9]={".FILL",".BLKW",".STRINGZ",".END","PUTS","IN","GETC","OUT","PUTSP"};
char LABEL[50]={'\0'};
char Operator[6]={'\0'};
char Pseudo_ops[9]={'\0'};
char Operand[16]={'\0'};
char Labels[1000][50];
int Label_Address[1000]={0};
long int START;
int count_instruction=0;
int count_operand=0;
int count_label=0;
char c;
int checkerror=0;
int stopprogram=0;
int existend=0;

char HexToBinary[16][5] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
//char *string=(char*)malloc(sizeof(char)*100);
char string1[100];
char annotation[100];
char CheckHalt[5];
int Stopsign=0;

void TranToBinary(int num); //input Binary number, Binary will be the binary form


bool CheckIfOperator(void)
{
    int i;
    for(i=0;i<24;i++)
        if(!strcmp(AllOperators[i],LABEL))
            return 1;
    return 0;
}

bool CheckIfPseudoops(void)
{
	int i;
	for(i=0;i<9;i++)
		if(!strcmp(ALLPseudo_ops[i],LABEL))
			return 1;
	return 0;
}

bool CheckIfLabels(void)
{
	int i;
	for(i=0;i<count_label;i++);
		
}

void AND(){
	fprintf(fw2,"0101");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	if(str[0] == 'R'){
		fprintf(fw2,"000");
		BinaryTrans(3);
		fprintf(fw2,"%s",Binary);
	}else{
		fprintf(fw2,"1");
		BinaryTrans(5);
		fprintf(fw2,"%s",Binary);		
	}
}
void ADD(){
	fprintf(fw2,"0001");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	if(str[0] == 'R'){
		fprintf(fw2,"000");
		BinaryTrans(3);
		fprintf(fw2,"%s",Binary);
	}else{
		fprintf(fw2,"1");
		BinaryTrans(5);
		fprintf(fw2,"%s",Binary);		
	}
}

void BR(){
	fprintf(fw2,"0000");
	if(!strcmp(str + 2, "n")){
		fprintf(fw2,"100");
	}else if(!strcmp(str + 2,"nz")){
		fprintf(fw2,"110");
	}else if(!strcmp(str + 2,"nzp")){
		fprintf(fw2,"111");
	}else if(!strcmp(str + 2,"z")){
		fprintf(fw2,"010");
	}else if(!strcmp(str + 2,"zp")){
		fprintf(fw2,"011");
	}else if(!strcmp(str + 2,"p")){
		fprintf(fw2,"001");
	}else if(!strcmp(str + 2,"np"))
		fprintf(fw2,"011");
	fscanf(fw1,"%s",str);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(9);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(9);
			fprintf(fw2,"%s",Binary);	
		}else if(offset < 0){
			itoa(offset,str,2);
			for(int i = 0;i < 50;i++) Binary[i] = '\0';
			if(strlen(str) > 9){
				strncpy(Binary,str+strlen(str) - 9,9);
			}
			fprintf(fw2,"%s",Binary);
		}else fprintf(fw2,"000000000");
	}
}

void JMP(){
	fprintf(fw2,"1100000");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fprintf(fw2,"00000");
}

void JSR(){
	fprintf(fw2,"01001");
	fscanf(fw1,"%s",str);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(11);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(11);
			fprintf(fw2,"%s",Binary);	
		}else if(offset < 0){
			itoa(offset,str,2);
			if(strlen(str) > 11){
				strncpy(Binary,str+strlen(str) - 11,11);
			}
			fprintf(fw2,"%s",Binary);
		}else fprintf(fw2,"00000000000");
		fprintf(fw2,"BR,offset = %d",offset);
	}
}
void JSRR(){
	fprintf(fw2,"0100000");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fprintf(fw2,"000000");
}

void LD(){
	fprintf(fw2,"0010");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(9);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(9);
			printf("%s",Binary);
			fprintf(fw2,"%s",Binary);	
		}else if(offset < 0){
			itoa(offset,str,2);
			if(strlen(str) > 9){
				strncpy(Binary,str+strlen(str) - 9,9);
			}
			fprintf(fw2,"%s",Binary);
		}else fprintf(fw2,"000000000");
	}
}
void LDI(){
	fprintf(fw2,"1010");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(9);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(9);
			fprintf(fw2,"%s",Binary);	
		}else if(offset < 0){
			itoa(offset,str,2);
			if(strlen(str) > 9){
				strncpy(Binary,str+strlen(str) - 9,9);
			}
			fprintf(fw2,"%s",Binary);
		}else fprintf(fw2,"000000000");
	}
}

void LDR(){
	fprintf(fw2,"0110");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(6);
	fprintf(fw2,"%s",Binary);
}
void LEA(){
	fprintf(fw2,"1110");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	memset(Binary,'\0',sizeof(char)*50);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(9);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(9);
			printf("\nBinary = %s,str = %s\n",Binary,str);
			fprintf(fw2,"%s",Binary);
			printf("\nBinary = %s,Binary[9] = %c,str = %s\n",Binary,Binary[9],str);
		}else if(offset < 0){
			itoa(offset,str,2);
			if(strlen(str) > 9){
				strncpy(Binary,str+strlen(str) - 9,9);
			}
			fprintf(fw2,"%s",Binary);
		}else if(offset == 0)fprintf(fw2,"000000000");
	}
}

void NOT(){
	fprintf(fw2,"1001");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fprintf(fw2,"111111");
}

void RET(){
	fprintf(fw2,"1100000111000000");
}

void RTI(){
	fprintf(fw2,"1000000000000000");
}

void ST(){
	fprintf(fw2,"0011");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(9);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(9);
			fprintf(fw2,"%s",Binary);	
		}else if(offset < 0){
			itoa(offset,str,2);
			if(strlen(str) > 9){
				strncpy(Binary,str+strlen(str) - 9,9);
			}
			fprintf(fw2,"%s",Binary);
		}
	}
}
void STI(){
	fprintf(fw2,"1011");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	if((str[0] == 'x'||str[0] == 'X'||str[0] == '#')&& '0'<= str[1] && str[1] <= '9'){
		BinaryTrans(9);
		fprintf(fw2,"%s",Binary);
	}else{
		int offset = Adress() - count_instruction - 1;
		if(offset > 0){
			str[0] = '#';
			itoa(offset,str+1,10);
			BinaryTrans(9);
			fprintf(fw2,"%s",Binary);	
		}else if(offset < 0){
			itoa(offset,str,2);
			if(strlen(str) > 9){
				strncpy(Binary,str+strlen(str) - 9,9);
			}
			fprintf(fw2,"%s",Binary);
		}
	}
}
void STR(){
	fprintf(fw2,"0111");
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(3);
	fprintf(fw2,"%s",Binary);
	fscanf(fw1,"%s",str);
	BinaryTrans(6);
	fprintf(fw2,"%s",Binary);
}

void TRAP(){
	fprintf(fw2,"11110000");
	if(!strcmp(str,"HALT")){
        strcpy(str,"x25");
	}else fscanf(fw1,"%s",str);
	BinaryTrans(8);
	fprintf(fw2,"%s",Binary);
}

void FILL(){
	int i;
	char tempbinary[17]={'\0'};
	char temphex[5]={'\0'};
	memset(Operand,'\0',sizeof(char)*16);
	fscanf(fp,"%s",Operand);
	for(i=0;i<16;i++)
		if(Operand[i]==';')
			Operand[i]='\0';
	if(Operand[0]=='x'||Operand[0]=='X')
	{
		for(i=1;i<5;i++)
			if(Operand[i]<='9')
				strcat(tempbinary,HexToBinary[Operand[i]-'0']);
			else if(temphex[i]<='f'&&temphex[i]>='a')
				strcat(tempbinary,HexToBinary[Operand[i]-'a'+10]);
			else
				strcat(tempbinary,HexToBinary[Operand[i]-'A'+10]);
		count_instruction++;
		fprintf(fp1,"%s\n",tempbinary);
	}
	else if(Operand[0]=='#')
	{
		if(Operand[1]!='-')
		{
			i=atoi(Operand+1);
			sprintf(temphex,"%04x",i);
			for(i=0;i<4;i++)
				if(temphex[i]<='9')
					strcat(tempbinary,HexToBinary[temphex[i]-'0']);
				else
					strcat(tempbinary,HexToBinary[temphex[i]-'a'+10]);
			count_instruction++;
			fprintf(fp1,"%s\n",tempbinary);
		}
		else
		{
			i=atoi(Operand+2);
			sprintf(temphex,"%04x",i);
			for(i=0;i<4;i++)
				if(temphex[i]<='9')
					strcat(tempbinary,HexToBinary[temphex[i]-'0']);
				else
					strcat(tempbinary,HexToBinary[temphex[i]-'a'+10]);
			for(i=15;tempbinary[i]!='1';i--);
			while(i>=0)
			{
				i--;
				if(tempbinary[i]=='1')
					tempbinary[i]='0';
				else
					tempbinary[i]='1';
			}
			count_instruction++;
			fprintf(fp1,"%s\n",tempbinary);
		}
	}
	else
	{
		count_instruction++;
		fprintf(fp1,"%s\n",Operand);
	}
}

void STRINGZ(){
	int i;
	char temp[5];
	char tempbinary[17]={'\0'};
	memset(string1,'\0',sizeof(char)*100);
	fgets(string1,100,fp);
	for(i=0;string1[i]==' ';i++);
	for(i=i+1;string1[i]!='"';i++)
	{
		sprintf(temp,"%04x",string1[i]);
		for(int j=0;j<4;j++)
			if(temp[j]<='9')
				strcat(tempbinary,HexToBinary[temp[j]-'0']);
			else
				strcat(tempbinary,HexToBinary[temp[j]-'a'+10]);
		count_instruction++;
		fprintf(fp1,"%s\n",tempbinary);
		memset(temp,'\0',sizeof(char)*5);
		memset(tempbinary,'\0',sizeof(char)*17);
	}
	fprintf(fp1,"0000000000000000\n");
	count_instruction++;
}

void BLKW(void)
{
	int space;
	fscanf(fp,"%d",&space);
	fgets(annotation,100,fp);
	for(int i=0;i<space;i++)
	{
		fprintf(fp1,"0000000000000000\n");
		count_instruction++;
	}
}

void GETC(void)
{
	fprintf(fp1,"1111000000100000\n");
	count_instruction++;
}

void OUT(void)
{
	fprintf(fp1,"1111000000100001\n");
	count_instruction++;
}

void PUTS(void)
{
	fprintf(fp1,"1111000000100010\n");
	count_instruction++;
}

void IN(void)
{
	fprintf(fp1,"1111000000100011\n");
	count_instruction++;
}

void PUTSP(void)
{
	fprintf(fp1,"1111000000100100\n");
	count_instruction++;
}

void END(void)
{
	memset(Operator,'\0',sizeof(char)*6);
    memset(LABEL,'\0',sizeof(char)*50);
    memset(Operand,'\0',sizeof(char)*16);
	Stopsign=1;
}

void ChooseOperator(){
	if(!strcmp(str,"AND")) AND();
	else if(!strcmp(str,"ADD")) ADD();
	else if(!strcmp(str,"LD")) LD();
	else if(!strcmp(str,"LDI")) LDI();
	else if(!strcmp(str,"LEA")) LEA();
	else if(!strcmp(str,"LDR")) LDR();
	else if(!strcmp(str,"ST")) ST();
	else if(!strcmp(str,"STI")) STI();
	else if(!strcmp(str,"STR")) STR();
	else if(!strcmp(str,"JMP")) JMP();
	else if(!strcmp(str,"JSRR")) JSRR();
	else if(!strcmp(str,"JSR")) JSR();
	else if(!strcmp(str,"TRAP")) TRAP();
	else if(!strcmp(str,"HALT")) TRAP();
	else if(!strcmp(str,"NOT")) NOT();
	else if(!strcmp(str,"RET")) RET();
	else if(!strcmp(str,"RTI")) RTI();
	else if(str[0] == 'B' && str[1] =='R') BR();
	else{
		if(str[0] =='0' || str[0] == '1'){
			fprintf(fw2,"%s",str);
		}else{
			int n = Adress() + START;
			TranToBinary(n);
			fprintf(fw2,"%s",Binary);	
			printf("\nIn ChooseOperator,str =  %s\n",str);		
		}
	}
}

void ChoosePseudoops(){
	if(!strcmp(Pseudo_ops,".FILL")) FILL();
	if(!strcmp(Pseudo_ops,".STRINGZ")) STRINGZ();
	if(!strcmp(Pseudo_ops,".END")) END();
	if(!strcmp(Pseudo_ops,".BLKW")) BLKW();
	if(!strcmp(Pseudo_ops,"GETC")) GETC();
	if(!strcmp(Pseudo_ops,"OUT")) OUT();
	if(!strcmp(Pseudo_ops,"PUTS")) PUTS();
	if(!strcmp(Pseudo_ops,"IN")) IN();
	if(!strcmp(Pseudo_ops,"PUTSP")) PUTSP();
}


int main(){
	FILE *f;
    FILE *f1;
    char arrow[120]={'\0'};
    char String[120]={'\0'};
    int l;
    f=fopen("C://Users//13501//DeskTop//1.txt","r");
    f1=fopen("C://Users//13501//DeskTop//Middle0.txt","w");
    while(fgets(arrow,120,f)!=NULL)
    {
        for(l=0;arrow[l]!='\n'&&arrow[l]!=';';l++)
            if(arrow[l]!=' '&&arrow[l]!='\t'){
            	break;
			}
        if(arrow[l]=='\n'||arrow[l]==';')
            continue;
        for(l=0;arrow[l]!='\n';l++)
        { 
            if(arrow[l]==';')
            {
                strncpy(String,arrow,l);
                if(strstr(String,".STRINGZ")==NULL)
                {
                	//puts(String);
                    arrow[l]='\0';
                    fprintf(f1,"%s\n",arrow);
                    //printf("goto\n");
                    goto last1;
                }
                else
                    continue;
            }
        }
		if(strstr(arrow,".END")!=NULL)
			existend=1;
        fprintf(f1,"%s",arrow);
        last1:memset(arrow,'\0',sizeof(char)*120);
    }
	if(!existend)
	{
		printf("Error:Expected .END at end of file.\n");
		return 0;
	}
    fclose(f);
    fclose(f1);
	

	fp=fopen("Middle0.txt","r");
    fp1=fopen("Middle.txt","w");
    fscanf(fp,"%s",LABEL);
    if(strcmp(LABEL,".ORIG")){
        printf("ERROR!\nExpected .ORIG but found '%s' instead.",LABEL);
        return 0;
    }
    fscanf(fp,"%s",str);
    fprintf(fp1,"%s\n",str);
    START=strtol(str+1,NULL,16);
    while(!Stopsign)
    {
		memset(Operator,'\0',sizeof(char)*6);
		memset(LABEL,'\0',sizeof(char)*50);
		memset(Operand,'\0',sizeof(char)*16);
		fscanf(fp,"%s",LABEL);
		strncpy(CheckHalt,LABEL,4);
		if(!strcmp(CheckHalt,"HALT"))
		{
			fprintf(fp1,"HALT\n");
			goto last;
		}
        if(!CheckIfOperator()&&!CheckIfPseudoops())
		{
			strcpy(Labels[count_label],LABEL);
			Label_Address[count_label]=count_instruction;
			count_label++;
			memset(LABEL,'\0',sizeof(char)*21);
			goto last;
		}
		if(CheckIfOperator())
		{
			strcpy(Operator,LABEL);
			if(!strcmp(Operator,"HALT"))
			{
				fprintf(fp1,"%s %s\n",Operator,Operand);
				goto last;
			}
			if((c=fgetc(fp))=='\n')
			{
				fprintf(fp1,"%s %s\n",Operator,Operand);
				goto last;  
			}
			while((c=fgetc(fp))==' '||c=='\t')
				continue;
			Operand[count_operand]=c;
			count_operand++;
			while((c=fgetc(fp))!='\n')
			{
				if(c==';')
				{
					fgets(annotation,100,fp);
					break;
				}
				if(c!=' '&&c!='\t')
				{
					if(c==',')
					{
						Operand[count_operand]=' ';
						count_operand++;
						checkerror++;
						if((!strcmp(Operator,"NOT")&&checkerror>1)||checkerror>2)
						{
							printf("Error:Illegal operand format!\n");
							stopprogram=1;
						}
					}
					else
					{
						Operand[count_operand]=c;
						count_operand++;
					}
				}
				else   
					continue;
			}
		}
		else if(CheckIfPseudoops())
		{
			strcpy(Pseudo_ops,LABEL);
			ChoosePseudoops();
			memset(Operator,'\0',sizeof(char)*6);
			memset(LABEL,'\0',sizeof(char)*50);
			memset(Operand,'\0',sizeof(char)*16);
			continue;
		}
		Operand[count_operand]='\0';
		count_operand=0;
		fprintf(fp1,"%s %s\n",Operator,Operand);
		count_instruction++;
		checkerror=0;
		last:continue;
    }
    fclose(fp);
    fclose(fp1);
    for(int k=0;k<count_label;k++)
		if(Labels[k][0]>='0'&&Labels[k][0]<='9')
		{
			printf("Illegal label name!\n");
			stopprogram=1;
		}
	if(stopprogram)
		return 0;
    fw1 = fopen("C://Users//13501//DeskTop//Middle.txt","r");
    fw2 = fopen("C://Users//13501//DeskTop//Machinelanguage.txt","w");
    count_instruction = 0;
    fscanf(fw1,"%s",str);
    BinaryTrans(16);
	//fprintf(fp1,"%s\n",Binary);
	fprintf(fw2,"%s\n",Binary);
    while(fscanf(fw1,"%s",str)!=EOF){
		printf("str = %s\n",str);
    	ChooseOperator();
    	fprintf(fw2,"\n");
    	count_instruction++;
		if(stopprogram)
			break;
    }
    fclose(fw1);
    fclose(fw2);
	return 0;
}


void TranToBinary(int num)
{
	char temphex[5]={'\0'};
	for(int i = 0;i < 17;i++) Binary[i] = '\0';
	if(num > 0)
	{
		sprintf(temphex,"%04x",num);
		strncpy(temphex,temphex+strlen(temphex) - 4,4);
		memset(Binary,'\0',sizeof(char)*50);
		for(int i=0;i<4;i++)
			if(temphex[i]<='9')
				strcat(Binary,HexToBinary[temphex[i]-'0']);
			else
				strcat(Binary,HexToBinary[temphex[i]-'a'+10]);
	}
	else
	{
		sprintf(temphex,"%04x",num);
		strncpy(temphex,temphex+strlen(temphex) - 4,4);
		memset(Binary,'\0',sizeof(char)*50);
		for(int i=0;i<4;i++){
			if(temphex[i]<='9')
				strcat(Binary,HexToBinary[temphex[i]-'0']);
			else
				strcat(Binary,HexToBinary[temphex[i]-'a'+10]);
		}
	}
}

void BinaryTrans(int t){
	int num;
	num = atoi(str + 1);
	if(num < 0){
		itoa(num,str,2);
		if(strlen(str) > t){
			memset(Binary,'\0',sizeof(char)*50);
			strncpy(Binary,str+strlen(str) - t,t);
		}
	}else{
	if(str[0] == 'x'||str[0] == 'X'){
		int num1 = 0;
		int k = 0;
		while(num){
			num1 += (num%10)*(pow(16,k));
			num = num/10;
			k++;
		}
		num = num1;
	}
	itoa(num,Binary,2);	
	int num2 = 0;
	int len = strlen(Binary);
	if(len < t){
		char temp[20];
		strcpy(temp,Binary);
		int k = 0;
		while(len < t){
			Binary[k] = '0';
			k++;
			len++;
		}
		strcpy(Binary+k,temp);
	}
}
}
int Adress(){
	int i = 0;
	while(i < count_label && strcmp(Labels[i],str)) i++;
	if(i < count_label) return Label_Address[i];
	return 0;
}
