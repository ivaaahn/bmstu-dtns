#ifndef _IO_INTERFACE_H_
#define _IO_INTERFACE_H_


typedef enum {
    ch_exit,
    ch_balance,
    ch_insert_avl,
    ch_remove_avl,
    ch_exist_avl,
    ch_export_avl,
    ch_destroy_avl,
    ch_fread_avl,
    ch_insert_bst,
    ch_remove_bst,
    ch_exist_bst,
    ch_export_bst,
    ch_destroy_bst,
    ch_fread_bst,
    ch_insert_ht,
    ch_remove_ht,
    ch_exist_ht,
    ch_export_ht,
    ch_destroy_ht,
    ch_check_restruct,
    ch_restruct,
    ch_fread_ht,
    ch_randomizer_other,
    ch_rightOrder_other,
    ch_import_all_other,
    ch_analyze_other,
    ch_count,
    ch_error
} choice_t;


int menuHandler();
void printMenu();


#endif  // _IO_INTERFACE_H_