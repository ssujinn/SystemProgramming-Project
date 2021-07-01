/* 포함되는 파일 */
#include "20171640.h"

/* 프로그램 시작*/
int main() {
	// oricmd : 처음 입력받은 커맨드
	// cmd : 명령어 부분만 잘라낸 char 배열
	// idx : 명령어 뒷부분만 잘라낸 char 배열
	char oricmd[100], cmd[100], idx[100];

	makeOPlist();	// opcode list로 hash table 생성

	while (1) {
		printf("sicsim> ");

		// local 변수 초기화
		memset(oricmd, '\0', sizeof(oricmd));
		memset(cmd, '\0', sizeof(cmd));
		memset(idx, '\0', sizeof(idx));

		// 커맨드 입력받아 저장
		fgets(oricmd, sizeof(oricmd), stdin);

		// cmd와 idx로 분리
		editcmd(oricmd, cmd, idx);

		// 명령어가 "h" 또는 "help" 일 때
		if (!strcmp(cmd, "help") || !strcmp(cmd, "h")) {
			if (idx[0] == '\0'){	// idx가 비었으면
				add_list(oricmd);	// history에 추가
				help();		// help 실행
			}
			else	// idx가 비어있지 않으면
				printf("Invalid Command\n");	// 에러 출력
		}
		// 명령어가 "d" 또는 "dir" 일 때
		else if (!strcmp(cmd, "d") || !strcmp(cmd, "dir")) {
			if (idx[0] == '\0'){	// idx가 비었으면
				add_list(oricmd);	// history에 추가
				dir();	// dir 실행
			}
			else	// idx가 비어있지 않으면
				printf("Invalid Command\n");	// 에러 출력
		}
		// 명령어가 "q" 또는 "quit" 일 때
		else if (!strcmp(cmd, "q") || !strcmp(cmd, "quit")) {
			if (idx[0] == '\0')	// idx가 비었으면
				break;		// 루프문 종료
			else			// 비어있지 않으면
				printf("Invalid Command\n");	// 에러 출력
		}
		// 명령어가 "hi" 또는 "history" 일 때
		else if (!strcmp(cmd, "hi") || !strcmp(cmd, "history")) {
			if (idx[0] == '\0'){	// idx가 비었으면
				add_list(oricmd);	// history에 추가
				history();		// history 실행
			}
			else				// 그렇지 않으면
				printf("Invalid Command\n");	// 에러 출력
		}
		// 명령어가 "du" 또는 "dump" 일 때
		else if (!strcmp(cmd, "du") || !strcmp(cmd, "dump")){
			if (dump(idx) == 1)	// dump() return 값이 1이면
				add_list(oricmd);	// history 추가
		}
		// 명령어가 "e" 또는 "edit" 일 때
		else if (!strcmp(cmd, "e") || !strcmp(cmd, "edit")){
			if (edit(idx) == 1)		// edit() return 값이 1이면
				add_list(oricmd);	// history 추가
		}
		// 명령어가 "f" 또는 "fill" 일 때
		else if (!strcmp(cmd, "f") || !strcmp(cmd, "fill")){
			if (fill(idx) == 1)		// fill() return 값이 1이면
				add_list(oricmd);	// history에 추가
		}
		// 명령어가 "reset" 일 때
		else if (!strcmp(cmd, "reset")){	
			if (idx[0] == '\0'){		// idx가 비었으면
				add_list(oricmd);	// history에 추가
				reset();		// reset 실행
			}
			else				// 그렇지 않으면
				printf("Invalid Command\n");	// 에러 출력

		}
		// 명령어가 "opcode" 일 때
		else if (!strcmp(cmd, "opcode")){	
			if (opcode(idx) == 1)		// opcode() return 값이 1이면
				add_list(oricmd);	// history에 추가
		}
		// 명령어가 "opcodelist" 일 때
		else if (!strcmp(cmd, "opcodelist")){
			if (idx[0] == '\0'){		// idx가 비었으면
				add_list(oricmd);	// history에 추가
				opcodelist();		// opcodelist 실행
			}
			else				// 그렇지 않으면
				printf("Invalid Command\n");	// 에러 출력
		}
		// 명령어가 "type" 일 때
		else if (!strcmp(cmd, "type")){
			if (type(idx) == 1)		// type() return이 1이면
				add_list(oricmd);	// history에 추가
		}
		// 명령어가 "assemble" 일 때
		else if (!strcmp(cmd, "assemble")){
			if (assemble(idx) ==1)		// assemble() return이 1이면
				add_list(oricmd);	// history에 추가
		}
		// 명령어가 "symbol" 일 때
		else if (!strcmp(cmd, "symbol")) {
			if (idx[0] == '\0'){	// idx가 비었으면
				if (symbolcmd() == 1)
					add_list(oricmd);
			}
			else			// 비어있지 않으면
				printf("Invalid Command\n");	// 에러 출력
		}
		// 그 이외의 입력 에러 처리
		else
			printf("Invalid Command\n");
	}

	// linked list dellocate를 위한 local 변수
	hiList *hifree, *tmp1;
	OPlist *opfree, *tmp2;
	int i;

	// history linked list dellocate
	hifree = hiHead;
	while (hifree != NULL){
		tmp1 = hifree;
		hifree = hifree -> link;
		free(tmp1);
	}

	// Opcodelist linked list dellocate
	for (i = 0 ; i < 20 ; i++){
		opfree = OPhead[i];
		while (opfree != NULL){
			tmp2 = opfree;
			opfree = opfree->link;
			free(tmp2);
		}
	}

	// free symbol table
	symbol *symptr, *symrmv;
	for (i = 0; i < 26; i++){
		symptr = symhead[i];
		while (symptr){
			symrmv = symptr;
			symptr = symptr -> link;
			free(symrmv);
		}
		symhead[i] = NULL;
	}

	// free objcode list
	objlist *obptr, *obrmv;
	obptr = objhead;
	while (obptr){
		obrmv = obptr;
		obptr = obptr -> link;
		free(obrmv);
	}

	return 0;
}

/* 사용되는 function */

/* start of proj 1 */

