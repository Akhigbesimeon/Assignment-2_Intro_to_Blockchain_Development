#include "blockchain_attendance.h"

// Global variables
LedgerModel active_model = MODEL_UTXO;
Account student_registry[MAX_STUDENTS];
UTXO* utxo_head = NULL;
Block* blockchain_head = NULL;
Block* pending_pool_head = NULL;

// Mock Hashing Algorithm
void compute_mock_hash(const char* input, char* output) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*input++)) {
        hash = ((hash << 5) + hash) + c;
    }
    sprintf(output, 65, "00000000000000000000000000000000000000000000000000000000%08lx", hash);
}

void generate_tx_hash(int index, const char* student_id, int reward, char* output) {
    char buffer[512]; 
    snprintf(buffer, sizeof(buffer), "TX:%d-%s-%d-%ld", index, student_id, reward, (long)time(NULL));
    compute_mock_hash(buffer, output);
}

void generate_block_hash(Block* b, char* output) {
    char buffer[1024]; 
    snprintf(buffer, sizeof(buffer), "%d-%ld-%s-%d-%d-%s-%s-%d", 
            b->index, b->timestamp, b->student_id, (int)b->status, 
            b->token_reward, b->tx_id, b->prev_hash, b->nonce);
    compute_mock_hash(buffer, output);
}

// System initialization 
void init_system() {
    for(int i = 0; i < MAX_STUDENTS; i++) {
        snprintf(student_registry[i].student_id, sizeof(student_registry[i].student_id), "STU00%d", i + 1);
        
        switch(i) {
            case 0: snprintf(student_registry[i].student_name, sizeof(student_registry[i].student_name), "Alice"); break;
            case 1: snprintf(student_registry[i].student_name, sizeof(student_registry[i].student_name), "Bob"); break;
            case 2: snprintf(student_registry[i].student_name, sizeof(student_registry[i].student_name), "Charlie"); break;
            case 3: snprintf(student_registry[i].student_name, sizeof(student_registry[i].student_name), "David"); break;
            case 4: snprintf(student_registry[i].student_name, sizeof(student_registry[i].student_name), "Ethan"); break;
            default: snprintf(student_registry[i].student_name, sizeof(student_registry[i].student_name), "Unknown"); break;
        }
        
        student_registry[i].balance = 0;
        student_registry[i].nonce = 0;
        student_registry[i].history_head = NULL;
    }
}

// Mark attendance and place in pending pool
void mark_attendance(const char* student_id, AttendanceStatus status) {
    int found = -1;
    for(int i = 0; i < MAX_STUDENTS; i++) {
        if(strcmp(student_registry[i].student_id, student_id) == 0) {
            found = i;
            break;
        }
    }
    if(found == -1) {
        printf("ERROR: Student ID %s not registered.\n", student_id);
        return;
    }

    int reward = (status == PRESENT) ? REWARD_PRESENT : ((status == LATE) ? REWARD_LATE : REWARD_ABSENT);
    
    Block* new_block = (Block*)malloc(sizeof(Block));
    if (new_block == NULL) return;

    new_block->index = (blockchain_head == NULL) ? 1 : blockchain_head->index + 1;
    new_block->timestamp = (long)time(NULL);
    
    // Use snprintf instead of raw strcpy for safety
    snprintf(new_block->student_id, sizeof(new_block->student_id), "%s", student_id);
    snprintf(new_block->student_name, sizeof(new_block->student_name), "%s", student_registry[found].student_name);
    
    new_block->status = status;
    new_block->token_reward = reward;
    new_block->nonce = 0;
    new_block->next = NULL;

    if(reward > 0) {
        generate_tx_hash(new_block->index, student_id, reward, new_block->tx_id);
    } else {
        snprintf(new_block->tx_id, sizeof(new_block->tx_id), "0000000000000000000000000000000000000000000000000000000000000000");
    }

    if(blockchain_head == NULL) {
        snprintf(new_block->prev_hash, sizeof(new_block->prev_hash), "0000000000000000000000000000000000000000000000000000000000000000");
    } else {
        snprintf(new_block->prev_hash, sizeof(new_block->prev_hash), "%s", blockchain_head->hash);
    }

    // Append to pending pool list
    if(pending_pool_head == NULL) {
        pending_pool_head = new_block;
    } else {
        Block* temp = pending_pool_head;
        while(temp->next != NULL) temp = temp->next;
        temp->next = new_block;
    }
    printf("SUCCESS: Attendance entry for %s put into Pending Pool.\n", student_registry[found].student_name);
}

// Core ledger settlement rules engine
void process_ledger_updates(Block* b) {
    if(b->token_reward == 0) return; 

    if(active_model == MODEL_UTXO) {
        UTXO* new_utxo = (UTXO*)malloc(sizeof(UTXO));
        if (new_utxo == NULL) return;
        snprintf(new_utxo->utxo_id, sizeof(new_utxo->utxo_id), "%s", b->tx_id);
        snprintf(new_utxo->owner_id, sizeof(new_utxo->owner_id), "%s", b->student_id);
        new_utxo->amount = b->token_reward;
        new_utxo->is_spent = 0;
        new_utxo->next = utxo_head;
        utxo_head = new_utxo;
    } else {
        for(int i = 0; i < MAX_STUDENTS; i++) {
            if(strcmp(student_registry[i].student_id, b->student_id) == 0) {
                student_registry[i].balance += b->token_reward;
                break;
            }
        }
    }
}

