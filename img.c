#include <csp/csp.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int count = 0;

int show_image_with_xdg_open(uint8_t *image_data, size_t size) {
    // Write image data to a temporary file
    char filename[64];
    sprintf(filename,"/tmp/temp_image_%d.png", count++);
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open temporary file.\n");
        return -1;
    }

    fwrite(image_data, 1, size, file);
    fclose(file);

    // Use xdg-open to open the image with the default viewer
    char command[128];
    sprintf(command, "xdg-open %s", filename);
    return system(command);
}


/* Client task sending requests to server task */
void client_start(void) {    
    csp_print("IMG RCV Client task started\n");

    csp_socket_t sock = {0};

    csp_bind(&sock, CSP_ANY);

    /* Create a backlog of 10 connections, i.e. up to 10 new connections can be queued */
    csp_listen(&sock, 10);

    while (1) {
        csp_conn_t *conn;
        if ((conn = csp_accept(&sock, 5000)) == NULL) {
            /* timeout */
            csp_print("csp_accept: timeout\n");
            continue;
        }
        csp_print("got connection\n");
        uint8_t* output;
        int size = 0;
        int ret;
        ret = csp_sfp_recv(conn, (void **) &output, &size, 10000);
        if (ret != CSP_ERR_NONE) {
            csp_print("Error in csp_sfp_recv: %d\n", ret);
            csp_close(conn);
            continue;
        }
        show_image_with_xdg_open(output, size);
        free(output);
      
        csp_print("done with receiving. closing connection\n");
        /* Close current connection */
        csp_close(conn);
    }
    return;
}
/* End of client task */
