# Description
Program to help with repairs playlists with format XSPF.

# Functions
1. 🧐🔗 Analysis for broken paths. The program helps identify any broken paths within the XSPF playlist. 
This can be useful for finding and fixing paths that are no longer valid or accessible.
2. 🔄📂 Simple replacing paths(!There are no other folders in the folder!). It allows you to replace paths within the 
playlist to a specific directory in which all files without sub folders are located. This is handy if you want to fix broken paths ones.
3. 🔄📂 Smart replacing paths(!There are other folders in the folder!). Performs intelligent path replacement. 
It scans all sub folders within a specified folder and searches for file names. 
If a matching file name is found, it updates the file path in the corresponding XSPF file.
4. 📃🔍 Checking 2 files XSPF for difference. The program can compare two XSPF files and highlight the differences between them. 
This feature helps you identify any additions, removals, or modifications made between the two versions.
5. 📂➕💾 Create a new file with new files by difference. 
It offers the ability to create a new XSPF file that contains the files present in one version but not in another. 
This feature helps you keep your playlist up to date by adding any new files that might have been added since the last update.

# Realisation/examples
1. For "analysis for broken paths" will created `broken_link.txt` file with report.
2. For "simple replacing paths" will create new one with added `_edited` text in name of file.
3. 