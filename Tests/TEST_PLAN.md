# Test Plan: Tailoring Order Workshop (CS212 Project 94)

There are three scripted test runs. Each one feeds a file of menu choices on standard input:

```
tailor < ../Tests/t1_empty_and_invalid.txt
```

The captured output of each run is saved beside its input as `Tests/<name>.out.txt`. The "Actual" column below was copied from those outputs, which came from the MSVC build. **All test cases passed.**

| Script | Focus |
|---|---|
| `t1_empty_and_invalid.txt` | Empty structures, invalid menu input, missing IDs |
| `t2_end_to_end.txt` | End-to-end trace of one order |
| `t3_single_and_boundaries.txt` | Single-item cases, boundaries, duplicates, capacity, the two scenarios, merge sort |

---

## A. Empty-structure cases (t1)

| # | Action | Expected | Actual | Result |
|---|---|---|---|---|
| A1 | 2: list customers | Empty-state message | `No customers on file yet.` | PASS |
| A2 | 9: list orders | Empty-state message | `No orders in the system.` | PASS |
| A3 | 10: preorder view | Empty-state message | `The order tree is empty.` | PASS |
| A4 | 14: view queue | Empty message with capacity | `The sewing queue is empty (0/8).` | PASS |
| A5 | 15: sew next | Refused, no crash | `Nothing to sew: the sewing queue is empty.` | PASS |
| A6 | 16: view stack | Empty message | `No finished alterations are waiting for pickup.` | PASS |
| A7 | 17: rework on empty stack | Refused, no crash | `Nothing to rework: the finished stack is empty.` | PASS |
| A8 | 6: sort an empty list | Message | `No customers to sort.` | PASS |
| A9 | 19: summary | All counts 0, consistency OK | `Customers 0 … Consistency check : OK` | PASS |

## B. Invalid input and missing IDs (t1 and t3)

| # | Input | Expected | Actual | Result |
|---|---|---|---|---|
| B1 | Menu `abc` | Re-prompt | `Error: please enter a whole number.` | PASS |
| B2 | Menu `99` | Range error | `Error: value must be between 0 and 20.` | PASS |
| B3 | Menu (empty line) | Re-prompt | `Error: please enter a whole number.` | PASS |
| B4 | Find, pick up, delete, update or correct order `555` | Not-found error | `Error: no order with ID 555.` | PASS |
| B5 | Find or delete customer `555` | Not-found error | `Error: no customer with ID 555.` | PASS |
| B6 | Create an order for customer `200`, who does not exist | Rejected | `Error: customer ID 200 does not exist. Add the customer first.` | PASS |
| B7 | Empty customer name | Re-prompt | `Error: this field cannot be empty.` | PASS |
| B8 | Chest `500` | Range error | `Error: value must be between 30 and 200.` | PASS |
| B9 | Price `abc` | Re-prompt | `Error: please enter a number (e.g. 42 or 42.5).` | PASS |
| B10 | Input ends partway through the Add Customer form (input `1`, `7`, then EOF) | Clean exit, no infinite loop | `Name:` / `(Input ended.)` / `Goodbye.` (exit code 0) | PASS |

## C. Single-item and boundary cases (t3)

| # | Action | Expected | Actual | Result |
|---|---|---|---|---|
| C1 | Add one customer and one order, then view the tree | Root-only BST, height 1 | `Nodes: 1, height: 1` / `[10] Skirt (WAITING)` | PASS |
| C2 | Delete order 10 (**root-only BST**) | Tree becomes empty; order also leaves the queue | `deleted from BST + sewing queue. Orders remaining: 0`, then `The order tree is empty.` and `queue is empty (0/8)` | PASS |
| C3 | Delete customer 1 (**only node** in the list) | List becomes empty | `Customers on file: 0`, then `No customers on file yet.` | PASS |
| C4 | Delete customer 50 (**first node** after sorting by ID) | Head removed | `Customer #50 deleted. Customers on file: 5` | PASS |
| C5 | Delete customer 105 (**last node**) | Tail removed; list still traverses correctly | `Customer #105 deleted.`; list shows #101–#104 | PASS |
| C6 | Delete order 5005 (**root with two children**) | Inorder successor 6006 becomes root | Preorder before `[5005]`, after `[6006]` at the root | PASS |
| C7 | Fill the queue to 8/8, then create order 8005 | Rejected; nothing is inserted into the BST | `Error: sewing rack is full (8 garments)…`; summary shows 8 orders and `Consistency check : OK` | PASS |
| C8 | Summary with a full queue | FULL flag shown | `Sewing queue : 8/8  (FULL)` | PASS |

