/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sevr.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouarsas <mouarsas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:18:04 by mouarsas          #+#    #+#             */
/*   Updated: 2023/03/14 20:54:10 by mouarsas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>

const int BUFFER_SIZE = 1024;

void* handle_client(void* client_fd_ptr) {
    int client_fd = *(int*) client_fd_ptr;
    char buffer[BUFFER_SIZE];
    int n;

    // Handle incoming messages
    while (true) {
        std::memset(buffer, 0, BUFFER_SIZE);
        n = read(client_fd, buffer, BUFFER_SIZE);
        if (n < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            break;
        }
        else if (n == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        else {
            std::cout << "Received message: " << buffer << std::endl;

            // Send response here

            std::memset(buffer, 0, BUFFER_SIZE);
        }
    }

    // Close socket
    close(client_fd);

    return NULL;
}

int main(int ac, char *av[]) {
    int server_fd, client_fd, port_number;
    std::string passWord;
    socklen_t client_address_length;
    struct sockaddr_in server_address, client_address;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Bind socket to port
    std::memset(&server_address, 0, sizeof(server_address));
    port_number = atoi(av[1]);
    passWord = av[2];
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    if (bind(server_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        std::cerr << "Eror binding socket to prort" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    listen(server_fd, 5);

    std::vector<pthread_t> client_threads;

    // Accept incoming connections
    while (true) {
        client_address_length = sizeof(client_address);
        client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_address_length);
        if (client_fd < 0) {
            std::cerr << "Error accepting incoming connection" << std::endl;
            return 1;
        }

        std::cout << "Client connected" << std::endl;

        // Create new thread to handle client connection
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, &client_fd) != 0) {
            std::cerr << "Error creating thread" << std::endl;
            return 1;
        }
        client_threads.push_back(client_thread);
    }

    // Wait for client threads to finish
    for (std::vector<pthread_t>::iterator it = client_threads.begin(); it != client_threads.end(); it++) {
        pthread_join(*it, NULL);
    }

    // Close socket
    close(server_fd);

    return 0;
}

// client //
// enter this argument in other prompet "nc localhost 555"