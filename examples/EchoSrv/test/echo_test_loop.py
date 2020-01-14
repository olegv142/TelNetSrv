"""
Telnet echo server performance test
"""

import sys
import socket

port = 23
addr = sys.argv[1]
msg = 'hello\n'
n = 1000

while True:
	s = socket.socket()
	s.connect((addr, port))

	for _ in range(n):
		sz = s.send(msg)
		cnt = 0
		buff = ''
		while cnt < sz:
			resp = s.recv(1024)
			buff += resp
			cnt += len(resp)
		assert buff == msg

	print '*',
