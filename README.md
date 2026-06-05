
# Blockchain-Based Attendance System

An advanced decentralized attendance ledger tracking system. This project extends standard immutable record keeping by implementing automated token economic incentives, transaction execution queues via a Memory Pending Pool, cryptographic Proof-of-Work configurations, and multi-paradigm transaction settlement models.

## Compilation and Setup Instructions

### System Dependencies
The core engine has been explicitly written utilizing standard ANSI C libraries (`stdio.h`, `stdlib.h`, `string.h`, `time.h`). No external cryptographic dependencies or third-party packages (such as OpenSSL) are required. The environment handles internal transaction verification and block cryptographic validation signatures via an optimized internal mock hashing mechanism.

### Build Instructions
Compile the modular program architecture natively using `gcc`:

```bash
gcc -Wall -O2 blockchain_attendance.c main.c -o attendance_blockchain
```

# Running the Application
Execute the compiled binary execution container using the following terminal script command:

```bash
./attendance_blockchain
```

# Configuration Guides
How to Switch Between Transaction Models
The system boots cleanly by giving the user an interactive prompt at initialization to choose the session-wide accounting engine logic.

* **The Prompt:** Upon initial startup, the terminal presents a dedicated configuration choice:

```bash
Select Core System Transaction Model:
1) UTXO Model
2) Account-Balance Model
Choice: 
```
* **Runtime Behavior:** Selecting 1 targets the UTXO (Unspent Transaction Output) engine mode globally. Selecting 2 activates the direct state Account-Balance database ruleset configuration. This state will actively bind across all internal ledger validations during that execution session.

# How to Set Mining Difficulty Level
When choosing Solo Mining operations, the complexity metric can be scaled on demand.

* Navigate to the primary workspace menu and choose option 3) Execute Solo Miner Processing Routine.

* The application will pause to request an assignment scale:

`Configure Target Search Leading Complexity Scale (1-4):`
* Entering a value changes the target number of leading zeros (0) required in the cryptographic hash prefix.

* **Note:** A difficulty level of 1 or 2 matches standard educational verification constraints. Difficulty layers of 4 dramatically increase the CPU calculation runtime loops needed to find a valid nonce.

# Testing Mining Simulations & Edge Cases
To verify compliance across all core grading criteria within your technical report submission, follow these step-by-step testing scripts:

### Test Flow A: Attendance Accumulation & The Pending Pool

* Boot up the framework and choose 1 (UTXO Model) or 2 (Account Model).

* Select menu option 1 to Process Student Attendance Entry. Input target ID STU001 and status code 2 (PRESENT).

* Select option 1 again. Input target ID STU002 and status code 0 (ABSENT).

* Run menu option 2 to Print Pending Transaction Memory Pool Status.

* Verify that STU001 shows an unconfirmed record waiting with a 10 Coin reward attached, while STU002 shows an active record tracking a 0 Coin transaction balance (verifying the Absent Student No-Transaction Edge Case).

### Test Flow B: Solo Mining Verification
* From the main screen selection loop, run option 3.

* Input a target difficulty target metric of 2.

* The engine will loop through blocks in the pending validation queue, repeatedly hashing incremental nonces until a match is confirmed.

* Run option 6 (Review System Ledger State and Balance Tables) to verify that the block has safely dropped out of the pending memory buffer and migrated over into the permanent chain structure.

### Test Flow C: Pool Mining Contribution Breakdown
* Insert multiple attendance nodes into the system memory loop using menu option 1.

* Select option 4 (Execute Pool Miner Simulation Framework).

* The program will output a clean console summary demonstrating shared mining metrics:

     * Automatic calculation of proportional work parameters across multiple sub-workers.

     *  Deduction of the strict 2% pool operating management surcharge.

     *  Exact breakdown distributions of block rewards relative to generated hash-rate performance metrics.

### Test Flow D: Cloud Mining Unprofitability Warn Guardrails
* Select menu option 5 to enter Simulate Multi-Round Cloud Mining Operations.
* Define a target lease parameter scope of 5 rounds.
* Because the operational fee configuration outpaces standard cloud computing block yields `($6.0\text{ fee} > 5.5\text{ yield}$)`, look for the terminal to print out the explicit security guardrail verification warning:
`WARNING: Cloud allocation processes are currently UNPROFITABLE in running round X!`

### Test Flow E: Manual Account Transfers & Nonce Security (Account Model Only)
* **Prerequisite:** Start the software by choosing the Account-Balance Model. Mine a block for student STU001 first, so they have a spendable asset index balance.
* Select option 7 to enter the custom transaction sandbox testing loop.
* Attempt to run a transfer transaction from STU001 to STU002 using an invalid tracking variable (e.g, inputting a value of 99 when the account's internal nonce is sitting at 0).
* Look for the system to throw an automated execution rejection flag: `REJECTED: Nonce mismatch!`.

# Structural Code Mapping
* `blockchain_attendance.h`: Modular header container enclosing primary runtime parameters, architectural state enumerations, structural node setups (Block, UTXO, Account, TxLogNode), and target function signatures.

* `blockchain_attendance.c`: Cryptographic validation engine, system memory pool managers, ledger verification logic, and multi-miner processing blocks.

* `main.c`: Core driver interface handling user interaction, menu loops, and input exception routing variables.
