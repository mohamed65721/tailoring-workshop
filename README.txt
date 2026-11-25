================================================================================
  CS212 Data Structures - Project 94: Tailoring Order Workshop
  README: how to compile and run
================================================================================

WHAT THIS PROJECT IS
--------------------
A menu-driven C++17 console application for running a tailoring workshop.
All four data structures are implemented by hand (no STL containers):

  Stack ........ recently finished alterations (newest on top, for rework)
  Queue ........ garments awaiting sewing (circular array, capacity 8)
  Linked List .. customer records and body measurements
  BST .......... order lookup by unique order ID (single source of truth)

Sorting algorithm: merge sort on the customer linked list (menu option 6).


FOLDER LAYOUT
-------------
  README.txt     this file
  README.md      the same information, shown on the GitHub page
  Source_Code/   all .cpp/.h files, the Visual Studio solution,
                 tailor.exe, run.bat, build.bat
  Report/        project report
  Screenshots/   screenshots of the program running
  Tests/         test plan (TEST_PLAN.md), test input scripts (*.txt)
                 and their captured outputs (*.out.txt)


HOW TO RUN
----------
Option 1 - Ready-made program (Windows)
  1. Download the repository (Code -> Download ZIP) and extract it.
  2. Open Source_Code and double-click run.bat (or tailor.exe).
  3. Choose 20 in the menu to load sample data, then try other options.

Option 2 - Visual Studio 2022
  1. Open Source_Code\TailoringWorkshop.sln
  2. Press Ctrl + F5 to build and run.

Option 3 - Command line, g++ (MinGW / Linux / macOS)
  cd Source_Code
  g++ -std=c++17 -Wall -Wextra -o tailor main.cpp Workshop.cpp OrderBST.cpp CustomerList.cpp InputUtils.cpp
  ./tailor

Option 4 - Command line, MSVC (Developer Command Prompt for VS)
  cd Source_Code
  cl /std:c++17 /EHsc /W4 main.cpp Workshop.cpp OrderBST.cpp CustomerList.cpp InputUtils.cpp /Fe:tailor.exe
  tailor.exe

You can also double-click Source_Code\build.bat. It rebuilds tailor.exe
with g++ if installed, otherwise with Visual Studio.


RUNNING THE SCRIPTED TESTS
--------------------------
Each file in Tests\ is a list of menu inputs. Feed it to the program:

  cd Source_Code
  tailor < ..\Tests\t2_end_to_end.txt

(In PowerShell: cmd /c "tailor.exe < ..\Tests\t2_end_to_end.txt")
Compare the output with the matching *.out.txt file.
Test cases and results are listed in Tests\TEST_PLAN.md.


MENU OVERVIEW
-------------
  Customers (linked list): 1 Add, 2 List, 3 Find, 4 Update, 5 Delete,
                           6 Sort (merge sort)
  Orders (BST):            7 Create, 8 Find, 9 List inorder,
                           10 Show tree preorder, 11 Update,
                           12 Correct ID, 13 Delete
  Workflow:                14 View queue, 15 Sew next,
                           16 View finished stack, 17 Rework latest,
                           18 Pickup, 19 Summary
  Other:                   20 Load sample data, 0 Exit

All names and phone numbers in the sample data are made up.
================================================================================
