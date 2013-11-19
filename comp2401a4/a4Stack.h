/*
 * a4Stack.h
 *
 *  Created on: Nov 14, 2013
 *      Author: jon
 */

#ifndef A4STACK_H_
#define A4STACK_H_

void add(MovieNodeType **, MovieNodeType *);
void removeNode(MovieNodeType *);
void freeNodeAndData(MovieNodeType *);
int removeByName(MovieNodeType **, char *);

#endif /* A4STACK_H_ */
