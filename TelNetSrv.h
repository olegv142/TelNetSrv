#pragma once

/*
 * Simple telnet server processing one line at a time.
 */

#include <Ethernet.h>
#include <Print.h>

class TelNetSrv : public Print {
public:
	TelNetSrv(uint16_t port, uint16_t max_rx_size, uint8_t max_tx_chunk = 16)
		: m_srv(port), m_max_rx(max_rx_size), m_max_tx_chunk(max_tx_chunk) {}

	// Start serving requests. May be called multiple times.
	void           start() { m_srv.begin(); }
	// Serve requests if any
	void           serve();
	// Clear current connection and drop buffer content
	void           reset() { m_clnt.stop(); clr_buff(); }

	// Buffer access methods
	String const&  get_buff() const { return m_rx_buff; };
	void           clr_buff() { m_rx_buff = String(); }
	bool           has_line() const;

	// Respond to current client with given string
	void           respond(String const& s) { m_clnt.write(s.c_str(), s.length()); }

	// The Print class method implementation
	// Append symbol to transmit buffer. Call flush() to force transmit. */
	virtual size_t write(uint8_t b) {
				m_tx_buff += (char)b;
				if (m_tx_buff.length() >= m_max_tx_chunk)
					flush();
			}

	// Flush transmit buffer
	void          flush() { respond(m_tx_buff); m_tx_buff = String(); }

protected:
	EthernetServer m_srv;
	EthernetClient m_clnt;
	uint16_t       m_max_rx;
	uint8_t        m_max_tx_chunk;
	String         m_rx_buff;
	String         m_tx_buff;
};
