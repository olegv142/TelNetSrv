"""
Telnet echo server performance test
"""

import sys
import socket
import time

port = 23
addr = sys.argv[1]
msg = 'hello\n'
n = 1000

s = socket.socket()
s.connect((addr, port))

start = time.time()
for _ in range(n):
	sz = s.send(msg)
	cnt = 0
	buff = ''
	while cnt < sz:
		resp = s.recv(1024)
		buff += resp
		cnt += len(resp)
	assert buff == msg

elapsed = time.time() - start
print n // elapsed, 'msgs / sec'
