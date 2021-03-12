#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH 200

int main()
{
	int num, line_count = 0;//입력받은 확진자 심 저장할 변수 num과 확진자 심 찾기 위해 파일에서 줄 세는 용
	int time;//시간표 출력 용
	char buffer[MAX_LENGTH];//확진자 심 스트링 저장
	char buffer_2[MAX_LENGTH];// 고위험 심 스트링 저장
	char buffer_3[MAX_LENGTH];// 덜 위험한 심 스트링 저장
	FILE* sims = NULL;
	//확진자 심 정하기//
	printf("Who is the confirmed SIM? (Choose from 1-500): ");
	scanf_s("%d", &num);
	if (0 == fopen_s(&sims, "sims.csv", "r")) //파일을 열고 읽기 전용으로 지정.
	{
		while (fgets(buffer, MAX_LENGTH, sims) != NULL) //fgets로 파일을 줄별로 읽어 확진자 심의 정보 버퍼에 저장하여 가져오기.
		{
			line_count++;
			if (line_count == num + 1) // 첫째 줄은 항목이므로 이를 제외하기 위해 +1을 해주었습니다. 해당 줄에 도달하면 줄을 출력해본다.
			{
				printf("The confirmed SIM's number and Masked(Y/N) and places it visited :\n%s\n", buffer);			
				// 확진자가 제대로 선정되었는지 확인 차 printf.
				{
					//buffer에 저장된 스트링 쪼개서 배열에 할당하기//
					char* conf_sim[30] = { NULL, };    // conf_sim 에 각각 확진자의 번호 마스크, 장소들 할당.
					int i = 0;                     // 주소 0부터
					char* ptr = strtok(buffer, ",");   // buffer에 저장된 스트링을 ptr에 받아서 , 를 기점으로 쪼갠다.
					while (ptr != NULL)            // 
					{
						conf_sim[i] = ptr;             // 쪼갠 문자열을 conf_sim[i]에 할당.
						i++;                       // 
						ptr = strtok(NULL, ",");   // 
					}
					//확진자 심의 시간표 출력//
					printf("\n<<The confirmed SIM's TIMETABLE>>\n\n");
					for (int i = 2; i < 20; i++)
					{
						int time = i + 6;
						if (conf_sim[i] != NULL)           // 
							printf("%d:00: %s\n", time, conf_sim[i]);   // 각 항목 프린트.
					}
					//고위험 심 추출하기//
					printf("\nThe Highly Suspected Sims Are: \n\n");
					fseek(sims, 0, SEEK_SET);//파일 포인터를 다시 맨 처음으로 이동. 아니면 맨처음에 fgets가 마지막으로 읽었던 곳에서부터 줄을 읽는다.
					while (fgets(buffer_2, MAX_LENGTH, sims))
					{
	                   //fgets로 줄 읽기
					   //buffer에 저장된 스트링 쪼개서 배열에 할당하기
						char* comp_sim[30] = { NULL, };    // comp_sim 에 각각 비교심의 번호 마스크, 장소들 할당
						int m = 0;                     // 주소 0부터
						char* ptr = strtok(buffer_2, ",");   // 쉼표로 구분
						while (ptr != NULL)            //comp_sim 쉼표로 분리해서 주소에 저장
						{
							comp_sim[m] = ptr;
							m++;
							ptr = strtok(NULL, ",");
						}
						//확진자와 나머지 심들의 장소를 맨 처음 집 다음인 [3]부터 [16]까지 비교하고 해당 심의 번호 출력//
						int x;// x 1씩 증가시키면서 스트링 비교
						for (x = 3; x < 16; x++)   //x가 0,1,2,16 일때는 각각 심의 번호, 마스크 여부, 각자의 집이므로 비교할 필요가 없어서 x=3부터 시작                   
						{
							if (x != num + 1) //확진자 번호는 다시 안 나오게
							{
								if ((strcmp(comp_sim[x], conf_sim[x]) == 0) && (strcmp(comp_sim[x], "Home") != 0) && (strcmp(comp_sim[0], conf_sim[0]) != 0))
									//확진자 심과 리스트의 심들 비교. 집에 있을 때는 각자 집에 있으므로 Home은 겹치는 동선에서 제외. 그리고 확진자 심 자기 자신의 번호도 제외.//	     	
									printf("%s ", comp_sim[0]);
								// 심의 번호가 담겨있는 comp_sim[0]를 출력//작성한 코드 특성 상 동선이 두 번 겹치면 번호도 두 번 출력된다.
							}
						}
					}
					//덜 위험한 심 추출하기//
					printf("\n\nThe Little bit Suspected Sims Are: \n\n");
					fseek(sims, 0, SEEK_SET);//파일 포인터를 다시 맨 처음으로 이동. 
						while (fgets(buffer_2, MAX_LENGTH, sims) ) //다시 고위험군 심 찾기
						{
							char* comp_sim[30] = { NULL, };    // comp_sim 에 각각 확진자의 번호 마스크, 장소들 할당
							int j = 0;                     // 주소 0부터
							char* ptr = strtok(buffer_2, ",");   // 쉼표로 구분
							while (ptr != NULL)            //comp_sim 쉼표로 분리해서 주소에 저장
							{
								comp_sim[j] = ptr;
								j++;
								ptr = strtok(NULL, ","); // 읽은 줄 배열에 저장
							}
							//확진자와 나머지 심들의 장소를 맨 처음 집 다음인 [3]부터 [15]까지 비교하여 고위험 심 지정//
						    int y;  // y 1씩 증가시키면서 스트링 비교
							for (y = 3; y < 16; y++)   //x가 0,1,2 일때는 각각 심의 번호, 마스크 여부, 각자의 집이므로 비교할 필요가 없어서 x=3부터 시작                   
							{
								 //확진자 번호는 비교 대상에서 제외
								    if ((strcmp(comp_sim[y], conf_sim[y]) == 0) && (strcmp(comp_sim[y], "Home") != 0 && (strcmp(comp_sim[1], "N")) == 0))  //fgets로 읽은 줄 비교해보기
									{
										//고위험군 심을 찾고 그 중 마스크 미착용자로 한정 
										fseek(sims, 0, SEEK_SET);//파일 포인터를 다시 맨 처음으로 이동, 고위험군과 비교할 심 목록 fgets로 다시 만들기
										while (fgets(buffer_3, MAX_LENGTH, sims))
										{
											char* comp_sim_2[30] = { NULL, };    // comp_sim_2 에 다시 각각 심의 번호 마스크, 장소들 할당
											int k = 0;                     // 주소 0부터
											char* ptr = strtok(buffer_3, ",");   // 쉼표로 구분
											while (ptr != NULL)            //comp_sim 쉼표로 분리해서 주소에 저장
											{
												comp_sim_2[k] = ptr;
												k++;
												ptr = strtok(NULL, ",");
											}
											int z;
											for (z = y + 1 ; z < 16; z++) //확진자와 겹친 다음인 y+1의 장소부터 비교
											{
												if ((strcmp(comp_sim_2[z], comp_sim[z]) == 0) && (strcmp(comp_sim_2[z], "Home") != 0) && (strcmp(comp_sim_2[0], comp_sim[0]) != 0) && (strcmp(comp_sim_2[0], conf_sim[0]) != 0))
													// 확진자와 고위험군의 번호는 출력 안하도록 조건 설정
												printf("%s ", comp_sim_2[0]); // 덜 위험 심의 번호가 담겨있는 comp_sim_2 [0]의 번호를 출력
											}	
										}
									}					
							}
						}
				}
			}
		}
		fcclose(sims);
		return 0;
	}
	system("PAUSE");
}
