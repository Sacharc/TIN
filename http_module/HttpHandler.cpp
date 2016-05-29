//
// Created by Michal Tomaszewski
//

#include "HttpHandler.h"

void httpHandlerStart(MessageHandler* messageHandler, CommandLineInterface *cli) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;
    char buf[2048];
    int on = 1;

    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_server < 0){
        perror("HTTP socket error");
        exit(1);
    }

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8040);

    if(bind(fd_server, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
        perror("HTTP bind error");
        close(fd_server);
        exit(1);
    }

    if(listen(fd_server, 10) == -1){
        perror("HTTP listen error");
        close(fd_server);
        exit(1);
    }

    while(!cli->isEnd()){
        int res;
        fd_set fds;
        struct timeval tv = {5, 0};

        FD_ZERO (&fds);
        FD_SET (fd_server, &fds);

        res = select (fd_server + 1, &fds, NULL, NULL, &tv);
        if (res > 0) {
            fd_client = accept(fd_server, (struct sockaddr*) &client_addr, &sin_len);

            if(fd_client == -1){
                perror("HTTP Connection failed.....\n");
                continue;
            }

//            printf("Got HTTP client connect...\n");

            memset(buf,0,2048);
            read(fd_client, buf, 2047);

            const char* webpage = buildHtmlString(messageHandler->getHistory().getAllRecords());

            if(webpage != NULL){
                write(fd_client, webpage, strlen(webpage) - 1);
            }
            close(fd_client);
        }
    }
    close(fd_server);
}



const char* buildHtmlString(std::vector<HistoryRecord*>* history){
    Statistics statistics;
    std::string webpageStr = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\n"
            "<!DOCTYPE HTML>\r\n"
            "<html><head><title>TIN_HTTP_MODULE</title></head>\r\n"
            "<body><center><h1>TIN - DETECTORS AND MANAGER</h1></center>\r\n"
            "<h4>Średnia rezystancja:               " + std::to_string(statistics.countAverageCurrentResistance(history)) + "</h4>" + "\r\n"
            "<h4>Srednie odstepstwo:                " + std::to_string(statistics.countAverageDifferenceResistance(history)) + "</h4>" + "\r\n"
            "<h4>Srednia typowa rezystancja:        " + std::to_string(statistics.countAverageTypicalResistance(history)) + "</h4>" + "\r\n"
            "<h4>Liczba alarmow:                    " + std::to_string(statistics.countDetectorWithAlarm(history)) + "</h4>" + "\r\n"
            "<h4>Liczba zerwanych kabli oporowych:  " + std::to_string(statistics.countDetectorWithInterruptNet(history)) + "</h4>" + "\r\n"
            "<br>\r\n"
            "<br>\r\n"
            "</table>\r\n"
            "<table>\r\n"
            "<tr>\r\n"
            "<td>CZAS</td>\r\n"
            "<td>ID</td>\r\n"
            "<td>TYP WIADOMOŚCI</td>\r\n"
            "<td>OBECNY POMIAR</td>\r\n"
            "<td>TYPOWA REZYSTANCJA</td>\r\n"
            "</tr>\r\n"
            "<tr>\r\n"
            "<td>---------------------------------------</td>\r\n"
            "<td>---------------------------------------</td>\r\n"
            "<td>---------------------------------------</td>\r\n"
            "<td>---------------------------------------</td>\r\n"
            "<td>---------------------------------------</td>\r\n"
            "</tr>\r\n";

    for (unsigned i = 1; i < history->size() && i < 100; ++i){
        char buff[80];
        std::string tr = "<tr>";
        std::string tre = "</tr>";
        std::string tde = "</td>";
        std::string td = "<td>";
        std::string endln = "\r\n";


        time_t time1 = history->at(history->size()-i)->getTime();
        strftime(buff, 80, "%Y-%m-%d %H:%M:%S", localtime(&time1));
        std::string time(buff);
        std::string id = std::to_string(history->at(history->size()-i)->getMessage().id);
        std::string type = messageTypeToString(history->at(history->size()-i)->getMessageType());
        std::string currentResistence = std::to_string(history->at(history->size()-i)->getCurrentResistance());
        std::string typicalResistance = std::to_string(history->at(history->size()-i)->getTypicalResistance());

        webpageStr +=
                tr + endln +
                     td + time + tde + endln +
                     td + id + tde + endln +
                     td + type + tde + endln +
                     td + currentResistence + tde + endln +
                     td + typicalResistance + tde + endln +
                tre + endln;
    }

    webpageStr += "</table>\r\n"
            "</body></html>\r\n";

    return webpageStr.c_str();

}

