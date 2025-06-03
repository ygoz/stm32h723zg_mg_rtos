#include "users.h"


struct user *s_users;  // List of authenticated users


int glue_authenticate(const char *user, const char *pass) {
    int level = 0; // Authentication failure
    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin") == 0) {
      level = 7;  // Administrator
    } else if (strcmp(user, "user") == 0 && strcmp(pass, "user") == 0) {
      level = 3;  // Ordinary dude
    }
    return level;
  }
  
  
// Parse HTTP requests, return authenticated user or NULL
struct user *authenticate(struct mg_http_message *hm) {
    char user[100], pass[100];
    struct user *current_user, *authenticated_user = NULL;
    mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  
    if (user[0] != '\0' && pass[0] != '\0') {
      // Both user and password is set, auth by user/password via glue API
      int level = glue_authenticate(user, pass);
      MG_DEBUG(("user %s, level: %d", user, level));

      if (level > 0) {  // Proceed only if the firmware authenticated us
        // uint64_t uid = hash(3, mg_str(user), mg_str(":"), mg_str(pass));
        for (current_user = s_users; current_user != NULL && authenticated_user == NULL; current_user = current_user->next) {
          if (strcmp(user, current_user->name) == 0) authenticated_user = current_user;
        }
        // Not yet authenticated, add to the list
        if (authenticated_user == NULL) {
          authenticated_user = (struct user *) calloc(1, sizeof(*authenticated_user));
          mg_snprintf(authenticated_user->name, sizeof(authenticated_user->name), "%s", user);
          mg_random_str(authenticated_user->token, sizeof(authenticated_user->token) - 1);
          authenticated_user->level = level, authenticated_user->next = s_users, s_users = authenticated_user;
        }
      }
    } 
    else if (user[0] == '\0' && pass[0] != '\0') {
      for (current_user = s_users; current_user != NULL && authenticated_user == NULL; current_user = current_user->next) {
        if (strcmp(current_user->token, pass) == 0) authenticated_user = current_user;
      }
    }
    MG_VERBOSE(("[%s/%s] -> %s", user, pass, authenticated_user ? "OK" : "FAIL"));
    return authenticated_user;
  }
  
void handle_login(struct mg_connection *c, struct user *u) {
    char cookie[256];
    mg_snprintf(cookie, sizeof(cookie),
                "Set-Cookie: access_token=%s; Path=/; "
                "%sHttpOnly; SameSite=Lax; Max-Age=%d\r\n",
                u->token, c->is_tls ? "Secure; " : "", 3600 * 24);
    mg_http_reply(c, 200, cookie, "{%m:%m,%m:%d}",  //
                  MG_ESC("user"), MG_ESC(u->name),  //
                  MG_ESC("level"), u->level);
  }
  
void handle_logout(struct mg_connection *c) {
    char cookie[256];
    mg_snprintf(cookie, sizeof(cookie),
                "Set-Cookie: access_token=; Path=/; "
                "Expires=Thu, 01 Jan 1970 00:00:00 UTC; "
                "%sHttpOnly; Max-Age=0; \r\n",
                c->is_tls ? "Secure; " : "");
    mg_http_reply(c, 401, cookie, "Unauthorized\n");
  }