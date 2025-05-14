/*
 * firmware_update.c
 *
 *  Created on: Mar 18, 2025
 *      Author: yam
 */

#include "mongoose.h"
#include "http/FOTA/firmware_update.h"


static const char *s_json_header =
    "Content-Type: application/json\r\n"
    "Cache-Control: no-cache\r\n";


// put documentaion in header, and organize the code 
void handle_firmware_upload(struct mg_connection *c, struct mg_http_message *hm) {
  char name[64], offset[20], total[20]; // next line inits the vars, faster then {0}.
  name[0] = offset[0] = '\0';
  long ofs = -1, tot = -1;

  // set http body in data variable, type mongoose string
  struct mg_str data = hm->body;

  // parse http query variables
  mg_http_get_var(&hm->query, "name", name, sizeof(name));
  mg_http_get_var(&hm->query, "offset", offset, sizeof(offset));
  mg_http_get_var(&hm->query, "total", total, sizeof(total));

  // log to putty...
  MG_INFO(("File %s, offset %s, len %lu", name, offset, data.len));

  // make sure the client side has initialized the fota request through the rest api
  // example:
  // -  offset = 0
  // -  total (size of the firmware, the .bin file) = 158K
  if ((ofs = mg_json_get_long(mg_str(offset), "$", -1)) < 0 ||
      (tot = mg_json_get_long(mg_str(total), "$", -1)) < 0) {
    mg_http_reply(c, 500, "", "offset and total not set\n");
  }

  // mg_ota_begin does the following:
  // -  raises flag that ota has began, 
  // -  checks that the total firmware size isn't too big, 
  //    up to 3 sectors allowed, each sector is of size 128KB
   else if (ofs == 0 && mg_ota_begin((size_t) tot) == false) {
    mg_http_reply(c, 500, "", "mg_ota_begin(%ld) failed\n", tot);
  } 
  
  // mg_ota_write writes the firmware to the second half of the flash at 0x08080000
  // 1KB chunks of the .bin file are recieved and mapped to the flash address at 0x08080000 + offset
  // if error, call mg_ota_end to stop the firmware update process...
  else if (data.len > 0 && mg_ota_write(data.buf, data.len) == false) {
    mg_http_reply(c, 500, "", "mg_ota_write(%lu) @%ld failed\n", data.len, ofs);
    mg_ota_end();
  } 
  
  // the last chunk recieved should be an empty chunck to signal that 
  // the whole .bin file has already been sent and the firmware should be updcated
  // mg_ota_end does the following:
  // -  rearranges the flash sectors:
  //      sectors 0-3 are for the current firmware
  //      sectors 4-6 are for the new firmware
  //      sector 7 is used as a temp sector to rearrange the flash sectors
  // -  reboots the MCU when done
  else if (data.len == 0 && mg_ota_end() == false) {
    mg_http_reply(c, 500, "", "mg_ota_end() failed\n", tot);
  } 
  
  else {
    mg_http_reply(c, 200, s_json_header, "true\n");
  }
}
