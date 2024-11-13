#include <csp/csp.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int read_file_into_array(const char *filename, char **buffer) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Allocate a buffer to hold the entire file
    *buffer = malloc(file_size + 1); // +1 for null-terminator
    if (!*buffer) {
        fprintf(stderr, "Error: Out of memory\n");
        fclose(file);
        return -1;
    }

    // Read the entire file into the buffer
    size_t bytes_read = fread(*buffer, 1, file_size, file);
    fclose(file);

    // Null-terminate the buffer
    (*buffer)[file_size] = '\0';

    return (int)bytes_read;
}

void client_start(void)  {    
    const char *filename = "/home/johannes/Pictures/Espresso_ViCAFE_BrewGuide.png"; // size over 1kb
    
    char *img;
    int bytes_read = read_file_into_array(filename, &img);

    if (bytes_read < 0) {
        fprintf(stderr, "Error reading file: %s.\n", filename);
        return;
    }

    while(1) {
        csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, 1, 10, 1000, CSP_O_NONE);
		if (conn == NULL) {
			/* Connect failed */
			csp_print("Connection failed\n");
			continue;
		}
        
        csp_print("connection established. Sending Image\n");
        
        int ret = csp_sfp_send(conn, img, bytes_read, 241, 5000);
        if (ret != CSP_ERR_NONE) {
            csp_print("Error sending image: %d\n", ret);
            csp_close(conn);
            sleep(2);
            continue;
        }
        csp_print("Image '%s' was sent. closing connection\n", filename);
        csp_close(conn);
        sleep(5);
    }
    return;
}