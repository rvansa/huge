/*
 * IndexJob.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: radim
 */

#include "tre.h"
#include "IndexJob.h"
#include <stdio.h>

IndexJob::IndexJob() {
	// TODO Auto-generated constructor stub

}

IndexJob::~IndexJob() {
	// TODO Auto-generated destructor stub
}



void IndexJob::run() {
	regex_t r;
	if (tre_regcomp(&r, "(foo)|(bar)", REG_EXTENDED | REG_NEWLINE)) {
		printf("Compile failed\n");
	} else {
		printf("Compile succeeded\n");
	}
	regmatch_t match[10];
	if (tre_regexec(&r, "foxbarfoo", 10, match, 0)) {
		printf("Search failed\n");
	} else {
		for (int i = 0; i < 10; ++i)
			printf("Found on %ld-%ld\n", match[i].rm_so, match[i].rm_eo);


	}
	printf("ENough\n");
}