void editcmd(char* cmd, char* new, char* idx) {
	int i = 0, j = 0;	// 반복분과 인덱스에 사용하는 임시 변수

	cmd[(unsigned)strlen(cmd) - 1] = '\0';	// 커맨드의 '\n'을 '\0'으로 치환

	while (cmd[i] == ' ' || cmd[i] == '\t')			
	// i에 처음으로 공백이 아닌 값의 인덱스 저장
		i++;

	while (cmd[i] != ' ' && cmd[i] != '\0' && cmd[i] != '\t'){
	// 공백 또는 NULL 값이 나오기 전까지의 값을 new에 저장(명령어 부분만 저장)
		new[j] = cmd[i];
		i++;
		j++;
	}

	if (cmd[i] != '\0'){	// 명령어 뒤에 파라미터가 더 있을 때
		j = 0;
		while (cmd[i] == ' ' || cmd[i] == '\t')	
		// i에 처음으로 공백이 아닌 값의 인덱스를 저장
			i++;
		while (cmd[i] != '\0'){	// idx에 NULL이 나오기 전까지 저장
			idx[j] = cmd[i];
			i++;
			j++;
		}
	}
}

void help() {
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
}

void dir() {
	DIR *ptrDir;
	struct dirent *ptrFile;
	struct stat fileStat;
	int num = 0;

	ptrDir = opendir(".");		// 현재 디렉토리 포인터 불러오기
	while ((ptrFile = readdir(ptrDir)) != NULL){	// 디렉토리 내의 파일
		stat(ptrFile->d_name, &fileStat);	
		printf("\t%s", ptrFile->d_name);	// 파일 읽어서 출력
		if (S_ISDIR(fileStat.st_mode))	
			printf("/");			// 파일이 디렉토리이면 '/' 붙이기
		else if (S_IXUSR & fileStat.st_mode)
			printf("*");			// 파일이 실행파일이면 '*' 붙이기
		num++;
		if (num % 4 == 0)
			printf("\n");			// 4개 파일 출력되면 줄바꿈
		}
	if (num % 4 != 0)
	printf("\n");
	closedir(ptrDir);	// 디렉토리 포인터 닫기
}

void history() {
	hiList *ptr;		// linked list를 탐색하기 위한 임시변수
	int num = 1;		// 줄 번호 나타낼 정수
	
	ptr = hiHead;

	while (ptr != NULL) {		
	// ptr이 NULL 하기 전까지 linked list 탐색하며 출력
		printf("        %-5d%s\n", num, ptr->cmd_name);
		num++;
		ptr = ptr->link;
	}
}

void add_list(char* cmd) {
	hiList *newNode = (hiList*)malloc(sizeof(hiList)); // 새로운 값을 저장할 노드
	hiList *ptr = hiHead;	// linked list 탐색에 사용되는 임시변수

	strcpy(newNode->cmd_name, cmd);
	newNode->link = NULL;
	// 새 노드에 입력받은 커맨드 저장, link 값은 아직 없으니 NULL

	if (hiHead == NULL) {	// head가 NULL 이면
		hiHead = (hiList*)malloc(sizeof(hiList));
		hiHead = newNode;
		// head 할당한 후 새 노드 저장
	}
	else {	// head가 NULL하지 않으면
		while (ptr->link != NULL)
			ptr = ptr->link;	// 새 노드를 연결할 노드까지 이동
		ptr-> link = newNode;		// 새 노드 연결
	}
}

int dump(char* idx){
	int cmflag = -1; // 콤마의 위치 인덱스
	int i = 0, j;	// 인덱스와 반복문에 사용할 변수
	int start, end;	// 메모리 시작 값과 마지막 값을 저장할 변수
	char tmp[100];	
	// 입력값이 올바르지 않음을 판단할 때 사용할 임시 변수
	int adflag = 0; // dump 뒤에 인자가 없을 때 1로 set(마지막 address 저장)

	while (idx[i] != '\0'){
		if (idx[i] == ','){
			cmflag = i;
			break;
		}
		i++;
	} // cmflag : 콤마의 위치

	if (cmflag >= 0){ // 콤마가 있을 경우
		if (sscanf(idx, "%x \t , \t %x %s", &start, &end, tmp) != 2){ 
		// 파라미터가 두개 존재하지 않는 경우
		// 두 파라미터가 16진수가 아닌 경우 에러
			printf("Invalid parameter\n");
			return 0;
		}
	}
	else { // 콤마가 없을 경우
		if (idx[0] == '\0'){ // dump 뒤로 들어온 파라미터가 없으면
			start = dumpend + 1;
			end = start + 159;
		}
		else { // dump 뒤로 들어온 파라미터가 있을 때
			if (sscanf(idx, "%x %s", &start, tmp) != 1){
			// 파라미터가 하나 존재하지 않는 경우
			// 파라미터가 16진수가 아닌 경우 에러
				printf("Invalid parameter\n");
				return 0;
			}
			end = start + 159;
		}
	}

	if (end > 0xFFFFF) // end 범위가 최대값을 벗어나면 최대값으로 저장
		end = 0XFFFFF;

	if (start < 0 || end < 0 || start > 0xFFFFF || end > 0xFFFFF || start > end){
	// start나 end가 허용된 범위안에 있지 않으면 에러
		printf("Address is out of range\n");
		return 0;
	}

	for (i = start / 16 * 16; i <= end / 16 * 16; i+= 16){
		printf("%05X ", i);	// address 출력 (왼쪽 칼럼)
		for (j = i; j < i + 16; j++){	
		// 메모리 값 16진수로 표현 (가운데 칼럼)
			if (j >= start && j <= end)
				printf("%02X ", memory[j]);
			else
				printf("   ");
		}

		printf("; ");

		for (j = i; j < i + 16; j++){	
		// 메모리 값 ASCII 값으로 표현 (오른쪽 칼럼)
			if(j >= start && j <= end && memory[j] >= 0X20 && memory[j] <= 0x7E)
			// 20에서 7E 까지의 값은 ASCII 값으로 출력한다
				putchar(memory[j]);
			else
			// 그 이외의 값은 '.' 으로 출력한다
				putchar('.');
		}
		printf("\n");
	}

	if (adflag == 1)	// dump 뒤에 인자가 없으면
		dumpend = end;	// 마지막으로 저장된 end값 global var에 저장

	if (dumpend >= 0xFFFFF)	// 저장된 global var이 최댓값을 넘어가면 초기화
		dumpend = -1;

	return 1;
}

