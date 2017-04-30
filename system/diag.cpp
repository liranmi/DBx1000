/*
 * diag.cpp
 *
 *  Created on: 30 Apr 2017
 *      Author: osboxes
 */
#include "diag.h"
#include "manager.h"
#include "tpcc_helper.h"
void Diag::init()
{
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


