#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "scaffoldgraph.h"
#include "scaffolding.h"
#include "lp/lp_lib.h"

using namespace std;

string fGetCfgFileName(){
	string paraStr_CfgFileName;
	char szWorkDir[100]={0};
	char szCfgFileNameTemp[100]={0};
	if(!getcwd(szWorkDir,260)){
		return "false";
	}
	paraStr_CfgFileName=szWorkDir;
	paraStr_CfgFileName.append("/");
	return paraStr_CfgFileName;
}

string int_Tostring(int i){
    stringstream st;
    string str;
    st<<i;
    st>>str;
    return str;
}

int WriteScript_corr(string leftread,string rightread,string contig,string output,int i){
    ofstream location_out;
    int pos;
    string path;
    string num=int_Tostring(i);
    pos=output.find_last_of('/');
    path=output.substr(0,pos)+"/mapping/BOSS_alignScript_corr"+num+".sh";
    cout<<"path:"<<path<<endl;
    char * openPath = new char[50];
    openPath=(char*)path.c_str();
    location_out.open(openPath); 
        if (!location_out.is_open())
        return 0;
    string out0=string("#!/bin/bash\n");
    string out1=string("bowtie2-build ")+contig+string(" ")+output.substr(0,pos)+string("/mapping/index")+string(" &&\n");
    string out2=string("bowtie2 -x ")+output.substr(0,pos)+string("/mapping/index ")+string(" -1 ")+leftread+string(" -2 ")+rightread+string(" -S ")+output.substr(0,pos)+string("/mapping/correction")+num+string(".sam ")+string("&&\n");
    string out3=string("samtools view -Sb ")+output.substr(0,pos)+string("/mapping/correction")+num+string(".sam ")+string(" -o ")+output.substr(0,pos)+string("/mapping/correction")+num+string(".bam ")+string("&&\n");
    string out4=string("samtools sort ")+output.substr(0,pos)+string("/mapping/correction")+num+string(".bam ")+string(" -o ")+output.substr(0,pos)+string("/mapping/correction_sort")+num+string(".bam ")+string("&&\n");
    string out5=string("samtools index ")+output.substr(0,pos)+string("/mapping/correction_sort")+num+string(".bam ");
    location_out<<out0;
    location_out<<out1;
    location_out<<out2;
    location_out<<out3;
    location_out<<out4;
    location_out<<out5;
    location_out.close();
}


int WriteScript(string leftread,string rightread,string contig,string output,int i){
    ofstream location_out;
    int pos;
    string path;
    string num=int_Tostring(i);
    pos=output.find_last_of('/');
    path=output.substr(0,pos)+"/mapping/BOSS_alignScript"+num+".sh";
    cout<<"path:"<<path<<endl;
    char * openPath = new char[50];
    openPath=(char*)path.c_str();
    location_out.open(openPath); 
        if (!location_out.is_open())
        return 0;
    string out0=string("#!/bin/bash\n");
    string out1=string("bowtie2-build ")+contig+string(" ")+output.substr(0,pos)+string("/mapping/index2")+string("&&\n");
    string out2=string("bowtie2 -x ")+output.substr(0,pos)+string("/mapping/index2 ")+leftread+string(" -S ")+output.substr(0,pos)+string("/mapping/correctionleft")+num+string(".sam ")+string("&&\n");
    string out3=string("samtools view -Sb ")+output.substr(0,pos)+string("/mapping/correctionleft")+num+string(".sam ")+string(" -o ")+output.substr(0,pos)+string("/mapping/correctionleft")+num+string(".bam ")+string("&&\n");
    string out4=string("bowtie2 -x ")+output.substr(0,pos)+string("/mapping/index2 ")+rightread+string(" -S ")+output.substr(0,pos)+string("/mapping/correctionright")+num+string(".sam ")+string("&&\n");
    string out5=string("samtools view -Sb ")+output.substr(0,pos)+string("/mapping/correctionright")+num+string(".sam ")+string(" -o ")+output.substr(0,pos)+string("/mapping/correctionright")+num+string(".bam ");

    location_out<<out0;
    location_out<<out1;
    location_out<<out2;
    location_out<<out3;
    location_out<<out4;
    location_out<<out5;

    location_out.close();
}

int bashExe(string openpath){
    int pos;
    string cmd ="bash "+openpath;
    char line[300];
    FILE *fp;
    const char *sysCommand = cmd.data();
    if ((fp = popen(sysCommand, "r"))== NULL){
        cout<<"error" << endl;
        return 0;
    }
    while (fgets(line, sizeof(line)-1, fp)!= NULL){
        cout<<line ;
    }
    pclose(fp);
}

int shellExe(string shell){
    char line[300];
    FILE *fp;
    const char *sysCommand = shell.data();
    if ((fp = popen(sysCommand, "r"))== NULL){
        cout<<"error" << endl;
        return 0;
    }
    while (fgets(line, sizeof(line)-1, fp)!= NULL){
        cout<<line ;
    }
    pclose(fp);
}