// UTXO balancing metrics logic
int calculate_utxo_balance(const char* student_id) {
    int balance = 0;
    UTXO* curr = utxo_head;
    while(curr != NULL) {
        if(strcmp(curr->owner_id, student_id) == 0 && curr->is_spent == 0) {
            balance += curr->amount;
        }
        curr = curr->next;
    }
    return balance;
}

void display_utxo_ledger() {
    printf("\nGLOBAL ACTIVE UTXO SET\n");
    UTXO* curr = utxo_head;
    int items = 0;
    while(curr != NULL) {
        if(curr->is_spent == 0) {
            printf("Owner: %s | Value: %d Coins | Hash ID: %.12s...\n", curr->owner_id, curr->amount, curr->utxo_id);
            items++;
        }
        curr = curr->next;
    }
    if(items == 0) printf("[System]: No Active UTXOs currently exist.\n");
}

// UTXO transfer handler
int process_utxo_transfer(const char* sender_id, const char* recipient_id, int amount, int fee) {
    int required = amount + fee;
    int gathered = 0;
    UTXO* curr = utxo_head;

    if (calculate_utxo_balance(sender_id) < required) {
        printf("[REJECTED]: Insufficient UTXO balance. Need %d\n", required);
        return 0;
    }

    curr = utxo_head;
    while (curr != NULL && gathered < required) {
        if (strcmp(curr->owner_id, sender_id) == 0 && curr->is_spent == 0) {
            curr->is_spent = 1; 
            gathered += curr->amount;
        }
        curr = curr->next;
    }

    UTXO* rx_utxo = (UTXO*)malloc(sizeof(UTXO));
    if (rx_utxo == NULL) return 0;
    snprintf(rx_utxo->utxo_id, sizeof(rx_utxo->utxo_id), "TX-TRANSFER-%ld", (long)time(NULL));
    snprintf(rx_utxo->owner_id, sizeof(rx_utxo->owner_id), "%s", recipient_id);
    rx_utxo->amount = amount;
    rx_utxo->is_spent = 0;
    rx_utxo->next = utxo_head;
    utxo_head = rx_utxo;

    int change = gathered - required;
    if (change > 0) {
        UTXO* change_utxo = (UTXO*)malloc(sizeof(UTXO));
        if (change_utxo != NULL) {
            snprintf(change_utxo->utxo_id, sizeof(change_utxo->utxo_id), "TX-CHANGE-%ld", (long)time(NULL));
            snprintf(change_utxo->owner_id, sizeof(change_utxo->owner_id), "%s", sender_id);
            change_utxo->amount = change;
            change_utxo->is_spent = 0;
            change_utxo->next = utxo_head;
            utxo_head = change_utxo;
            printf("[UTXO SYSTEM]: Generated change output of %d Coins back to %s\n", change, sender_id);
        }
    }
    return 1;
}

// Account transfer logic 
int process_account_transfer(const char* sender_id, const char* recipient_id, int amount, int fee, int submitted_nonce) {
    int s_idx = -1, r_idx = -1;
    for(int i = 0; i < MAX_STUDENTS; i++) {
        if(strcmp(student_registry[i].student_id, sender_id) == 0) s_idx = i;
        if(strcmp(student_registry[i].student_id, recipient_id) == 0) r_idx = i;
    }

    if(s_idx == -1 || r_idx == -1) return 0;
    if(submitted_nonce != student_registry[s_idx].nonce) return 0; 
    if(student_registry[s_idx].balance < (amount + fee)) return 0; 

    student_registry[s_idx].balance -= (amount + fee);
    student_registry[r_idx].balance += amount;

    TxLogNode* node = (TxLogNode*)malloc(sizeof(TxLogNode));
    if (node == NULL) return 0;
    snprintf(node->sender, sizeof(node->sender), "%s", sender_id);
    snprintf(node->recipient, sizeof(node->recipient), "%s", recipient_id);
    node->amount = amount;
    node->fee = fee;
    node->nonce = submitted_nonce;
    node->next = student_registry[s_idx].history_head;
    student_registry[s_idx].history_head = node;

    student_registry[s_idx].nonce++;
    return 1;
}

void print_student_history(const char* student_id) {
    for(int i = 0; i < MAX_STUDENTS; i++) {
        if(strcmp(student_registry[i].student_id, student_id) == 0) {
            printf("\nHISTORY FOR %s (%s)\n", student_registry[i].student_name, student_id);
            TxLogNode* curr = student_registry[i].history_head;
            while(curr != NULL) {
                printf("[Tx -> %s] Amount: %d | Fee: %d | Used Nonce: %d\n", curr->recipient, curr->amount, curr->fee, curr->nonce);
                curr = curr->next;
            }
            return;
        }
    }
}

