#ifndef _json_h_
#define _json_h_

#include <WString.h>

String jsonOfNetworks();

String jsonOfIP();

String planeOfHTTP(bool group);

void json_upload(String table_name,String ip_address, String json, String username, String password);

void json_delete(String table_name, int position);

String read_json_string(String element, String file_name);

String json_read(String table_name, int index);


size_t json_count(String table_name);

#endif