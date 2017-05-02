/*
 * diag.cpp
 *
 *  Created on: 30 Apr 2017
 *      Author: osboxes
 */
#include "diag.h"
#include "manager.h"
#include "row.h"
#include "tpcc_helper.h"
#include "tpcc.h"
#include "tpcc_const.h"

void Diag::init()
{
	 if (pthread_mutex_init(&mgr_lock, NULL) != 0){
	        printf("\n mutex init failed\n");
	    }
	order_line_keys = new set<idx_key_t>[g_num_wh + 1];
	for (int j = 0 ;j<16 ; j++)
		order_lines_counter[j] = 0;
}

bool Diag::insert_key_order_line(idx_key_t key,uint64_t wh){
	return order_line_keys[wh].insert(key).second;
}

uint64_t Diag::get_order_line_set_size(uint64_t wh){
	return order_line_keys[wh].size();
}

uint64_t Diag::check_orderlineDb_size_per_wh(INDEX * index, uint64_t wh){
	uint64_t counter = 0;
	set <idx_key_t>::iterator it;
	txn_man * txn = glob_manager->get_txn_man(0);
	for(it = order_line_keys[wh].begin(); it != order_line_keys[wh].end(); it++){
		itemid_t * item = txn->index_read(index, *it, wh_to_part(wh));
		while(item != NULL){
			counter++;
			item = item->next;
		}
	}
	return counter;
}

void	Diag::record_mgr(base_query * m_query,uint64_t thd_id){
#if WORKLOAD == TPCC
	tpcc_query * query = (tpcc_query *) m_query;
	switch (query->type) {
		case TPCC_PAYMENT :
			break;
		case TPCC_NEW_ORDER :
			return record_orders(m_query,thd_id); break;
		default:
			break;
	}
#endif
}

void	Diag::record_orders(base_query * m_query,uint64_t thd_id){
#if WORKLOAD == TPCC
	txn_man * txn = glob_manager->get_txn_man(thd_id);
	tpcc_query * query = (tpcc_query *) m_query;
	uint64_t w_id = query->w_id;
    uint64_t d_id = query->d_id;
    uint64_t c_id = query->c_id;
	uint64_t ol_cnt = query->ol_cnt;
	int64_t o_id = query->rec_oid;
	pthread_mutex_lock(&mgr_lock);
	for (uint64_t ol = 1 ; ol <= ol_cnt;ol++){
		if (glob_diag->insert_key_order_line(orderlineKey(w_id,d_id,o_id,ol),w_id - 1) == false)
			cout << "duplicated key = " << orderlineKey(w_id,d_id,o_id,ol) << endl;
		glob_diag->order_lines_counter[w_id - 1]++;
	}
	pthread_mutex_unlock(&mgr_lock);
#endif
}

