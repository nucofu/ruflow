this app is under development

**\*help**
```
ruflow/rupiah flow - simple cashflow manager for tracking your own personal record

Usage:
  ruflow [options]

Options:
  -h                             show help message
  -v                             show versions
  -f <file>                      creating or accessing file
    -l                             show last activity
    -n <saving> <title>            write new record
    -i <amount> <description>      write incoming cash
    -o <amount> <description>      write outgoing cash
    
Examples:
  ruflow -f cashflow.log -n 200000 "Ucok Cashflow"
  ruflow -f cashflow.log -i 5000 "from udin"
  ruflow -f cashflow.log -o 2000 "from icih"
```

**\*changelog**

(prototype 3.1)
- Renamed variable ‘name’ to ‘title’ for consistency.
- Renamed function getValue() to getLastActivityValue() for consistency.
- Improved validation for savings/amount input. 
- Added auto-sum savings feature for -n on previous activity logs.
- Fixed other minor bugs.

(prototype 3.2)
- Added fallback logic for -n when no argument is provided but last activity record exists.
- Added automatic -n execution when the date changes.
- Added argument with spaces warning for title and description fields.
- Fixed other minor bugs.

**\*notes**
- now -n option can be run without arguments if it previously still kept the last activity record
- when you write \<title\> or \<description\> don't use whitespace otherwise write inside a quote
- \<title\> length is 31 and \<description\> length is 127


**\*known bugs**
- you tell me


**\*reference**
- https://www.instagram.com/reel/DLY_Wycx9C_/
- https://man7.org/linux/man-pages/man3/getopt.3.html
- https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