void display_balances() {
    printf("\nSTUDENT BALANCE LOGS\n");
    for(int i = 0; i < MAX_STUDENTS; i++) {
        int bal = (active_model == MODEL_UTXO) ? calculate_utxo_balance(student_registry[i].student_id) : student_registry[i].balance;
        printf("ID: %s | Name: %-10s | Total Balance: %d Coins\n", student_registry[i].student_id, student_registry[i].student_name, bal);
    }
}

// Solo mining implementation
void mine_solo(int difficulty) {
    if(pending_pool_head == NULL) {
        printf("System Alert: Pending pool is empty. Nothing to mine.\n");
        return;
    }

    if(difficulty < 1 || difficulty > 4) difficulty = DEFAULT_DIFFICULTY;

    char target[10] = {0};
    for(int i = 0; i < difficulty; i++) target[i] = '0';

    printf("\nMINING: Commencing Solo Mining Cycle with difficulty target %d...\n", difficulty);

    while(pending_pool_head != NULL) {
        Block* current_work = pending_pool_head;
        pending_pool_head = pending_pool_head->next; 

        int attempts = 0;
        char hash_res[65];

        while(1) {
            attempts++;
            generate_block_hash(current_work, hash_res);
            if(strncmp(hash_res, target, difficulty) == 0) {
                snprintf(current_work->hash, sizeof(current_work->hash), "%s", hash_res);
                break;
            }
            current_work->nonce++;
        }

        printf(" -> Block #%d Confirmed! Total Hash Computations: %d\n", current_work->index, attempts);
        process_ledger_updates(current_work);

        current_work->next = blockchain_head;
        blockchain_head = current_work;
    }
}

// Pool mining implementation
void mine_pool() {
    if(pending_pool_head == NULL) {
        printf("System Alert: Pool empty. Action Cancelled.\n");
        return;
    }

    int rates[3] = {450, 350, 200}; 
    int total_hashrate = 1000;
    int base_mining_reward = 20; 
    
    double fee_deducted = base_mining_reward * 0.02;
    double net_reward_pool = base_mining_reward - fee_deducted;

    printf("\nCOOPERATIVE POOL MINING METRICS LOG \n");
    printf("%-10s | %-12s | %-10s | %-15s\n", "Miner ID", "Hash Attempts", "Share %", "Reward Earned");
    printf("------------------------------------------------------------\n");

    for(int i = 0; i < 3; i++) {
        double share_percentage = ((double)rates[i] / total_hashrate) * 100.0;
        double individual_payout = (rates[i] / (double)total_hashrate) * net_reward_pool;
        printf("Miner_0%d   | %-12d | %-8.1f%% | %.2f Coins\n", i+1, rates[i], share_percentage, individual_payout);
    }
    printf("System Information: Deducted structural Pool operator fee: %.2f\n", fee_deducted);

    mine_solo(1);
}

// Cloud mining simulation engine
void mine_cloud(int rounds) {
    if(rounds < 1 || rounds > 5) rounds = 3;

    double running_gross = 0.0;
    double running_fees = 0.0;
    double round_fee = 6.0;
    double round_yield = 5.5; 

    printf("\nCLOUD RENTAL INFRASTRUCTURE TRACKING SUMMARY\n");
    
    for(int r = 1; r <= rounds; r++) {
        running_gross += round_yield;
        running_fees += round_fee;
        double round_net = running_gross - running_fees;

        printf("Round %d/5: Gross Payout: %.1f | Fees Charged: %.1f | Accumulated Net Status: %.1f\n", 
               r, running_gross, running_fees, round_net);
        
        if(running_fees > running_gross) {
            printf("WARNING: Cloud allocation processes are currently UNPROFITABLE in running round %d!\n", r);
        }
    }
}

// Utility functions
void print_pending_pool() {
    printf("\nUNMINED PENDING ATTENDANCE POOL POINTERS\n");
    Block* curr = pending_pool_head;
    if(curr == NULL) printf("The pending execution buffer pool is empty.\n");
    while(curr != NULL) {
        printf("Unconfirmed Index %d | Student: %s | Status: %s | Target Reward: %d Coins\n", 
               curr->index, curr->student_name, 
               (curr->status == PRESENT) ? "PRESENT" : ((curr->status == LATE) ? "LATE" : "ABSENT"), 
               curr->token_reward);
        curr = curr->next;
    }
}

void print_blockchain() {
    printf("\nMAIN CONFIRMED BLOCKCHAIN LEDGER\n");
    Block* curr = blockchain_head;
    if(curr == NULL) printf("No immutable blocks recorded yet.\n");
    while(curr != NULL) {
        printf("Block #%d [%s]\n", curr->index, curr->student_name);
        printf(" -> Status Value: %-7s | Token Reward: %d\n", 
               (curr->status == PRESENT) ? "PRESENT" : ((curr->status == LATE) ? "LATE" : "ABSENT"), curr->token_reward);
        printf(" -> Tx Reference ID: %.24s...\n", curr->tx_id);
        printf(" -> Block Signature: %.24s...\n", curr->hash);
        printf("----------------------------------------------------------------\n");
        curr = curr->next;
    }
}
