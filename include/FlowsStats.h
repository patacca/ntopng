/*
 *
 * (C) 2019-23 - ntop.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _FLOWS_STATS_H_
#define _FLOWS_STATS_H_

#include "ntop_includes.h"

/* *************************************** */

class FlowsStats {
 private:
  std::set<std::string> clients, servers;
  u_int32_t num_flows, tot_score;
  u_int64_t tot_sent, tot_rcvd;
  u_int8_t l4_proto;
  FlowsHostInfo* client;
  FlowsHostInfo* server;

  u_int16_t vlan_id;
  char* proto_name;
  char* info_key;
  u_int64_t key;
  u_int64_t proto_key;

 public:
  FlowsStats(const IpAddress* c, const IpAddress* s, u_int8_t _l4_proto,
             u_int64_t bytes_sent, u_int64_t bytes_rcvd, u_int32_t score) {
    num_flows = 0, tot_sent = tot_rcvd = 0, tot_score = 0, l4_proto = _l4_proto,
    proto_name = NULL;
    incFlowStats(c, s, bytes_sent, bytes_rcvd, score);
  }

  ~FlowsStats() {
    if (proto_name) free(proto_name);
  }

  inline u_int32_t getNumClients() { return (clients.size()); }
  inline u_int32_t getNumServers() { return (servers.size()); }
  inline u_int8_t getL4Protocol() { return (l4_proto); }
  inline u_int32_t getNumFlows() { return (num_flows); }
  inline u_int64_t getTotalSent() { return (tot_sent); }
  inline u_int64_t getTotalRcvd() { return (tot_rcvd); }
  inline u_int32_t getTotalScore() { return (tot_score); }

  inline void setVlanId(u_int16_t _vlan_id) { vlan_id = _vlan_id; }
  inline u_int16_t getVlanId() { return vlan_id; }
  inline void setProtoName(char* _proto_name) {
    if (proto_name) free(proto_name);
    proto_name = strdup(_proto_name);
  }
  inline char* getProtoName() { return (proto_name ? proto_name : (char*)""); }
  inline void setKey(u_int64_t _key) { key = _key; }
  inline u_int64_t getKey() { return key; }
  inline void setProtoKey(u_int64_t _key) { proto_key = _key; }
  inline u_int64_t getProtoKey() { return proto_key; }
  inline void setInfoKey(char* _key) { info_key = _key; }
  inline char* getInfoKey() { return (info_key ? info_key : (char*)""); }

  inline void setClient(IpAddress* _ip, Host* _host) {
    client = new (std::nothrow) FlowsHostInfo(_ip, _host);
  }
  inline void setServer(IpAddress* _ip, Host* _host) {
    server = new (std::nothrow) FlowsHostInfo(_ip, _host);
  }

  inline void incFlowStats(const IpAddress* c, const IpAddress* s,
                           u_int64_t bytes_sent, u_int64_t bytes_rcvd,
                           u_int32_t score) {
    char buf_c[48], buf_s[48];

    clients.insert(
        std::string(((IpAddress*)c)->get_ip_hex(buf_c, sizeof(buf_c)))),
        servers.insert(
            std::string(((IpAddress*)s)->get_ip_hex(buf_s, sizeof(buf_s)))),
        num_flows++, tot_sent += bytes_sent, tot_rcvd += bytes_rcvd,
        tot_score += score;
    ;
  }

  char* getCliIP(char* buf, u_int len) { return (client->getIP(buf, len)); }
  char* getSrvIP(char* buf, u_int len) { return (server->getIP(buf, len)); }
  char* getCliName(char* buf, u_int len) {
    return (client->getHostName(buf, len));
  }
  char* getSrvName(char* buf, u_int len) {
    return (server->getHostName(buf, len));
  }
  char* getCliIPHex(char* buf, u_int len) {
    return (client->getIPHex(buf, len));
  }
  char* getSrvIPHex(char* buf, u_int len) {
    return (server->getIPHex(buf, len));
  }
  u_int16_t getCliVLANId() { return (client->getVLANId()); }
  u_int16_t getSrvVLANId() { return (server->getVLANId()); }
  bool isCliInMem() { return (client->isHostInMem()); }
  bool isSrvInMem() { return (server->isHostInMem()); }
};

#endif /* _FLOWS_STATS_H_ */
