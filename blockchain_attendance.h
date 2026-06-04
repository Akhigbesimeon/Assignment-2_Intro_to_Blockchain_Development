#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Configuration constants
#define MAX_STUDENTS 5
#define DEFAULT_DIFFICULTY 2
#define REWARD_PRESENT 10
#define REWARD_LATE 5
#define REWARD_ABSENT 0
#define TX_FEE 1

// Data enumerations 
typedef enum { ABSENT, LATE, PRESENT } AttendanceStatus;
typedef enum { MODEL_UTXO, MODEL_ACCOUNT } LedgerModel;

// UTXO structure
typedef struct UTXO {
    char utxo_id[65];        
    char owner_id[20];       
    int amount;
    int is_spent;            
    struct UTXO* next;
} UTXO;

// Account structure
typedef struct TxLogNode {
    char sender[20];
    char recipient[20];
    int amount;
    int fee;
    int nonce;
    struct TxLogNode* next;  
} TxLogNode;

typedef struct Account {
    char student_id[20];
    char student_name[50];
    int balance;
    int nonce;
    TxLogNode* history_head; 
} Account;

// Block structure
typedef struct Block {
    int index;
    long timestamp;
    char student_id[20];
    char student_name[50];
    AttendanceStatus status;
    int token_reward;         
    char tx_id[65];           
    char prev_hash[65];
    char hash[65];
    int nonce;                
    struct Block* next;
} Block;

// Global variables
extern LedgerModel active_model;
extern Account student_registry[MAX_STUDENTS];
extern UTXO* utxo_head;
extern Block* blockchain_head;
extern Block* pending_pool_head;

// Core functions
void init_system();
void compute_mock_hash(const char* input, char* output);
void generate_tx_hash(int index, const char* student_id, int reward, char* output);
void generate_block_hash(Block* b, char* output);

// Attendance functions
void mark_attendance(const char* student_id, AttendanceStatus status);
void process_ledger_updates(Block* b);

// Ledger management
int calculate_utxo_balance(const char* student_id);
void display_utxo_ledger();
int process_utxo_transfer(const char* sender_id, const char* recipient_id, int amount, int fee);
int process_account_transfer(const char* sender_id, const char* recipient_id, int amount, int fee, int submitted_nonce);
void print_student_history(const char* student_id);
void display_balances();

// Mining simulation
void mine_solo(int difficulty);
void mine_pool();
void mine_cloud(int rounds);

// Utilities
void print_pending_pool();
void print_blockchain();

#endif
