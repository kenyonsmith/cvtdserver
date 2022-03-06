#include "mongoose/mongoose.h"

static const char* pCvtdUri = "http://cvtd.info:8080/CVTDfeed/V200/XML/_System.php?key=%s";
char pCvtdKey[32];
char pCvtdUriWithKey[256];

static void fn(struct mg_connection* c, int ev, void* ev_data, void* fn_data)
{
    if (ev == MG_EV_CONNECT)
    {
        // Connected to server
        printf("Connected to server\n");
        struct mg_str host = mg_url_host(pCvtdUriWithKey);

        // Send request
        mg_printf(c, "GET %s HTTP/1.0\r\n"
                     "Host: %.*s\r\n"
                     "\r\n",
                     mg_url_uri(pCvtdUriWithKey), (int) host.len, host.ptr);
    } else if (ev == MG_EV_HTTP_MSG)
    {
        // Response is received. Print it
        struct mg_http_message* hm = (struct mg_http_message*) ev_data;
        printf("%.*s", (int) hm->message.len, hm->message.ptr);
        c->is_closing = 1;          // Tell mongoose to close this connection
        *(bool*) fn_data = true;    // Tell event loop to stop
    } else if (ev == MG_EV_ERROR)
    {
        printf("There was an error\n");
        *(bool*) fn_data = true;
    }
}

int main(int argc, char* argv[])
{
    struct mg_mgr mgr;
    bool done = false;

    // Open up key.txt and fill out pCvtdUriWithKey
    FILE* pKeyFile = fopen("key.txt","r");
    if (pKeyFile == NULL)
    {
        printf("Error: Could not find key.txt\n");
        return -1;
    }
    fread(pCvtdKey, sizeof(char), 32, pKeyFile);
    sprintf(pCvtdUriWithKey, pCvtdUri, pCvtdKey);
    fclose(pKeyFile);

    mg_mgr_init(&mgr);
    mg_http_connect(&mgr, pCvtdUriWithKey, fn, &done);

    while (!done)
    {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);

    return 0;
}
