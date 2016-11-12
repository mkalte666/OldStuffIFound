import sys
import httplib

failLevel = 0

conn = httplib.HTTPSConnection("bee-more-random.tk")
conn.request("GET", "/idesk/")
r1 = conn.getresponse()
print r1.status, r1.reason
data1 = r1.read()
words = data1.split(" ")
count = dict()
for w in words:
	try: 
		count[w] += 1
	except KeyError:
		count[w] = 1
for w in count:
	print w+": "+str(count[w])