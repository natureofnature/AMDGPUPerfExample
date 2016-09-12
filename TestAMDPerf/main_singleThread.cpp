//
//  main.c
//  CLCommon
//
//  Created by NATURE on 11/9/2016.
//  Copyright © 2016 NATURE. All rights reserved.
//
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define AMPPERAPIEnable
#endif
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include<CL/cl.h>
#endif
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#include <direct.h>
#ifdef AMPPERAPIEnable
#include "PerformanceCountersAMD.h"
#endif
#endif
#include<fstream>

#ifdef _WIN32//check pwd for windows
void PrintFullPath(char * partialPath)
{
	
	char full[_MAX_PATH];
	if (_fullpath(full, partialPath, _MAX_PATH) != NULL)
		printf("Full path is: %s\n", full);
	else
		printf("Invalid path\n");
}
#endif
//check error
void ckE(int status, int line) {
	if (status != CL_SUCCESS) {
		printf("Line[%d] Error happens with code %d\n", line, status);
		exit(1);
	}
}
//file to char*
char* readFile(char* filePath) {
	char* content = NULL;
	FILE* f = fopen(filePath, "r");
	if (f == NULL) {
		printf("File %s does not exist\n", filePath);
		return content;
	}
	content = new char[10000];
	memset(content, 0, 10000);
	int lenSize = 256;
	char* tmpC = new char[lenSize];
	while (fgets(tmpC, lenSize, f)) {
		strcat(content, tmpC);
	}
	return content;
}

void iniArray(int size, float* array) {
	for (int i = 0; i<size; i++) {
		array[i] = rand() % (i + 1);
	}
}

int chosenPlatform = 0;
int chosenDevice = 1;


