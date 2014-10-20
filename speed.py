#!/usr/bin/env python
import sys
from datetime import datetime
flag = 'SHIFT-REDUCE generator [test] v0.1'
end_token = '.......... EXIT ..........'
num = 1921

if __name__=="__main__":
    try:
        fp=open(sys.argv[1], "r")
    except:
        print >> sys.stderr, "failed to open file."
        sys.exit(1)

    i=1
    for section in fp.read().strip().split(end_token):
        if flag not in section:
            continue
        if 'dev' not in section:
            continue

        start_time = None
        end_time = None
        for line in section.split("\n"):
            if 'SRG: output result to' in line:
                start_time = datetime.strptime(line.split()[1], "%H:%M:%S")
        end_time = datetime.strptime(line.split()[1], "%H:%M:%S")
        print i, (end_time - start_time).total_seconds() / num
        i += 1
