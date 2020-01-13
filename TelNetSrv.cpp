#include <Arduino.h>
#include "TelNetSrv.h"

#define RX_CHUNK 16

void TelNetSrv::serve()
{
	EthernetClient clnt = m_srv.accept();
	if (clnt) {
		reset();
		m_clnt = clnt;
	}
	int avail = m_clnt.available();
	bool cmd_byte = false, opt_byte = false;
	while (avail > 0) {
		uint8_t chunk_buff[RX_CHUNK];
		unsigned chunk_sz = avail > RX_CHUNK ? RX_CHUNK : avail;
		int rd = m_clnt.read(chunk_buff, chunk_sz);
		if (rd != chunk_sz) {
			reset();
			return;
		}
		for (int8_t i = 0; i < chunk_sz; ++i) {
			uint8_t b = chunk_buff[i];
			// skip telnet command and options bytes
			if (cmd_byte) {
				if (b >= 251)
					// next byte is the command option
					opt_byte = true;
				cmd_byte = false;
			} else if (opt_byte) {
				// just skip it
				opt_byte = false;
			} else {
				if (b == 255) // IAC byte
					// next byte is the command
					cmd_byte = true;
				else {
					// enforce max message size limitation
					if (m_buff.length() <= m_max_size)
						m_buff += (char)b;
					else
						m_buff[m_max_size] = (char)b;
				}
			}
		}
		avail -= chunk_sz;
	}
}

bool TelNetSrv::has_line() const
{
	if (!m_buff)
		return false;
	switch (m_buff[m_buff.length() - 1]) {
	case '\n':
	case '\r':
		return true;
	default:
		return false;
	}
}
