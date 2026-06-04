#include "blockchain_attendance.h"

// Main function
int main() {
    init_system();
    int selection = 0;
    int nested_sel = 0;
    int system_difficulty = DEFAULT_DIFFICULTY;

    printf("  ENHANCED BLOCKCHAIN ATTENDANCE ECOSYSTEM ENGINE   \n");
    printf("Select Core System Transaction Model:\n1) UTXO Model\n2) Account-Balance Model\nChoice: ");
    
    if (scanf("%d", &selection) && selection == 2) {
        active_model = MODEL_ACCOUNT;
        printf("CONFIG: Core active set initialization switched to Account-Balance Model.\n");
    } else {
        active_model = MODEL_UTXO;
        printf("CONFIG: Core active set initialization confirmed for UTXO Model.\n");
    }

    while (1) {
        printf("\nMAIN MENU\n");
        printf("1) Process Student Attendance Entry\n");
        printf("2) Print Pending Transaction Memory Pool Status\n");
        printf("3) Execute Solo Miner Processing Routine\n");
        printf("4) Execute Pool Miner Simulation Framework\n");
        printf("5) Simulate Multi-Round Cloud Mining Operations\n");
        printf("6) Review System Ledger State and Balance Tables\n");
        printf("7) Test Internal Manual Account Transfers\n");
        printf("8) Shutdown Core System Engine Application\n");
        printf("Selection: ");
        
        if (scanf("%d", &selection) != 1) {
            while (getchar() != '\n');
            printf("ERROR: Invalid menu selection input.\n");
            continue;
        }

        switch (selection) {
            case 1: {
                char target_id[20];
                int code = 0;
                printf("\nEnter Student ID (e.g., STU001 to STU005): ");
                if (scanf("%s", target_id) != 1) break;
                printf("Select Code Status Type (0=ABSENT, 1=LATE, 2=PRESENT): ");
                if (scanf("%d", &code) != 1) break;
                mark_attendance(target_id, (AttendanceStatus)code);
                break;
            }
            case 2:
                print_pending_pool();
                break;
            case 3:
                printf("\nConfigure Target Search Leading Complexity Scale (1-4): ");
                if (scanf("%d", &system_difficulty) == 1) {
                    mine_solo(system_difficulty);
                }
                break;
            case 4:
                mine_pool();
                break;
            case 5:
                printf("\nDefine Target Cloud Allocation Run Count Length (1-5): ");
                if (scanf("%d", &nested_sel) == 1) {
                    mine_cloud(nested_sel);
                }
                break;
            case 6:
                print_blockchain();
                display_balances();
                if (active_model == MODEL_UTXO) {
                    display_utxo_ledger();
                }
                break;
            case 7: {
                char s[20], r[20];
                int amt = 0;
                printf("\nEnter Source Sender Account ID: ");
                if (scanf("%s", s) != 1) break;
                printf("Enter Target Recipient Account ID: ");
                if (scanf("%s", r) != 1) break;
                printf("Specify Asset Transfer Amount Value: ");
                if (scanf("%d", &amt) != 1) break;
                
                if (active_model == MODEL_ACCOUNT) {
                    int n = 0;
                    printf("Input Account Verification Security Nonce Value: ");
                    if (scanf("%d", &n) != 1) break;
                    
                    if (process_account_transfer(s, r, amt, TX_FEE, n)) {
                        printf("SUCCESS: Token value shift verified and cleared.\n");
                    }
                    print_student_history(s);
                } else {
                    if (process_utxo_transfer(s, r, amt, TX_FEE)) {
                        printf("SUCCESS: UTXO token transfer settled onto ledger.\n");
                    }
                }
                break;
            }
            case 8:
                printf("\nDe-allocating operational nodes. Shutting down system engine environment cleanly.\n");
                return 0;
            default:
                printf("Invalid instruction specified.\n");
        }
    }
    return 0;
}
