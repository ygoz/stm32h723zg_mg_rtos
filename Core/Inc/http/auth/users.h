
#include "mongoose.h"

struct user {
    struct user *next;
    char name[32];   // User name
    char token[21];  // Login token
    int level;       // Access level
  };

  
extern struct user *s_users;



/**
 * @brief Authenticates a user based on HTTP request credentials or token.
 *
 * This function processes the HTTP request to extract authentication credentials
 * (username and password) using `mg_http_creds()`. It supports two authentication flows:
 *
 * - **Username/Password Authentication**:  
 *   If both username and password are present, it validates them via the firmware-specific
 *   `glue_authenticate()` function. If the credentials are valid (level > 0), the function:
 *   - Searches for an existing user with the same username in the global user list (`s_users`).
 *   - If the user exists, it returns the existing user pointer.
 *   - If not, it allocates a new user, generates a random access token, sets the access level,
 *     and prepends it to the global user list.
 *
 * - **Token-Based Authentication**:  
 *   If only a token is provided (username is empty), the function attempts to find a user in
 *   the global user list (`s_users`) whose token matches the provided value.
 *
 * @param[in] hm Pointer to the HTTP message (`struct mg_http_message`) containing the
 *               `Authorization` header with credentials or token.
 *
 * @return A pointer to a valid `struct user` if authentication succeeds; otherwise, `NULL`.
 *
 * @note
 * - The returned user pointer references a globally managed list (`s_users`).
 * - New users are dynamically allocated with `calloc()` and stored in `s_users`.
 * - Make sure the global user list is appropriately managed (e.g. cleaned up on logout or expiry).
 *
 * @see glue_authenticate(), mg_http_creds(), mg_random_str()
 */
struct user *authenticate(struct mg_http_message *hm);


/**
 * @brief Logs in a user by setting an access token cookie and returning user info.
 *
 * This function sets a `Set-Cookie` header containing the user's access token,
 * configured to be `HttpOnly`, `SameSite=Lax`, and with a 24-hour expiration (`Max-Age`).
 * If the connection is over TLS, the cookie is also marked as `Secure`.
 * 
 * The response body is a JSON object containing the authenticated user's name and level.
 * The HTTP status code is set to `200 OK`.
 *
 * @param[in] c Pointer to the Mongoose connection (`struct mg_connection`).
 * @param[in] u Pointer to the authenticated user (`struct user`) whose token
 *              and info will be returned and set in the cookie.
 *
 * @note This function is typically called after successful authentication.
 *
 * @return void
 */
void handle_login(struct mg_connection *c, struct user *u);


/**
 * @brief Logs out a user by clearing the access token cookie.
 *
 * This function invalidates the user's session by sending a `Set-Cookie`
 * header with an empty `access_token`, an expiration date in the past,
 * and `Max-Age=0`. The cookie is marked as `HttpOnly`, and if the 
 * connection is secure (TLS), it is also marked as `Secure`.
 * 
 * The function then responds to the client with an HTTP 401 Unauthorized
 * status code and a plain text response body.
 *
 * @param[in] c Pointer to the Mongoose connection (`struct mg_connection`).
 *
 * @note This is typically used when a user logs out or their session
 *       is invalidated.
 *
 * @return void
 */
void handle_logout(struct mg_connection *c);