int main(int argc, char *argv[]){
    
    if(argc == 1){
        cout<<"Command line:"<<endl;
        cout<<"./boss <contigs.fa> <leftread.fa> <rightread.fa> <read_length>"<<endl; 
        cout<<"<insert_size> <std> <min_weight> <min_number> <is_paired_end>"<<endl;
        cout<<"<edge_weight_method> <scaffold_file_name>"<<endl;
        cout<<endl;
        cout<<"Parameters:"<<endl;
        cout<<"<contigs.fa>:"<<endl;
        cout<<'\t'<<"The file includes contigs produced by one assembler."<<endl;
        cout<<"<leftread.fasta>:"<<endl;
        cout<<'\t'<<"The left end read of paired-end/mate-pair library."<<endl;
        cout<<"<rightread.fasta>:"<<endl;
        cout<<'\t'<<"The right end read of paired-end/mate-pair library."<<endl;
        cout<<"<read_length>:"<<endl;
        cout<<'\t'<<"The length of read."<<endl;
        cout<<"<insert_size>:"<<endl;
        cout<<'\t'<<"The insert size of read library."<<endl;
        cout<<"<std_percentage>:"<<endl;
        cout<<'\t'<<"The percentage of standard deviation to insert size,"<<endl;
        cout<<'\t'<<"std = insert_size*std_percentage. In default,"<<endl;
        cout<<'\t'<<"std_percentage = 0.07."<<endl;
        cout<<"<min_weight>:"<<endl;
        cout<<'\t'<<"One cutoff for removing suprious edgs in"<<endl;
        cout<<'\t'<<"the scaffold graph. In default, min_weight = 0.2."<<endl;
        cout<<"<min_number>:"<<endl;
        cout<<'\t'<<"The minimum number of links between contigs. In default,"<<endl;
        cout<<'\t'<<"min_number = 2."<<endl;
        cout<<"<is_paired_end>:"<<endl;
        cout<<'\t'<<"It is equal to 0 or 1, 1 represents that the read library is"<<endl;
        cout<<'\t'<<"paired-end, 0 represents that the read library is mate-paired."<<endl;
        cout<<"<edge_weight_method>:"<<endl; 
        cout<<'\t'<<"It is equal to 0 or 1, 0 represents that the edge weight calculated"<<endl;
        cout<<'\t'<<"by arithmetic mean, 1 represents that the edge weight calculated"<<endl;
        cout<<'\t'<<"by geometric mean. In default, edge_weight_method = 0."<<endl;
        cout<<"<scaffold_file_name>:"<<endl; 
        cout<<'\t'<<"Output file name, the file includes scaffolds produced by BOSS."<<endl;
        cout<<endl;
        cout<<"Example:"<<endl;
        cout<<'\t'<<"./boss contigs.fa left.bam right.bam 76 650 0.07 0.2 2 0 0 ecoli"<<endl;
        cout<<'\t'<<"This command will produce scaffolding file, ecoli_ScaffoldSet.fa."<<endl;
        exit(0);
    }
	
    if((argc-3)%9!=0){
        cout<<"Please Input Correct Arguments!"<<endl;
        exit(0);
    }
    int libraryCount=(argc-3)/9;
	
	string path=argv[argc-1];
    string openpath;
	int pos=path.find_last_of('/');
    openpath=path.substr(0,pos);
	string dirName=openpath+string("/mapping");
	int isCreate = mkdir((char*)dirName.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	if(!isCreate){
		cout<<"create path success!"<<endl;
	}
	else{
		cout<<"create path failed! "<<endl;
	}
	//alignpath_corr=openpath+"/mapping/BOSS_alignScript_corr.sh";
	//alignpath=openpath+"/mapping/BOSS_alignScript.sh";

	string contig;
	if(libraryCount==1){
    for(int i = 0; i < libraryCount; i++){
			contig=argv[1];
		WriteScript_corr(argv[2+9*i],argv[2+9*i+1],contig,argv[argc-1],i);
		string num=int_Tostring(i);
		bashExe(openpath+"/mapping/BOSS_alignScript_corr"+num+".sh");
		int mu=atoi(argv[2+9*i+3]);
		float sigma=mu*atof(argv[2+9*i+4]);
		int sigma1=(int)round(sigma);
		string muStr=int_Tostring(mu);
		string sigmaStr=int_Tostring(sigma1);
		string paraStr_CfgFileName=fGetCfgFileName();
		string shell=string("python ")+paraStr_CfgFileName+string("mec.py -bam ")+openpath+string("/mapping/correction_sort")+num+string(".bam -a 0.4 -b 0.5 -m ")+muStr+string(" -s ")+sigmaStr+string(" -i ")+argv[1]+string(" -o ")+openpath+string("/mapping/correctionContig.fasta");
		cout<<shell<<endl;
		shellExe(shell);
		string shell2=string("python ")+paraStr_CfgFileName+string("contigSort_origin.py ")+openpath+string("/mapping/correctionContig.fasta ")+openpath+string("/mapping/correctionContig1.fasta ");
		cout<<shell2<<endl;
		shellExe(shell2);
	}
	}
	
    for(int i = 0; i < libraryCount; i++){
		if(libraryCount==2){
			WriteScript(argv[2+9*i],argv[2+9*i+1],argv[1],argv[argc-1],i); 
		}
        else{
			WriteScript(argv[2+9*i],argv[2+9*i+1],openpath+string("/mapping/correctionContig1.fasta "),argv[argc-1],i);
		}
		string num=int_Tostring(i);
        bashExe(openpath+"/mapping/BOSS_alignScript"+num+".sh");
    }
	

    time_t timep;
    time(&timep);
    ofstream ocout;
    char * bossInforFileName = new char[20];
    strcpy(bossInforFileName, "BOSS_infor.txt");
    ocout.open(bossInforFileName);
    for(int i = 0; i<argc; i++){
        ocout<<argv[i]<<" ";
    }
    ocout<<endl;
    ocout<<"BOSS start time:"<<asctime(gmtime(&timep))<<endl;

    InputArg * inputArg = new InputArg[libraryCount];
	string input1[libraryCount],input2[libraryCount],num;
    for(int i = 0; i < libraryCount; i++){
		num=int_Tostring(i);
		input1[i]=openpath+string("/mapping/correctionleft")+num+string(".bam");
		input2[i]=openpath+string("/mapping/correctionright")+num+string(".bam");
        inputArg[i].bamFileName1 =(char*)input1[i].c_str();
        inputArg[i].bamFileName2 =(char*)input2[i].c_str();
        inputArg[i].readLength = atoi(argv[2+9*i+2]);
        inputArg[i].insertsize = atoi(argv[2+9*i+3]);
        inputArg[i].std = atof(argv[2+9*i+4])*inputArg[i].insertsize;
        inputArg[i].minEdgeWeight = atof(argv[2+9*i+5]);
        inputArg[i].minEdgeLinkNumber = atoi(argv[2+9*i+6]);
        inputArg[i].pairedRead = atoi(argv[2+9*i+7]);
        inputArg[i].edgeMeanMethod = atoi(argv[2+9*i+8]);
    }

	string input3;
	if(libraryCount==1){
		input3=openpath+string("/mapping/correctionContig1.fasta");
	}
    else{
		input3=argv[1];
	}
	cout<<input3<<endl;
    char * contigSetFileName = (char*)input3.c_str();
    long int contigCount = 0;
    ContigSet * contigSet = GetContigSet(contigSetFileName, contigCount); 
    
    long int * scaffoldLength = NULL;
    long int * contigLength = new long int[contigCount];
    for(long int i = 0; i<contigCount; i++){
        contigLength[i] = strlen(contigSet[i].contig);
    }
    ScaffoldSet * scaffoldSet = InitScaffoldSet(contigSet, contigCount);
    ScaffoldToContig * scaffoldToContig = NULL;
    long int scaffoldCount = contigCount;
    ScaffoldGraphHead * scaffoldGraphHead = new ScaffoldGraphHead[libraryCount];
    
    long int ss = 0;
    for(int i = 0; i < libraryCount; i++){
        scaffoldToContig = GetScaffoldToContig(scaffoldSet, contigLength, contigCount, scaffoldCount); 
        scaffoldLength = GetScaffoldSetLength(scaffoldSet, scaffoldCount);
        BuildScaffoldGraphFromTwoBam(scaffoldSet, scaffoldLength, contigSet, contigLength, scaffoldToContig, scaffoldCount, scaffoldGraphHead + i, inputArg + i, argv[argc-1]);
        OptimizeScaffoldGraph((scaffoldGraphHead+i)->scaffoldGraph, scaffoldCount, inputArg[i].readLength, inputArg[i].insertsize, inputArg[i].std, inputArg[i].insertsize+lambda*inputArg[i].std, inputArg[i].edgeMeanMethod);
        ScaffoldSet * tempScaffoldSet = OptimizeScaffoldSet(contigSet, scaffoldSet, (scaffoldGraphHead+i)->scaffoldGraph, scaffoldCount, contigCount, contigLength, inputArg[i].insertsize, inputArg[i].std, lambda);
        if(tempScaffoldSet != NULL){
            scaffoldSet = tempScaffoldSet;
        }
    }

    OutPutScaffoldSet(scaffoldSet, contigSet, contigCount, argv[argc-1]);
    time_t timep1;
    time(&timep1);
    ocout<<"BOSS end time:"<<asctime(gmtime(&timep1))<<endl; 
    ocout<<"AllTime:"<<difftime(timep1, timep)<<endl;
    
}

