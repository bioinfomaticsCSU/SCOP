# SCOP(a novel scaffolding algorithm based on contigs classification and optimization)

License
=========

Copyright (C) 2017  Min Li(limin@mail.csu.edu.cn), Li Tang(tangli_csu@csu.edu.cn)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.

Min Li(limin@mail.csu.edu.cn), Li Tang(tangli_csu@csu.edu.cn)
School of Information Science and Engineering
Central South University
ChangSha
CHINA, 410083


Scaffolder: SCOP
=================

1) Introduction

	SCOP is a scaffolder which utilizes the classification of vertices and the alignment information of edges to optimize scaffold graph. 
	The contigs can be produced by any assembler.
	The input read data of SCOP is paired-end or Mate-pair read set, in which the left read library and the right read library should have the same read length.

2) Before installing and running
```	
	Please build and install bamtools from https://github.com/pezmaster31/bamtools. And add enviroment vairable BAMTOOLS_HOME which is the path of bamtools.
	Please install Python2.7.11, which can be downloaded from https://www.python.org/download/releases/2.7/. And make sure python2.7 is installed in the system default path.  
	Please install Cython-0.25.2, which can be downloadedfrom https://pypi.python.org/pypi/Cython/0.25.2.
	Please install pysam-0.8.4, which can be downloaded from https://pypi.python.org/pypi/pysam/0.8.4. 
	Please install Bowtie2, which can be downloaded from https://sourceforge.net/projects/bowtie-bio/files/bowtie2/2.3.2. And add the path of Bowtie2 into the system environment variable.
	Please install Samtools, which can be downloaded from https://sourceforge.net/projects/samtools/files/samtools/. And add the path of samtools into the system environment variable.
```
3) Installing
```
	SCOP should run on Linux operating sysetm with gcc. We test SCOP using gcc4.6.3 on Ubuntu.
	Create a main directory (eg:SCOP). Copy all source code to this directory.
	cd SCOP
	export BAMTOOLS_HOME=/path_bamtools/
	cp /path_bamtools/lib/* /usr/lib/
	make all
```
4) Running
```
	Run command line: 
	"SCOP <contigs.fa> <paired_left.fastq> <paired_right.fastq> <read_length> <insert_size> <std> <min_weight> <min_number> <is_paired_end> <edge_weight_method> <scaffold_file_name>"
	<contigs.fa>: 
		The file includes contigs produced by one assembler.
	<paired_left.fastq>:
		The left read library of paired-end or Mate-pair read set.
	<paired_right.fastq>:
		The right read library of paired-end or Mate-pair read set.
	<read_length>: 
		The length of read.
	<insert_size>: 
		The insert size of read library.
	<std_percentage>: 
		The percentage of standard deviation to insert size, std = insert_size*std_percentage. In default, std_percentage = 0.07.
	<min_weight>: 
		One cutoff for removing suprious edgs in the scaffold graph. In default, min_weight = 0.2. If the coverage is larger than 100, the value of min_weight can set to be 0.3 or more large. Note that, min_weight is smaller than 1.
	<min_number>: 
		The minimum number of links between contigs. In default, min_number = 2. If the coverage is larger than 100, the value of min_number can set to be 3 or more large.
	<is_paired_end>: 
		It is equal to 0 or 1, 1 represents that read library is paired-end library, 0 represents that read library is mate-paired library.
	<edge_weight_method>: 
		It is equal to 0 or 1, 0 represents that the edge weight calculated by arithmetic mean, 1 represents that the edge weight calculated by geometric mean.
	<scaffold_file_name>: 
		The output file name, this file includes scaffolds produced by SCOP. 
```
5) Example
```
	(a). If there is one read library:
	./SCOP contigs.fa paired_left.fastq paired_right.fastq 76 650 0.07 0.2 2 0 0 result
	This command will produce the scaffolding result: result_ScaffoldSet.fa
	
	(b). If there are two read libraries:
	./SCOP contigs.fa paired_left1.fastq paired_right1.fastq 76 650 0.07 0.2 2 0 0 paired_left2.fastq paired_right2.fastq 75 2700 0.07 0.2 2 0 0 result_com
	This command will produce the scaffolding result: result_com_ScaffoldSet.fa
```
6) Suggestion
```
	When you want to enhance the accuracy of scaffolding results, please set large numbers for the parameters  <min_weight> and <min_number>;
	When the coverage number of paired read library is small (smaller than 30), you can try small numbers for the parameters <min_weight> and <min_number>;   
```
7) Evaluation
```
	You can use the tool  QUAST (http://bioinf.spbau.ru/quast) or GAGE (in the directory GAGE) to evaluate results. 
```