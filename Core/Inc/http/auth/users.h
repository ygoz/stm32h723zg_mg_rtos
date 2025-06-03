
#include "mongoose.h"

struct user {
    struct user *next;
    char name[32];   // User name
    char token[21];  // Login token
    int level;       // Access level
  };

  
extern struct user *s_users;


struct user *authenticate(struct mg_http_message *hm);
void handle_login(struct mg_connection *c, struct user *u);
void handle_logout(struct mg_connection *c);