int edit(char* idx){
	int cmflag = -1;	// 콤마 인덱스 값
	int i = 0;		// 인덱스나 반복문에 사용할 변수
	int adr, val;		// 주소값과 메모리값을 저장할 변수
	char tmp[100];		// 임시 변수

	while (idx[i] != '\0'){
		if (idx[i] == ','){
			cmflag = i;
			break;
		}
		i++;
	} // cmflag : 콤마의 위치

	if (cmflag == -1){ // 콤마가 없으면 에러
		printf("Invalid parameter\n");
		return 0;
	}
	else{	// 콤마가 있으면
		if (sscanf(idx, "%x \t , \t %x %s", &adr, &val, tmp) != 2){
		// 파라미터 개수가 2개가 아니거나
		// 들어온 파라미터가 16진수가 아니면 에러
			printf("Invalid parameter\n");
			return 0;
		}
	}

	if (adr < 0x00000 || adr > 0xFFFFF){
	// 주소값 범위가 허용된 범위를 벗어나면 에러
		printf("Address is out of range\n");
		return 0;
	}

	if (val > 0xFF || val < 0){
	// value 값이 허용된 범위를 벗어나면 에러
		printf("Value is out of range\n");
		return 0;
	}

	memory[adr] = val;	// 메모리에 입력된 16진수 값 저장

	return 1;
}

int fill(char* idx){
	int cm1 = -1, cm2 = -1;		// 콤마 인덱스 값
	int i = 0;			// 변수, 반복문에 쓰이는 변수
	int start, end, value;		// 파라미터로 들어온 start, end, value 값
	char tmp[100];			// 임시 변수

	while (idx[i] != '\0'){		// 첫번째 콤마 탐색
		if (idx[i] == ','){
			cm1 = i;
			break;
		}
		i++;
	}
	i++;
	while (idx[i] != '\0'){		// 두번째 콤마 탐색
		if (idx[i] == ','){
			cm2 = i;
			break;
		}
		i++;
	}

	if (cm1 == -1 || cm2 == -1){	// 콤마가 2개가 아니면 에러
		printf("Invalid parameter\n");
		return 0;
	}

	if (sscanf(idx, "%x \t , \t %x \t , \t %x %s", &start, &end, &value, tmp) != 3){
	// 16진수인 파라미터 값이 3개가 아닐경우 에러
		printf("Invalid parameter\n");
		return 0;
	}

	if (start < 0 || end < 0 || start > 0xFFFFF || end > 0xFFFFF || start > end){
	// start와 end 값이 범위 내에 없을 경우 에러
		printf("Address is out of range\n");
		return 0;
	}
	
	if (value > 0xFF || value < 0){
	// value 값이 허용된 범위를 벗어나면 에러
		printf("Value is out of range\n");
		return 0;
	}

	for (i = start; i <= end; i++)
		memory[i] = value;	// 메모리에 입력받은 값 저장

	return 1;
}

void reset(){
	memset(memory, 0, sizeof(memory));	// 메모리 값 0으로 초기화
}

void makeOPlist(){
	FILE *fp = fopen("opcode.txt", "r");	// 파일 포인터에 읽기전용으로 로드
	char inst[10], form[5];
	int op, tmp;

	// 파일포인터가 비었으면 에러 출력하고 종료
	if (fp == NULL){
		printf("Opcode list file is NULL\n");
		return;
	}
	
	while (1){
		if (fscanf(fp, "%x %s %s", &op, inst, form) == EOF) // 파일 정보 저장
			break;	// 더이상 읽을 줄이 없으면 탐색 중단

		// 읽어들인 정보를 새로운 노드에 저장
		OPlist *newNode = (OPlist*)malloc(sizeof(OPlist));
		newNode->opcode = op;
		strcpy(newNode->inst_name, inst);
		strcpy(newNode->form, form);
		newNode->link = NULL;

		// 명령어를 hash function을 이용하여 암호화된 값으로 변환한다.
		tmp = hashFunction(inst);
		
		// 새 노드를 linked list에 연결
		if (OPhead[tmp] == NULL){	// 해당 인덱스의 헤드가 NULL이면
			OPhead[tmp] = newNode;	// 제일 첫 노드에 저장
		}
		else {					// 그렇지 않으면
			OPlist *ptr = OPhead[tmp];
			while (ptr->link != NULL)
				ptr = ptr->link;
			ptr->link = newNode;		// 가장 뒤에 있는 노드 뒤에 연결
		}
	}
	fclose(fp);	// 파일 포인터 닫기
}

int hashFunction(char* inst){
	int i;
	int len = strlen(inst);

	// 명령어를 읽어서 정수형으로 암호화 한다
	i = inst[0] + inst[len - 1] + len;

	return i % 20;
}

int opcode(char* idx){
	char tmp[100];	// 임시 변수
	char inst[10];	// 오피코드 검색할 명령어
	int i = -1;	// 임시 변수
	OPlist *ptr;	// linked list 탐색할 때 사용하는 변수

	// idx에서 명령어 읽어온다
	// 파라미터가 1개가 아니면 에러를 출력한다
	if (sscanf(idx, "%s %s", inst, tmp) != 1){
		printf("Invalid mnemonic\n");
		return 0;
	}

	i = hashFunction(inst); // hash function으로 암호화
	ptr = OPhead[i];	// 암호화된 인덱스로 linked list 탐색

	// linked list에서 입력된 instruction과 같은 mnemonic을 찾는다
	while (ptr != NULL){
		if (!strcmp(ptr->inst_name, inst))
			break;
		ptr = ptr->link;
	}

	// 해당하는 mnemonic이 없으면 에러 처리
	if (ptr == NULL){
		printf("Invalid mnemonic\n");
		return 0;
	}

	// 찾으면 결과 출력
	printf("opcode is %02X\n", ptr->opcode);
		
	return 1;
}

void opcodelist(){
	int i;		// 반복문에 사용할 변수
	OPlist *ptr;	// linked list 탐색할 포인터

	for (i = 0; i < 20; i++){	// 모든 index 탐색
		printf("%d : ", i);
		ptr = OPhead[i];

		// 한 인덱스에서 연결된 모든 linked list 출력
		if (ptr){
			while (ptr -> link != NULL){
				printf("[%s,%02X] -> ", ptr->inst_name, ptr->opcode);
				ptr = ptr->link;
			}
			printf("[%s,%02X]", ptr->inst_name, ptr->opcode);
		}
		printf("\n");
	}
}

/* end of proj 1 */

/* start of proj 2 */

void getfilename(char *str){
	int i = 0, j;		// i, j : 배열의 인덱스를 표현
	int flag = 0;		// 공백을 제거할지 안할지 나타내는 플래그

	// 들어온 string의 가장 끝에 의미없는 공백이 있는지 확인
	while (str[i] != '\0'){
		if (flag == 0)	{
			if (str[i] == ' ' || str[i] == '\t'){
				flag = 1;
				j = i;
			}
		}
		else if (flag == 1){
			if (str[i] != ' ' && str[i] != '\t')
				flag = 0;
		}

		i++;
	}

	// 공백을 제거한다
	if (flag == 1){
		while (str[j] != '\0'){
			str[j] = '\0';
			j++;
		}
	}
}

