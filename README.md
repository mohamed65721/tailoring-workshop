# Tailoring Order Workshop (CS212 Project 94)

This is a C++17 console application that runs a tailoring workshop. It keeps customer measurements on file and tracks each garment order from the sewing queue, through quality control, to customer pickup. All four data structures are written by hand. **No STL containers are used.**

| Structure | Role in the workshop |
|---|---|
| **Stack** | Recently finished alterations. The newest garment is on top, so it is the first one quality control re-checks. |
| **Queue** | Garments awaiting sewing, handled first come, first served. The rack holds 8. |
| **Linked list** | Customer records and their measurements. |
| **BST** | Order lookup by unique order ID. This is the single source of truth for order details. |

The full design, the algorithm (merge sort), the complexity analysis and the limitations are in **[Report/CS212_Tailoring_Order_Workshop_Report.pdf](Report/CS212_Tailoring_Order_Workshop_Report.pdf)** (also provided as .docx).

## Repository layout

```
README.md          <- this file: how to run the program
README.txt         <- the same instructions as plain text
Source_Code/       <- all .cpp/.h files, the Visual Studio solution, tailor.exe, run.bat, build.bat
                      (Full_Code_All_Files.txt = all 13 source files in one file, for reading)
Report/            <- project report (PDF and DOCX)
Screenshots/       <- screenshots of the program running
Tests/             <- test plan, test input scripts and their captured outputs
```

## How to run

### Option 1: run the ready-made program (Windows)
1. Download the repository: **Code → Download ZIP**, then extract it.
2. Open `Source_Code/` and double-click **`run.bat`**. You can also run `tailor.exe` directly.
3. In the menu, choose **20** to load sample data, then try the other options.

### Option 2: Visual Studio 2022
1. Open `Source_Code/TailoringWorkshop.sln`.
2. Press **Ctrl + F5** to build and run.

### Option 3: compile from the command line

g++ (MinGW, Linux or macOS):
```
cd Source_Code
g++ -std=c++17 -Wall -Wextra -o tailor main.cpp Workshop.cpp OrderBST.cpp CustomerList.cpp InputUtils.cpp
./tailor
```

MSVC (Developer Command Prompt):
```
cd Source_Code
cl /std:c++17 /EHsc /W4 main.cpp Workshop.cpp OrderBST.cpp CustomerList.cpp InputUtils.cpp /Fe:tailor.exe
tailor.exe
```

You can also double-click `Source_Code/build.bat`. It uses g++ if it is installed and Visual Studio otherwise.

### Running the scripted tests
Each test file in `Tests/` is a list of menu inputs. Feed one to the program like this:
```
cd Source_Code
tailor < ../Tests/t2_end_to_end.txt
```
In PowerShell, use `cmd /c "tailor.exe < ..\Tests\t2_end_to_end.txt"` instead. Compare what you see with the matching `*.out.txt` file. The test cases and their results are listed in **[Tests/TEST_PLAN.md](Tests/TEST_PLAN.md)**.

## Menu overview

| Area | Options |
|---|---|
| Customers (linked list) | 1 Add · 2 List · 3 Find · 4 Update · 5 Delete · 6 Sort (merge sort) |
| Orders (BST) | 7 Create · 8 Find · 9 List inorder · 10 Show tree preorder · 11 Update · 12 Correct ID · 13 Delete |
| Workflow | 14 View queue · 15 Sew next · 16 View finished stack · 17 Rework latest · 18 Pickup · 19 Summary |
| Other | 20 Load sample data · 0 Exit |

All names and phone numbers in the sample data are made up.
