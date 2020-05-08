import os
import hashlib

path = 'advanced_mylog/'
files = os.listdir(path)
files.sort()
for f in files:
    if f.endswith('.log'):
        f_result = open('PrevTests/Advanced/%s' % f, 'rb')
        f_output = open('advanced_mylog/%s' % f, 'rb')
        results = f_result.read()
        outputs = f_output.read()
        if hashlib.md5(results).hexdigest() == hashlib.md5(outputs).hexdigest():
            print("\033[32m %s passed \033[0m" % (f))
        else:
            print("\033[31m %s failed \033[0m" % (f))

        # f_result = open('PrevTests/Advanced/%s' % f, 'r')
        # f_output = open('advanced_mylog/%s' % f, 'r')
        # results = f_result.readlines()
        # outputs = f_output.readlines()
        # flag = True
        # for i in range(len(results)):
        #     if results[i] != outputs[i]:
        #         flag = False
        #         print("Error line: %d" % (i + 1))
        #         print(("Correct line: %s" % results[i]).strip())
        #         print(("My line: %s" % outputs[i]).strip())
        # if flag:
        #     print("\033[32m%s passed \033[0m" % (f))
        # else:
        #     print("\033[31m%s failed \033[0m" % (f))