int type(char *f_name){
	FILE *fp;
	char tmp;

	// 파일 열기
	getfilename(f_name);
	fp = fopen(f_name, "r");

	// 파일 없으면 종료
	if (!fp) {
		printf("No such file in this directory\n");
		return 0;
	}

	// 파일 내용 출력
	while (1) {
		tmp = fgetc(fp);
		if (tmp == EOF)
			break;
		printf("%c", tmp);
	}

	//파일 닫기
	fclose(fp);

	return 1;
}

int assemble(char *f_name){
	FILE *fp;
	int i = 0;
	char name_tmp[100];
	char filetmp[100];
	symbol *ptr, *rmv;
	objlist *obptr, *obrmv;

	memset(filetmp,'\0',sizeof(filetmp));
	
	// free symbol table
	for (i = 0; i < 26; i++){
		ptr = symhead[i];
		while (ptr){
			rmv = ptr;
			ptr = ptr -> link;
			free(rmv);
		}
		symhead[i] = NULL;
	}
	symflag = 0;

	// free objcode list
	obptr = objhead;
	while (obptr){
		obrmv = obptr;
		obptr = obptr -> link;
		free(obrmv);
	}
	objhead = NULL;

	// 파일 열기
	getfilename(f_name);
	fp = fopen(f_name, "r");

	// 파일이 없으면 에러 출력 후 종료
	if (!fp) {
		printf("No such file in this directory\n");
		return 0;
	}

	// .asm 파일인지 확인
	i = 0;
	while (1){
		if (f_name[i] == '.')	// i는 '.'이 찍힌 인덱스
			break;
		name_tmp[i] = f_name[i];	// '.' 이 나오기 전까지 파일 이름 저장
		i++;
	}
	i++;
	if (strcmp(f_name+i, "asm")) {			// . 다음에 asm이 아니면
		printf("Invalid file extension\n"); 	// 에러 메세지 출력 후
		return 0;				// 종료
	}
	
	// 패스1, 패스2 에서 오류 발생했으면 파일 삭제하고 종료
	if (pass_1(f_name) == 0){
		strcpy(filetmp, name_tmp);
		strcat(filetmp, ".lst");
		remove(filetmp);
		memset(filetmp, '\0', sizeof(filetmp));
		strcpy(filetmp, name_tmp);
		strcat(filetmp, ".obj");
		remove(filetmp);
		remove("intermediate");
		return 0;
	}

	remove("intermediate");

	// 어셈블 정상작동 되면 생성된 파일명 출력
	printf("Successfully assemble %s.\n", f_name);
	symflag = 1;

	// 파일 닫고 종료
	fclose(fp);
	return 1;
}

