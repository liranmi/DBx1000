#ifndef _REMOTE_QUERY_H_
#define _REMOTE_QUERY_H_

#include "global.h"
#include "helper.h"
#include "query.h"

#define MAX_DATA_SIZE 100 
class tpcc_query;
class base_query;


// different for different CC algos?
// read/write
/*
struct r_query {
public:
	RemReqType rtype;
	uint64_t len;
	uint64_t return_id;
	uint64_t txn_id;
	uint64_t part_cnt;
	uint64_t * parts;
	char * data;
};
*/


class Remote_query {
public:
	void init(uint64_t node_id, workload * wl);
	void remote_qry(base_query * query, int type, int dest_id);
	void signal_end();
	void send_remote_query(uint64_t dest_id, void ** data, int * sizes, int num, uint64_t tid);
	void send_remote_rsp(uint64_t dest_id, void ** data, int * sizes, int num, uint64_t tid);
	void unpack(base_query * query, void * d, int len);
	int q_idx;
	/*
#if WORKLOAD == TPCC
	tpcc_query * queries;
#endif
*/
private:
	pthread_cond_t cnd;
	pthread_mutex_t mtx;
	
	uint64_t _node_id;
	workload * _wl;

};
#endif