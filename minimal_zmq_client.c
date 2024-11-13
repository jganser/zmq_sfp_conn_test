#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <csp/csp.h>
#include <csp/interfaces/csp_if_zmqhub.h>

static uint8_t client_address = 1;

int router_start(void);
extern void client_start(void);


static struct option long_options[] = {
    {"interface-address", required_argument, 0, 'a'},
    {"help", no_argument, 0, 'h'},    
    {0, 0, 0, 0}
};

void print_help() {
    csp_print("Usage: csp_client [options]\n"
           " -a <address>               set interface address\n"
           " -h                         print help\n");
}

/* main - initialization of CSP and start of server/client tasks */
int main(int argc, char * argv[]) {

    const char * zmq_device = "127.0.0.1";    
    int opt;
    while ((opt = getopt_long(argc, argv, "a:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                client_address = atoi(optarg);
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            case '?':
                // Invalid option or missing argument
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    csp_print("Initialising CSP\n");

    /* Init CSP */
    csp_init();

    /* Start router */
    router_start();

    /* Add interface(s) */
    csp_iface_t * default_iface = NULL;
    if (zmq_device) {
        int error = csp_zmqhub_init(client_address, zmq_device, 0, &default_iface);
        if (error != CSP_ERR_NONE) {
            csp_print("failed to add ZMQ interface [%s], error: %d\n", zmq_device, error);
            exit(1);
        }
        default_iface->is_default = 1;
    }

    csp_print("Connection table\r\n");
    csp_conn_print_table();

    csp_print("Interfaces\r\n");
    csp_iflist_print();

    /* Start client thread */
    client_start();

    return 0;
}