int pass_1(char *f_name){
	int locctr = 0;
	FILE *src = fopen(f_name, "r");
	FILE *intmd = fopen("intermediate", "w");
	char line[200];
	char s1[30], s2[30], s3[30], s4[30], tmpstr[30];
	int lnum = 5, cmflag = -1;
	int i, k;
	adr_start = 0;
	pro_length = 0;
	int tmp = 0, dectmp;
	int endflag = 0;
	symbol *symptr, *pre, *cur;
	OPlist *opptr;
	int idx;
	
	// 한 줄 읽어오기
	fgets(line, sizeof(line), src);
	// 받아온 줄에서 인자 받기
	tmp = sscanf(line, "%s %s %x %s", s1, s2, &adr_start, tmpstr);
	if (tmp){
		// START 있으면
		if (!strcmp(s2, "START")){
			// 정상 형식일 때 START 라인 출력
			if (tmp == 3){
				locctr = adr_start;
				fprintf(intmd, "%d\t%d\t", lnum, locctr);
				lnum += 5;
				fprintf(intmd, "%s", line);
			}
			else {
				printf("%d: error: start line error\n", lnum);
				return 0;
			}
		}
		// START 없으면 파일 닫고 다시 열기
		else {
			fclose(src);
			src = fopen(f_name, "r");
		}
	}
	memset(line, '\0', sizeof(line));

	// 파읽 끝에 도달할 때 까지 읽기
	while (fgets(line, sizeof(line), src) != NULL){

		// 초기화
		memset(s1, '\0', sizeof(s1));
		memset(s2, '\0', sizeof(s2));
		memset(s3, '\0', sizeof(s3));
		memset(s4, '\0', sizeof(s4));
		memset(tmpstr, '\0', sizeof(tmpstr));
		tmp = 0;
		cmflag = -1;

		// 주석이면 그대로 출력
		if (line[0] == '.'){
			fprintf(intmd, "%d\t%s", lnum, line);
			lnum += 5;
		}
		// 주석이 아니면
		else {
			// 콤마 있는지 확인
			i = 0;
			while (line[i] != '\0'){
				if (line[i] == ','){
					cmflag = i;
					break;
				}
				i++;
			} // cmflag : 콤마의 위치

			// 입력 받은 라인에서 인자로 값 할당
			// label이 안들어온 경우
			if (line[0] == ' ' || line[0] == '\t'){
				// 콤마가 있으면
				if (cmflag != -1){
					tmp = sscanf(line, "%s \t %s \t %s \t %s", s2, s3, s4, tmpstr);
					if (line[cmflag+1] != ' ' && line[cmflag+1] != '\t'){
						if (line[cmflag -1] != ' ' && line[cmflag -1] != '\t'){
						k = 0;
						while (s3[k] != ',')
							k++;
						k++;
						strcpy(s4, s3+k);
						tmp++;
						}
					}
					else if (s4[0] == ','){
						if (strlen(s4) == 1){
							strcpy(s4, tmpstr);
							tmp--;
						}
						else{
							strcpy(s4, s4+1);
							s4[strlen(s4) - 1] = '\0';
						}
					}

					if (tmp != 3){
						printf("%d: error: wrong input line\n", lnum);
						return 0;
					}
					tmp--;
				}
				// 콤마가 없으면
				else {
					tmp = sscanf(line, "%s \t %s \t %s", s2, s3, tmpstr);
					if (tmp != 2) {
						if (tmp == 1 && !strcmp(s2, "RSUB"))
							tmp = 1;
						else {
							printf("%d: error: wrong input line\n", lnum);
							return 0;
						}
					}
				}
			}
			// 에러처리
			else if (line[0] <= '9' && line[0] >= '0') {
				printf("%d: error: first char cannot be number\n", lnum);
				return 0;
			}
			// label이 들어온 경우
			else {
				// 콤마가 있으
				if (cmflag != -1){
					tmp = sscanf(line, "%s \t %s \t %s \t %s \t %s", s1, s2, s3, s4, tmpstr);
					if (line[cmflag+1] != ' ' && line[cmflag+1] != '\t'){
						if(line[cmflag-1] != ' ' && line[cmflag-1] != '\t'){
						k = 0;
						while (s3[k] != ',')
							k++;
						k++;
						strcpy(s4, s3+k);
						tmp++;
						}
					}
					else if (s4[0] == ','){
						if (strlen(s4) == 1){
							strcpy(s4, tmpstr);
							tmp--;
						}
						else{
							strcpy(s4, s4+1);
							s4[strlen(s4) - 1] = '\0';
						}
					}

					if (tmp != 4){
						printf("%d: error: wrong input line\n", lnum);
						return 0;
					}
					tmp--;
				}
				// 콤마가 없으면
				else {
					tmp = sscanf(line, "%s \t %s \t %s \t %s", s1, s2, s3, tmpstr);
					if (tmp != 3){
						printf("%d: error: wrong input line\n", lnum);
						return 0;
					}
				}
			}

			/* s1 : label 
  			   s2 : directive 
			   s3 : operand 
			   s4 : second operand after comma */

			// label 이 있을 때 
			if (s1[0] != '\0'){
				// sybol table에서 label 찾기
				idx = ZtoAhash(s1);
				symptr = symhead[idx];
				while (symptr){
					if (!strcmp(symptr->name, s1))
						break;
					symptr = symptr -> link;
				}

				// label 있으면 중복 에러
				if (symptr){
					printf("%d: error: duplicate symbol - %s\n", lnum, s1);
					return 0;
				}
				// 없으면 symbol table에 추가
				else {
					// 새 노드에 값 할당
					symbol *newNode = (symbol*)malloc(sizeof(symbol));
					strcpy(newNode->name, s1);
					newNode->link = NULL;
					newNode->locctr = locctr;

					// head가 비었으면 head가 새 노드
					if (symhead[idx] == NULL)
						symhead[idx] = newNode;
					// head가 NULL이 아니면
					else {
						pre = symhead[idx];
						cur = pre;

						// Z to A 순으로 linked list 저장
						while (cur){
							k = 0;
							while (1){
								if (cur->name[k] != s1[k])	
								break;
								k++;
							}
							if (cur->name[k] < s1[k])
								break;

							pre = cur;
							cur = cur -> link;
						}
						// 올바른 위치에 new node 삽입
						if (cur == pre){
							symhead[idx] = newNode;
							symhead[idx]->link = cur;
						}
						else{
							pre->link = newNode;
							newNode->link = cur;
						}
					}
				}
			}

			// directive가 opcode list에 있는지 확인
			if (s2[0] == '+')
				idx = hashFunction(s2+1);
			else
				idx = hashFunction(s2);
			opptr = OPhead[idx];
			while (opptr){
				if (s2[0] != '+'){
					if (!strcmp(opptr->inst_name, s2))
						break;
				}
				else{
					if (!strcmp(opptr->inst_name, s2+1))
						break;
				}
				opptr = opptr -> link;
			}

			// opcode list에 directive가 있으면
			if (opptr){
				// 출력형식에 맞춰 출력
				if (s1[0] != '\0')
				fprintf(intmd, "%d\t%d\t%s", lnum, locctr, line);
				else
				fprintf(intmd, "%d\t%d\t-%s", lnum, locctr, line);
				
				// format만큼 location counter 증가
				if (!strcmp(opptr->form, "1"))
					locctr += 1;
				else if (!strcmp(opptr->form, "2"))
					locctr += 2;
				else if (s2[0] == '+')
					locctr += 4;
				else
					locctr += 3;
				// 줄번호 증가
				lnum += 5;

			}
			// directive가 "BASE"일 때
			else if (!strcmp(s2, "BASE")){
				fprintf(intmd, "%d\t-1\t-%s", lnum, line);
				lnum += 5;
			}
			// directive가 "END"일 때
			else if (!strcmp(s2, "END") && tmp <= 2){
				fprintf(intmd, "%d\t-1\t-%s", lnum, line);
				lnum += 5;
				endflag = 1;
				break;
			}
			// directive가 "WORD" 일 때
			else if (!strcmp(s2, "WORD") && tmp == 3) {
				fprintf(intmd, "%d\t%d\t%s", lnum, locctr, line);
				locctr += 3;
				memset(tmpstr, '\0', sizeof(tmpstr));
				tmp = sscanf(s3, "%d %s", &dectmp, tmpstr);
				if (tmp != 1){
					printf("%d: error: 'WORD' operand error - %s\n", lnum, s3);
					return 0;
				}
			}
			// directive가 "BYTE" 일때 
			else if (!strcmp(s2, "BYTE") && tmp == 3){
				fprintf(intmd, "%d\t%d\t", lnum, locctr);
				// 뒤에 C이면
				if (s3[0] == 'C')
					locctr += strlen(s3) -3;
				// 뒤에 X이면
				else if (s3[0] == 'X')
					locctr += (strlen(s3) - 2) / 2;
				// 둘다 아니면 에러
				else {
					printf("%d: error: 'BYTE' input error\n", lnum);
					return 0;
				}
				lnum += 5;
				fprintf(intmd, "%s", line);
			}
			// directive가 "RESW"일 때
			else if (!strcmp(s2, "RESW") && tmp == 3){
				fprintf(intmd, "%d\t%d\t%s", lnum, locctr, line);
				lnum += 5;
				locctr += 3 * atoi(s3);
			}
			// directive가 "RESB"일 때
			else if (!strcmp(s2, "RESB") && tmp == 3){
				fprintf(intmd, "%d\t%d\t%s", lnum, locctr, line);
				lnum += 5;
				locctr += atoi(s3);
			}
			// 위에 언급된 것 중에 없고 opcodelist에도 없으면 에러
			else {
				printf("%d: error: Invlid directive - %s\n", lnum, s2);
				return 0;
			}
		}

		memset(line, '\0', sizeof(line));
	}

	// END가 없으면 에러 처리 후 종료
	if (endflag == 0){
		printf("error: No END sign\n");
		return 0;
	}

	// 파일 포인터 종료
	fclose(src);
	fclose(intmd);

	// 프로그램 길이
	pro_length = locctr - adr_start;
	
	// 패스2에서 오류 발생시 종료
	if (pass_2(f_name) == 0)
		return 0;

	return 1;
}

