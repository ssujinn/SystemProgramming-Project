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
		// 그 이외의 입력 출력하지 않음
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
	

	return 0;
}

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
	int adflag = 0;	// dump 뒤에 인자가 없을 때 1로 set(마지막 address 저장)

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
			adflag = 1;
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
			printf("[%s,%X]", ptr->inst_name, ptr->opcode);
		}
		printf("\n");
	}
}
