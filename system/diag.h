/*
 * diag.h
 *
 *  Created on: 30 Apr 2017
 *      Author: osboxes
 */
#pragma once

#include <iostream>
#include<set>
#include<string>
#include "global.h"
#include <atomic>
#include "tpcc_query.h"

class Diag {
public:
	void 			init();
	bool 			insert_key_order_line(idx_key_t key,uint64_t wh);
	uint64_t 		get_order_line_set_size(uint64_t wh);
	uint64_t		check_orderlineDb_size_per_wh(INDEX * index, uint64_t wh);
	void			record_mgr(base_query * m_query,uint64_t thd_id);
	void			record_orders(base_query * m_query,uint64_t thd_id);
	std::atomic<int> order_lines_counter[16];

	// returns the next timestamp.

private:
	pthread_mutex_t mgr_lock;
	set<idx_key_t>  * order_line_keys;

};