int pass_2(char *f_name){
	FILE *intmd, *obj, *lst;
	char of_name[100], lf_name[100];
	char line[200];
	char s1[30], s2[30], s3[30], s4[30];
	int k = 0, l = 0;
	int n, i, x, b, p, e;
	int pc_adr = adr_start;
	int b_adr = 0;
	int tmp = 0;
	int lnum, locctr = 0;
	int cmflag = -1;
	int format;
	int idx;
	OPlist *opptr;
	symbol *symptr;
	char reg[10][2];
	char objcode[10];
	int newop, disp;
	int dectmp, xbpe;
	char strtmp[100], tmpstr[100];
	char bytetmp[100];

	// 인덱스가 레지스터 값
	strcpy(reg[0], "A");
	strcpy(reg[1], "X");
	strcpy(reg[2], "L");
	strcpy(reg[3], "B");
	strcpy(reg[4], "S");
	strcpy(reg[5], "T");
	strcpy(reg[6], "F");
	strcpy(reg[7], " ");
	strcpy(reg[8], "PC");
	strcpy(reg[9], "SW");

	memset(of_name, '\0', sizeof(of_name));
	memset(lf_name, '\0', sizeof(lf_name));

	// lst, obj 파일명 생성하여 쓰기로 파일 열기
	while (f_name[k] != '.'){
		of_name[k] = f_name[k];
		lf_name[k] = f_name[k];
		k++;
	}

	strcat(of_name, ".obj");
	strcat(lf_name, ".lst");
	
	// 파일 열기
	intmd = fopen("intermediate", "r");
	obj = fopen(of_name, "w");
	lst = fopen(lf_name, "w");

	// 첫 줄 받아온다
	fgets(line, sizeof(line), intmd);
	tmp = sscanf(line, "%d \t %d \t %s \t %s \t %s", &lnum, &locctr, s1, s2, s3);

	if (tmp){
		// START가 있으면 lst, obj 파일 작성
		if (!strcmp(s2, "START")){
			fprintf(lst, "%d\t%04X\t%s\t%s\t%X\n", lnum, locctr, s1, s2, adr_start);
			fprintf(obj, "H%-6s%06X%06X\n", s1, adr_start, pro_length);
		}
		// 없으면 obj 파일 H 부분 작성
		else {
			fclose(intmd);
			intmd = fopen("intermediate", "r");
			fprintf(obj, "H      %06X%06X\n", adr_start, pro_length);
		}
	}


	// 계속해서 파일 읽기
	while (1){
		// 초기화
		n = 1;
		i = 1;
		x = 0;
		b = 0;
		p = 0;
		e = 0;
		memset(line, '\0', sizeof(line));
		memset(s1, '\0', sizeof(s1));
		memset(s2, '\0', sizeof(s2));
		memset(s3, '\0', sizeof(s3));
		memset(s4, '\0', sizeof(s4));
		memset(objcode, '\0', sizeof(objcode));
		memset(strtmp, '\0', sizeof(strtmp));
		memset(bytetmp, '\0', sizeof(bytetmp));
		memset(tmpstr, '\0', sizeof(tmpstr));
		cmflag = -1;
		tmp = 0;
		xbpe = 0;
		disp = 0;
		// 초기화

		fgets(line, sizeof(line), intmd);
		if (line[0] == '\0')
			break;

		// 콤마 있는지 확인
		k = 0;
		while (line[k] != '\0'){
			if (line[k] == ','){
				cmflag = k;
				break;
			}
			k++;
		} // cmflag : 콤마의 위치

		// 인자 입력받기
		// 콤마 없으면
		if (cmflag == -1){
			tmp = sscanf(line, "%d \t %d \t %s \t %s \t %s", &lnum, &locctr, s1, s2, s3);
		}
		// 콤마 있으면
		else {
			tmp = sscanf(line, "%d %d \t %s \t %s \t %s \t %s \t %s", &lnum, &locctr, s1, s2, s3, s4, tmpstr);
			if (line[cmflag+1] != ' ' && line[cmflag+1] != '\t'){
				if(line[cmflag-1] != ' ' && line[cmflag-1] != '\t'){

				k = 0;
				while (s3[k] != ',')
					k++;
				k++;
				strcpy(s4, s3+k);
				tmp++;
				s3[strlen(s3) - strlen(s4) - 1] = '\0';
				}
			}
			else if (s4[0] == ','){
				if (strlen(s4) == 1){
					strcpy(s4, tmpstr);
					tmp--;
				}	
				else{
					strcpy(s4, s4+1);
					s4[strlen(s4) - 1] = '\0';
				}
			}

			else{
				s3[strlen(s3) - 1] = '\0';
			}
		}

		/* s1 : label
		   s2 : directive
		   s3 : operand
		   s4 : 2nd operand after comma */
	
		// 주석이면 그대로 출력 
		if (s1[0] == '\0'){
			fprintf(lst, "%s", line);
		}
		else {
			// 'END' 면 종료
			if (!strcmp(s2, "END")){
				fprintf(lst, "%d\t\t\t%s\t%s\n", lnum, s2, s3);
				break;
			}
			// 'BASE'면 b 레지스터에 값 넣
			if (!strcmp(s2, "BASE")){
				idx = ZtoAhash(s3);
				symptr = symhead[idx];
				while (symptr){
					if (!strcmp(symptr->name, s3))
						break;
					symptr = symptr -> link;
				}
				if (!symptr){
					printf("%d: error: 'BASE' symbol error - %s\n", lnum, s3);
					return 0;
				}

				b_adr = symptr->locctr;
				fprintf(lst, "%d\t\t\t%s\t%s\n", lnum, s2, s3);

			}
			
			// operand 확인
			switch (s3[0]){
				// '#' 있으면 immediate addresing
				case '#':
					n = 0;
					i = 1;
					b = 0;
					if (s3[1] >= '0' && s3[1] <= '9')
						p = 0;
					break;
				// '@' 있으면 indirect addressing
				case '@':
					n = 1;
					i = 0;
					break;
				// 보통의 경우는 simple addressing
				default:
					n = 1;
					i = 1;
					break;
			}

			// opcode list에 있는 명령어인지 확인
			if (s2[0] == '+')
				idx = hashFunction(s2+1);
			else
				idx = hashFunction(s2);
			opptr = OPhead[idx];
			while (opptr){
				if (s2[0] != '+'){
					if (!strcmp(opptr->inst_name, s2))
						break;
				}
				else{
					if (!strcmp(opptr->inst_name, s2+1))
						break;
				}
				opptr = opptr -> link;
			}

			// opcode list에 입력된 mnemonic이 있으면
			if (opptr){
				// 몇 형식인지 확인
				if (!strcmp(opptr->form, "1"))
					format = 1;
				else if (!strcmp(opptr->form, "2"))
					format = 2;
				else if (s2[0] == '+'){
					format = 4;
					b = 0;
					p = 0;
					e = 1;
				}
				else
					format = 3;

				// pc register에 값 저장
				pc_adr = locctr + format;

				// 1형식인 경우
				// OPCODE가 obj code
				if (format == 1){
					if (s3[0] != '\0'){
						printf("%d: error: format1 cannot have operand\n", lnum);
						return 0;
					}
					if (s1[0] == '-')
						fprintf(lst, "%d\t%04X\t\t%s\t\t%02X\n", lnum, locctr, s2, opptr->opcode);
					else 
						fprintf(lst, "%d\t%04X\t%s\t%s\t\t%02X\n", lnum, locctr, s1, s2, opptr->opcode);
					
					// 오브젝트 코드 문자열로 저장
					sprintf(objcode, "%02X", opptr->opcode);
					add_objlist(objcode, locctr, 0);
				}

				// 2형식인 경우
				// OPcode 뒤에 register 번호
				else if (format == 2){
					// 인자 없으면 에러
					if (s3[0] == '\0'){
						printf("%d: error: format2 need at least one operand\n", lnum);
						return 0;
					}
					// 유효한 레지스터인지 확인
					for (k = 0 ; k < 10 ; k++){
						if (!strcmp(s3, reg[k]))
							break;
					}
					// 아니면 에러
					if (k == 10){
						printf("%d: error: wrong register name - %s\n", lnum, s3);
						return 0;
					}
					
					// 파일에 출력
					if (s1[0] == '-')
						fprintf(lst, "%d\t%04X\t\t%s\t%s", lnum, locctr, s2, s3);
					else
						fprintf(lst, "%d\t%04X\t%s\t%s\t%s", lnum, locctr, s1, s2, s3);
					l = 0;
					if (cmflag != -1){ // 콤마 있으면
						for (l = 0; l < 10 ; l++){
							if (!strcmp(s4, reg[l]))
								break;
						}
						if (l == 10){
							printf("%d: error: wrong register name - %s\n", lnum, s4);
							return 0;
						}
						fprintf(lst, ", %s", s4);
					}
					fprintf(lst, "\t\t%02X%d%d\n", opptr->opcode, k, l);

					// 오브젝트 코드 문자열로 저장
					sprintf(objcode, "%02X%d%d", opptr->opcode, k, l);
					add_objlist(objcode, locctr, 0);
				}
				// 3형식인 경우
				else if (format == 3){
					// opcode 뒤에 2 bits 값을 n, i로 대체
					newop = opptr->opcode / 2 / 2 * 2 *2 + 2 * n + i;
					// xbpe 값
					xbpe = 2*2*2*x+2*2*b+2*p+e;
					// immediate addressing (#)
					if (n == 0 && i == 1){
						tmp = sscanf(s3+1, "%d %s", &dectmp, strtmp);
						// # 뒤에 상수 이면
						if (tmp == 1){
							// object 코드 저장
							sprintf(objcode, "%02X%01X%03X", newop, xbpe, dectmp);
						}
						else{
							tmp = strlen(s3) - 1;
							strcpy(s3, s3+1);
							s3[tmp] = '\0';
						}
					}
					// 상수가 아니어서 
					//오브젝코드가 생성되지 않았을때
					if (objcode[0] == '\0'){
						// indirect addressing (@)
						if (n == 1 && i == 0){
							tmp = strlen(s3) - 1;
							strcpy(s3, s3+1);
							s3[tmp] = '\0';
						}

						// symbol table에 있는 operand 인지 확인
						if (s3[0] != '\0'){
						idx = ZtoAhash(s3);
						symptr = symhead[idx];
						while (symptr){
							if (!strcmp(symptr->name, s3))
								break;
							symptr = symptr -> link;
						}
						// 없으면 에러 출력하고 종료
						if (!symptr){
							printf("%d: error: symbol error - %s\n", lnum, s3);
							return 0;
						}
							
						// disp 값 저장
						p = 1; b = 0;
						disp = symptr->locctr - pc_adr;

						// pc relative 사용할 수 없는 경우
						if (disp > 2047 || disp < (-2048)){
							// base relative 사용
							b = 1;
							p = 0;
							disp = symptr->locctr - b_adr;
						}
						if (disp < 0)
							disp = disp & 4095;

						if (cmflag != -1){	// 콤마 있을 때
							// X 레지스터 들어오면 x 조정
							if (!strcmp(s4, "X")){
								x = 1;
							}
							// X 레지스터 아니면 에러
							else {
								printf("%d: error: wrong register name - %s\n", lnum, s4);
								return 0;
							}
						}
						}

						// obj code 저장
						xbpe = 2*2*2*x + 2*2*b + 2*p + e;
						sprintf(objcode, "%02X%01X%03X", newop, xbpe, disp);
					}

					// lst 파일에 출력
					if (s1[0] == '-')
					fprintf(lst, "%d\t%04X\t\t%s\t%s", lnum, locctr, s2, s3);
					else
					fprintf(lst, "%d\t%04X\t%s\t%s\t%s", lnum, locctr, s1, s2, s3);
					if (cmflag != -1)
						fprintf(lst, ", %s\t%s\n", s4, objcode);
					else
						fprintf(lst, "\t\t%s\n", objcode);
					add_objlist(objcode, locctr, 0);
				}
				// 4형식인 경우
				else if (format == 4){
					// opcode 뒤에 2 bits 값을 n, i로 대체
					newop = opptr->opcode / 2 / 2 * 2 *2 + 2 * n + i;
					// xbpe 값
					xbpe = 2*2*2*x+2*2*b+2*p+e;
					// immediate addressing (#)
					if (n == 0 && i == 1){
						tmp = sscanf(s3+1, "%d %s", &dectmp, strtmp);
						// # 뒤에 상수 이면
						if (tmp == 1){
							// object 코드 저장
							sprintf(objcode, "%02X%01X%05X", newop, xbpe, dectmp);
						}
						else{
							tmp = strlen(s3) - 1;
							strcpy(s3, s3+1);
							s3[tmp] = '\0';
						}
						add_objlist(objcode, locctr, 0);
					}
					// 상수가 아니어서 
					//오브젝코드가 생성되지 않았을때
					if (objcode[0] == '\0'){
						// indirect addressing (@)
						if (n == 1 && i == 0){
							tmp = strlen(s3) - 1;
							strcpy(s3, s3+1);
							s3[tmp] = '\0';
						}

						// symbol table에 있는 operand 인지 확인
						if (s3[0] != '\0'){
						idx = ZtoAhash(s3);
						symptr = symhead[idx];
						while (symptr){
							if (!strcmp(symptr->name, s3))
								break;
							symptr = symptr -> link;
						}
						// 없으면 에러 출력하고 종료
						if (!symptr){
							printf("%d: error: symbol error - %s\n", lnum, s3);
							return 0;
						}
						

						if (cmflag != -1){	// 콤마 있을 때
							// X 레지스터 들어오면 x 조정
							if (!strcmp(s4, "X")){
								x = 1;
							}
							// X 레지스터 아니면 에러
							else {
								printf("%d: error: wrong register name - %s\n", lnum, s4);
								return 0;
							}
						}
						}

						
						// obj code 저장
						xbpe = 2*2*2*x + 2*2*b + 2*p + e;
						sprintf(objcode, "%02X%01X%05X", newop, xbpe, symptr->locctr);
						add_objlist(objcode, locctr, 1);
					}

					// lst 파일에 출력
					if (s1[0] == '-')
					fprintf(lst, "%d\t%04X\t\t%s\t%s", lnum, locctr, s2, s3);
					else
					fprintf(lst, "%d\t%04X\t%s\t%s\t%s", lnum, locctr, s1, s2, s3);
					if (cmflag != -1)
						fprintf(lst, ", %s\t%s\n", s4, objcode);
					else
						fprintf(lst, "\t\t%s\n", objcode);

				}

			}
			// 입력이 opcode list에 저장된 mnemonic이 아니면
			else {
				// 'BYTE'인 경우
				if (!strcmp(s2, "BYTE")){
					// 'C' 형식으로 들어올 때
					if (s3[0] == 'C'){
						strcpy(bytetmp, s3+2);
						bytetmp[strlen(bytetmp) - 1] = '\0';
						sprintf(objcode, "%02X", bytetmp[0]);
						k = 1;
						while (bytetmp[k] != '\0'){
							sprintf(strtmp, "%02X", bytetmp[k]);
							strcat(objcode, strtmp);
							k++;
						}
					}
					// 'X' 형식으로 들어올 때
					else if (s3[0] == 'X'){
						strcpy(objcode, s3+2);
						objcode[strlen(objcode) - 1] = '\0';
					}
					fprintf(lst, "%d\t%04X\t%s\t%s\t%s\t\t%s\n", lnum, locctr, s1, s2, s3, objcode);
					add_objlist(objcode, locctr, 0);
				}
				// 'WORD'인 경우
				else if (!strcmp(s2, "WORD")){
					sscanf(s3, "%d", &k);
					sprintf(objcode, "%06X", k);
					fprintf(lst, "%d\t%04X\t%s\t%s\t%s\t\t%s\n", lnum, locctr, s1, s2, s3, objcode);
					add_objlist(objcode, locctr, 0);

				}
				// 'RESB' 나 'RESW' obj code 없음
				else if (!strcmp(s2, "RESB") || !strcmp(s2, "RESW")){
					fprintf(lst,"%d\t%04X\t%s\t%s\t%s\n", lnum, locctr, s1, s2, s3);
				}				
			}


		}

	}
	// .lst 파일 출력 완료

	// .obj 파일 출력 시작
	// T 부분
	objlist *objptr = objhead;
	char ttmp[62];
	memset(ttmp, '\0', sizeof(ttmp));
	do{
		fprintf(obj, "T");
		fprintf(obj, "%06X", objptr->loc);
		while (objptr && strlen(ttmp) + strlen(objptr->objcode) <= 62){
			strcat(ttmp, objptr->objcode);
			objptr = objptr -> link;
			if (!objptr)
				break;
			if (objptr->flag == 1)
				break;
		}
		fprintf(obj, "%02X", (unsigned int)strlen(ttmp) / 2);
		fprintf(obj, "%s\n", ttmp);
		memset(ttmp, '\0', sizeof(ttmp));
	} while (objptr);
	
	// M 부분
	objptr = objhead;
	while (objptr){
		if (objptr->mflag ==1){
			fprintf(obj, "M");
			fprintf(obj, "%06X", objptr->loc+ 1);
			fprintf(obj, "05\n");
		}
		objptr = objptr->link;
	}

	// E 부분
	fprintf(obj, "E");
	fprintf(obj, "%06X\n", adr_start);


	fclose(intmd);
	fclose(obj);
	fclose(lst);
	return 1;
}