int main(int argc, const char * argv[]) {
#ifdef _WIN32
	printf("Current work path is \n");
	PrintFullPath(".\\");
#endif
	if (argc == 5) {
		if (!strcmp(argv[1], "-P")) {
			chosenPlatform = atoi(argv[2]);
		}
		else {
			printf("args not set, value is default for platforms\n");
		}
		if (!strcmp(argv[3], "-D")) {
			chosenDevice = atoi(argv[4]);
		}
		else {
			printf("args not set, value is default for devices\n");
		}
	}
	else {
		printf("args not in proper form, using default value\n");
	}

	cl_uint numPlatforms;
	cl_uint numDevices;
	cl_int status;
	cl_platform_id* platforms;
	cl_device_id* devices;
	cl_context context;

	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	ckE(status, __LINE__);
	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);
	ckE(status, __LINE__);
	printf("There are %d platforms and platform %d is chosen\n", numPlatforms, chosenPlatform);
	cl_platform_id this_platform = platforms[chosenPlatform];

	status = clGetDeviceIDs(this_platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
	ckE(status, __LINE__);
	devices = (cl_device_id*)malloc(sizeof(cl_device_id)*numDevices);
	status = clGetDeviceIDs(this_platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
	ckE(status, __LINE__);
	printf("Thera are %d devices on chosen paltform and divice id is %d\n", numDevices, chosenDevice);
	cl_device_id this_device = devices[chosenDevice];

	context = clCreateContext(NULL, 1, &this_device, NULL, NULL, &status);
	ckE(status, __LINE__);
	const char* programSource = readFile((char*)"Kernel1.cl");
	cl_program program = clCreateProgramWithSource(context, 1, &programSource, NULL, &status);
	ckE(status, __LINE__);
	status = clBuildProgram(program, 1, &this_device, NULL, NULL, &status);
	if (status != CL_SUCCESS) {
		cl_uint infosize;
		status = clGetProgramBuildInfo(program, this_device, CL_PROGRAM_BUILD_LOG, 0, NULL, &infosize);
		ckE(status, __LINE__);
		char* info = new char[infosize+1];
		status = clGetProgramBuildInfo(program, this_device, CL_PROGRAM_BUILD_LOG, infosize, info, NULL);
		printf("build program info:\n %s", info);
		delete info;
	}

	cl_command_queue cmdQueueA = clCreateCommandQueue(context, this_device, CL_QUEUE_PROFILING_ENABLE, &status);
	ckE(status, __LINE__);

	cl_command_queue cmdQueueB = clCreateCommandQueue(context, this_device, CL_QUEUE_PROFILING_ENABLE, &status);
	ckE(status, __LINE__);

	cl_kernel simpleKernel = clCreateKernel(program, (char*)"VectorAdd", &status);
	ckE(status, __LINE__);
	cl_kernel compareKernel = clCreateKernel(program, (char*)"VectorFun", &status);
	ckE(status, __LINE__);


	

	


	//// for command 1
	/*data & buffer*/
	int numThread = 10240;
	float* A = new float[numThread];
	float* B = new float[numThread];
	iniArray(numThread, A);
	cl_mem memA = clCreateBuffer(context, CL_MEM_READ_ONLY, numThread * sizeof(cl_float), NULL, &status);
	ckE(status, __LINE__);
	cl_mem memB = clCreateBuffer(context, CL_MEM_WRITE_ONLY, numThread * sizeof(cl_float), NULL, &status);
	ckE(status, __LINE__);


	status = clEnqueueWriteBuffer(cmdQueueA, memA, CL_FALSE, 0, numThread * sizeof(cl_float), A, 0, NULL, NULL);
	ckE(status, __LINE__);


	status = clSetKernelArg(simpleKernel, 0, sizeof(cl_mem), &memA);
	ckE(status, __LINE__);
	status = clSetKernelArg(simpleKernel, 1, sizeof(cl_mem), &memB);
	ckE(status, __LINE__);
	status = clSetKernelArg(simpleKernel, 2, sizeof(cl_uint), &numThread);
	ckE(status, __LINE__);
	size_t globalsize[1] = { (size_t)numThread };
	size_t localsize[1] = { 64 };



	/////for command 2

	float* C = new float[numThread];
	float* D = new float[numThread];
	iniArray(numThread, C);
	cl_mem memC = clCreateBuffer(context, CL_MEM_READ_ONLY, numThread * sizeof(cl_float), NULL, &status);
	ckE(status, __LINE__);
	cl_mem memD= clCreateBuffer(context, CL_MEM_WRITE_ONLY, numThread * sizeof(cl_float), NULL, &status);
	ckE(status, __LINE__);
	status = clEnqueueWriteBuffer(cmdQueueB, memC, CL_FALSE, 0, numThread * sizeof(cl_float), C, 0, NULL, NULL);
	ckE(status, __LINE__);
	status = clSetKernelArg(compareKernel, 0, sizeof(cl_mem), &memC);
	ckE(status, __LINE__);
	status = clSetKernelArg(compareKernel, 1, sizeof(cl_mem), &memD);
	ckE(status, __LINE__);
	status = clSetKernelArg(compareKernel, 2, sizeof(cl_uint), &numThread);
	ckE(status, __LINE__);






#ifdef AMPPERAPIEnable
	PerformanceAMD pAMD;
	pAMD.GPA_Initialize();
	ckE(pAMD.GPA_OpenContext(cmdQueueB), __LINE__);
	//ckE(pAMD.GPA_OpenContext(cmdQueueB), __LINE__);
	gpa_uint32 numCounter;
	pAMD.GPA_GetNumCounters(&numCounter);
	printf("num counter is %d\n", numCounter);
	
	std::ofstream ous;
	ous.open("./info.txt", std::ios::out);


	for (int i = 0; i < numCounter; i++) {
		ous <<"["<< i << "] [";
		const char* counterName = new const char[100];
		pAMD.GPA_GetCounterName(i,&counterName);
		ous << counterName<<"] [";
		const char* counterDecription = new const char[1024];
		pAMD.GPA_GetCounterDescription(i, &counterDecription);
		ous << counterDecription <<"]"<< std::endl;
		
		
		
	}
	ous.close();




#endif
	

	cl_event eventA=NULL;
	cl_event eventB=NULL;


#ifdef AMPPERAPIEnable
	int performanceCounterNo = 15;
	pAMD.GPA_EnableCounter(performanceCounterNo);
	gpa_uint32 currentWaitSessionID = 1;
	gpa_uint32 sessionID;
	bool readyResult = false;

	//每begin 一次session, session id 就增加1



	pAMD.GPA_BeginSession(&sessionID);
	gpa_uint32 numRequiredPasses;
	pAMD.GPA_GetPassCount(&numRequiredPasses);
	for (gpa_uint32 i = 0; i<numRequiredPasses; i++) {
		pAMD.GPA_BeginPass();


		
		pAMD.GPA_BeginSample(0);
#endif
		status = clEnqueueNDRangeKernel(cmdQueueB, compareKernel, 1, NULL, globalsize, localsize, 0, NULL, &eventB);
		ckE(status, __LINE__);
		
		status = clEnqueueNDRangeKernel(cmdQueueA, simpleKernel, 1, NULL, globalsize, localsize, 0, NULL, &eventA);
		ckE(status, __LINE__);
	
	
#ifdef AMPPERAPIEnable
	pAMD.GPA_EndSample();

	
	
	
	
	
	pAMD.GPA_EndPass();
	}
	pAMD.GPA_EndSession();

	GPA_Status sessionStatus;
	clFinish(cmdQueueA);
	clFinish(cmdQueueB);

	
	sessionStatus = pAMD.GPA_IsSessionReady(&readyResult, currentWaitSessionID);


	while (sessionStatus == GPA_STATUS_ERROR_SESSION_NOT_FOUND) {       // skipping a session which got overwritten
		
		sessionStatus = pAMD.GPA_IsSessionReady(&readyResult, currentWaitSessionID);
	}
	if (readyResult) {

		printf("-------------state ready---------------\n");
		GPA_Type type;
		pAMD.GPA_GetCounterDataType(performanceCounterNo, &type);
		switch (type) {
		case GPA_TYPE_UINT32: {
			gpa_uint32 value;
			pAMD.GPA_GetSampleUInt32(currentWaitSessionID,0, performanceCounterNo, &value);
			printf(" value is %u\n", value);

		}break;
		case GPA_TYPE_UINT64: {
			gpa_uint64 value;
			pAMD.GPA_GetSampleUInt64(currentWaitSessionID, 0, performanceCounterNo, &value);
			printf(" value is %I64u\n", value);
		}break;
		case GPA_TYPE_FLOAT32: {
			gpa_float32 value;
			pAMD.GPA_GetSampleFloat32(currentWaitSessionID, 0, performanceCounterNo, &value);
			printf(" value is %f\n", value);
		}break;
		case GPA_TYPE_FLOAT64: {
			gpa_float64 value;
			pAMD.GPA_GetSampleFloat64(currentWaitSessionID, 0, performanceCounterNo, &value);
			printf(" value is %f\n", value);
		}break;
		default: {
			printf("data type does not allow\n");
		}
		} 
	}
	cl_ulong start1, end1, start2, end2;
	status = clGetEventProfilingInfo(eventA, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start1,NULL);
	ckE(status, __LINE__);
	status = clGetEventProfilingInfo(eventA, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end1, NULL);
	ckE(status, __LINE__);

	status = clGetEventProfilingInfo(eventB, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start2, NULL);
	ckE(status, __LINE__);
	status = clGetEventProfilingInfo(eventB, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end2, NULL);
	ckE(status, __LINE__);
	int a = start1 - start1;
	int b = end1 - start1;
	int c = start2 - start1;
	int d = end2 - start1;

	int earliest = a < c ? a : c;
	int latest = b > d ? b : d;


	int duration2 = b - a + d - c;


	printf("kernelA start = %d , end = %d \nkernelB start = %d , end = %d\nduration is %d\n sequntial duration is %d\n", a, b, c, d,latest-earliest,duration2);

	




#endif
	status = clEnqueueReadBuffer(cmdQueueA, memB, CL_TRUE, 0, numThread * sizeof(cl_float), B, 0, NULL, NULL);
	ckE(status, __LINE__);
	status = clEnqueueReadBuffer(cmdQueueB, memD, CL_TRUE, 0, numThread * sizeof(cl_float), D, 0, NULL, NULL);
	ckE(status, __LINE__);


	for (int i = 0; i<numThread; i++) {
		if ((B[i] - A[i]) != 1.0f) {
			printf("Result is not correct %f vs %f\n",B[i],A[i]);
			
		}
		if ((D[i] - C[i]) != 1.0f) {
			printf("Result is not correct %f vs %f\n", D[i], C[i]);

		}

	}
	 
	printf(" Result is right and your GPU is working!\n");


	ckE(clReleaseMemObject(memA), __LINE__);
	ckE(clReleaseMemObject(memB), __LINE__);
	ckE(clReleaseMemObject(memC), __LINE__);
	ckE(clReleaseMemObject(memD), __LINE__);
	ckE(clReleaseKernel(simpleKernel), __LINE__);
	ckE(clReleaseKernel(compareKernel), __LINE__);
	ckE(clReleaseCommandQueue(cmdQueueA), __LINE__);
	ckE(clReleaseCommandQueue(cmdQueueB), __LINE__);
	ckE(clReleaseProgram(program), __LINE__);
	ckE(clReleaseContext(context), __LINE__);
	ckE(clReleaseDevice(this_device), __LINE__);

	return 0; 
}