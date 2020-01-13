//
// Simple telnet echo server
//

#include "TelNetSrv.h"
#include "avr/boot.h"

#define DHCP_TOUT 10000
#define TELNET_PORT 23
#define MAX_CMD_LEN 32

static TelNetSrv g_telnet_srv(TELNET_PORT, MAX_CMD_LEN);

void telnet_setup()
{
  uint8_t mac[6] = {
    boot_signature_byte_get(0x0E),
    boot_signature_byte_get(0x0F),
    boot_signature_byte_get(0x10),
    boot_signature_byte_get(0x11),
    boot_signature_byte_get(0x12),
    boot_signature_byte_get(0x13)
  };
  mac[0] &= ~1;
  mac[0] |= 2;
  if (Ethernet.begin(mac, DHCP_TOUT)) {
    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
    g_telnet_srv.start();
  } else {
    Serial.println("Can't get server IP address");
  }
}

void setup()
{
  Serial.begin(9600);
  telnet_setup();
}

void loop() {
  g_telnet_srv.serve();

  String cmd;
  if (!g_telnet_srv.get_line(cmd))
    return;

  g_telnet_srv.print(cmd);
}