## D. Duplicates and referential integrity (t3)

| # | Action | Expected | Actual | Result |
|---|---|---|---|---|
| D1 | Load sample data twice | Second load rejects every record | `customer ID 101 already exists…`, `order ID 5005 already exists…` | PASS |
| D2 | Create order `3003` again | Rejected before any other fields are asked for | `Error: order ID 3003 already exists. Duplicate IDs are rejected.` | PASS |
| D3 | Correct ID 7007 to 3003, which is taken | Rejected | `Error: order ID 3003 is already in use.` | PASS |
| D4 | Delete customer 101, who has orders | Blocked | `customer #101 still has 2 order(s). Delete those orders first…` | PASS |

## E. Scenarios and algorithm (t3)

| # | Action | Expected | Actual | Result |
|---|---|---|---|---|
| E1 | Correct ID 7007 to 7070 while it **waits in the queue** | BST re-keyed; queue keeps position 2 | `Updated: BST + sewing queue (position 2 kept).` The queue lists `2. #7070` | PASS |
| E2 | Sew 3003, then correct it to 3333 **while it is on the stack** | Stack entry rewritten in place | `Updated: BST + finished stack (position kept).`, then `1. #3333 … <- top` | PASS |
| E3 | Rework the latest finished order | 3333 popped and enqueued at the back | `re-queued for sewing (position 8)` | PASS |
| E4 | Merge sort by ID, then by name | Correct order | ID: 50, 101, 102, 103, 104, 105. Name: Ahmed, Karim, Mona, Sara | PASS |
| E5 | Final summary | Counts match across all structures | `WAITING 8 … Consistency check : OK` | PASS |

---

## F. End-to-end trace of one order (t2)

| Step | Menu | Action | Observed output |
|---|---|---|---|
| 1 | 1 | Add customer 201, Laila Mostafa | `Customer #201 (Laila Mostafa) added. Customers on file: 1` |
| 2 | 7 | Create order 9001: wedding dress, take in bodice 2 cm, 1500 EGP | `Order #9001 created and placed in the sewing queue (position 1 of 1).` |
| 3 | 8 | Look up 9001 | `9001 201 Wedding dress … 1500.00 WAITING` / `Location: sewing queue, position 1 of 1` |
| 4 | 14 | View the queue | `Sewing queue (1/8)` / `1. #9001 Wedding dress - Take in bodice 2 cm` |
| 5 | 15 | Sew next | `Sewed order #9001 … Status -> COMPLETED. Pushed onto finished stack (size 1). Queue now holds 0.` |
| 6 | 16 | View the stack | `1. #9001 … <- top` |
| 7 | 17 | Rework (fails quality control) | `Order #9001 failed quality check: popped from finished stack and re-queued for sewing (position 1).` |
| 8 | 8 | Look up 9001 | `… WAITING` / `Location: sewing queue, position 1 of 1` |
| 9 | 15 | Sew again | `Status -> COMPLETED. Pushed onto finished stack (size 1).` |
| 10 | 11 | Update: alteration becomes "+ bustle", price 1650 | `9001 201 Wedding dress Take in bodice 2 cm + bustle 1650.00 COMPLETED` |
| 11 | 16 | View the stack | The stack shows the **updated** text, because it reads from the BST |
| 12 | 18 | Pick up 9001 | `Order #9001 handed to the customer. Amount due: 1650.00 EGP. Status -> PICKED_UP.` |
| 13 | 8 | Look up 9001 | `… PICKED_UP` / `Location: delivered to customer` |
| 14 | 18 | Pick up 9001 again | `Error: order #9001 was already picked up.` |
| 15 | 19 | Summary | `Orders 1`, `queue 0/8`, `stack 0`, `PICKED_UP 1`, `Revenue collected : 1650.00 EGP`, `Consistency check : OK` |

## Observation linked to the design limitation

At the end of t3 the BST holds 8 orders but has **height 5**. Orders 8001 to 8004 were inserted in increasing order, so they form a right-leaning chain. This shows the unbalanced-BST limitation described in the project report (Report folder).