int ZtoAhash(char* label){
	// Z to A 순서로
	return 25 - (label[0] - 'A');
}

int symbolcmd(){
	symbol *ptr;
	int i = 0;
	
	// symbol table 만들어져있는지 확인
	if (symflag == 1){
	// symbol table 내용 차례로 출력
	while (i < 26){
		ptr = symhead[i];

		while (ptr) {
			printf("\t%s\t%04X\n", ptr->name, ptr->locctr);
			ptr = ptr -> link;
		}
		i++;	
	}
	}
	// table 없으면 에러
	else {
		printf("No symbol table\n");
		return 0;
	}
	return 1;
}

void add_objlist(char* objcode, int loc, int m){
	objlist *newNode = (objlist*)malloc(sizeof(objlist)); // 새로운 값을 저장할 노드
	objlist *ptr = objhead;	// linked list 탐색에 사용되는 임시변수

	strcpy(newNode->objcode, objcode);
	newNode->link = NULL;
	newNode->loc = loc;
	newNode->flag = 0;
	newNode->mflag = 0;
	// 새 노드에 값 저장

	if (objhead == NULL) {	// head가 NULL 이면
		objhead = (objlist*)malloc(sizeof(objlist));
		objhead = newNode;
		// head 할당한 후 새 노드 저장
		if (m == 1)
			newNode->mflag = 1;
	}
	else {	// head가 NULL하지 않으면
		while (ptr->link != NULL)
			ptr = ptr->link;	// 새 노드를 연결할 노드까지 이동
		ptr-> link = newNode;		// 새 노드 연결
		if ((newNode->loc) - (ptr->loc) >= 4096)
			newNode->flag = 1; 
		if (m == 1)
			newNode->mflag = 1;
	}
}

/* end of proj 2 */
