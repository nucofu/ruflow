this app is under development

**\*help**
ruflow/rupiah flow - simple cashflow manager for managing your own personal cashflow record

Usage:
  ruflow [options]

Options:
  -h                             show help message
  -v                             show versions
  -f \<file\>                      creating or accessing file
    -l                             show last activity
    -n \<saving\> \<name\>             write new record
    -i \<amount\> \<description\>      write incoming cash
    -o \<amount\> \<description\>      write outgoing cash

Examples:
  ruflow -f cashflow.log -n 200000 Ucok
  ruflow -f cashflow.log -i 5000 fromUdin
  ruflow -f cashflow.log -o 2000 fromIcih


**\*changelog**
- adding help message
- new option -v
- getValue() function is fixed
- other minor changes


**\*tips**
- write new record once a day, to change the date


**\*notes**
- when you write new record, write again your name and your last savings manually
- when you write \<name\> or \<description\> don't use whitespace
- \<name\> length is 31 and \<description\> length is 127


**\*known bugs**
- you tell me


**\*reference**
- https://www.instagram.com/reel/DLY_Wycx9C_/
- https://man7.org/linux/man-pages/man3/getopt.3.html
- https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
