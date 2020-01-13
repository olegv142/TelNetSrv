#pragma once

/*
 * Simple telnet server processing one line at a time.
 */

#include <Ethernet.h>
#include <Print.h>

class TelNetSrv : public Print {
public:
	TelNetSrv(uint16_t port, uint16_t max_size)
		: m_srv(port), m_max_size(max_size) {}

	// Start serving requests. May be called multiple times.
	void           start() { m_srv.begin(); }
	// Serve requests if any
	void           serve();
	// Clear current connection and drop buffer content
	void           reset() { m_clnt.stop(); clr_buff(); }

	// Buffer access methods
	String const&  get_buff() const { return m_buff; };
	void           clr_buff() { m_buff = String(); }
	bool           has_line() const;
	bool           get_line(String& ret) {
				if (!has_line())
					return false;
				ret = get_buff();
				clr_buff();
				return true;
			}

	// Respond to current client with given string
	void           respond(String const& s) { m_clnt.write(s.c_str(), s.length()); }

	// The Print class method implementation
	virtual size_t write(uint8_t b) { m_clnt.write(b); }

protected:
	EthernetServer m_srv;
	EthernetClient m_clnt;
	uint16_t m_max_size;
	String m_buff;
};
