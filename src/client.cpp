#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <signal.h>
#include <zmq.hpp>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

const int TIMER = 50000;
const int SMOL_TIMER = 500;
const int DEFAULT_PORT  = 5050;
int n = 2;
int port_iter;
std::string command;

zmq::context_t context(2);
zmq::socket_t player_socket(context, ZMQ_REP);


bool send_message(zmq::socket_t &socket, const std::string &message_string) {
    zmq::message_t message(message_string.size());
    memcpy(message.data(), message_string.c_str(), message_string.size());
    return socket.send(message);
}

std::string receive_message(zmq::socket_t &socket) {
    zmq::message_t message;
    bool ok = false;
    try {
        ok = socket.recv(&message);
    }
    catch (...) {
        ok = false;
    }
    std::string recieved_message(static_cast<char*>(message.data()), message.size());
    if (recieved_message.empty() || !ok) {
        return "";
    }
    return recieved_message;
}

void create_node(int id, int port) {
    char* arg0 = strdup("./client");
    char* arg1 = strdup((std::to_string(id)).c_str());
    char* arg2 = strdup((std::to_string(port)).c_str());
    char* args[] = {arg0, arg1, arg2, NULL};
    execv("./client", args);
}

std::string get_port_name(const int port) {
    return "tcp://127.0.0.1:" + std::to_string(port);
}

void* check_invite(void *param) {
    std::string invite_tmp;
    // std::string answer;
    player_socket.connect(get_port_name(5555 + port_iter));
    // std::cout << "Поток клиента для получения инвайтов начал работу и ждет запрос" << std::endl;
    std::string invite_msg = receive_message(player_socket);
    std::stringstream invite_ss(invite_msg);
    std::getline(invite_ss, invite_tmp, ':');
    if (invite_tmp == "invite") {
        std::getline(invite_ss, invite_tmp, ':');
        std::cout << "Игрок с ником " + invite_tmp + " приглашает вас в игру!" << std::endl;
        std::cout << "Вы согласны? Напшите два раза y/n" << std::endl;
        std::cin >> command;
        std::cout << "Ваш ответ: " + command + "\n";
        if (command[0] == 'y') {
            std::cout << "Вы приняли запрос!" << std::endl;
            send_message(player_socket, "accept");
            pthread_exit(0);
        }
        else {
            std::cout << "Вы отклонили запрос!" << std::endl;
            send_message(player_socket, "reject");
        }
    }
	pthread_exit(0);
}


typedef struct {
} check_params;

int main(int argc, char** argv) {
    zmq::context_t context(2);
    zmq::socket_t main_socket(context, ZMQ_REQ);
    int pid = getpid();
    main_socket.connect(get_port_name(5555));

    // поток для отслеживания пригласил ли меня кто-то в игру
    check_params check_param;
    pthread_t tid;
    
    // pthread_join(tid, NULL);
    // std::cout << "Поток клиента для отправки команд начал работу" << std::endl;
    std::cout << "Вы можете пригласить своего друга по логину в игру!\n";
    std::cout << "Просто напишите invite (friend_login), но сначала авторизуйтесь\n";
    std::string received_message;
    std::string tmp;
    int iteration = 1;

    while(true) {
        // login
        if (iteration == 1) {
            iteration += 1;

            std::string login;
            std::cout << "Введите ваш логин: ";
            std::cin >> login;

            // формируем запрос
            std::string login_msg = "login:" + std::to_string(pid) + ":" + login;
            send_message(main_socket, login_msg);
            received_message = receive_message(main_socket);
            std::stringstream ss(received_message);
            
            // обрабатываем ответ
            std::getline(ss, tmp, ':');
            if (tmp == "Ok") {
                std::getline(ss, tmp, ':');
                port_iter = std::stoi(tmp);
                // std::cout << "Моя порт итерация: " + std::to_string(port_iter) << std::endl;
                player_socket.connect(get_port_name(5555 + port_iter));
                std::cout << "Вы успешно авторизовались!" << std::endl;
                pthread_create(&tid, NULL, check_invite, &check_param);
            }
            else {
                std::cout << "Во время авторизации что-то пошло не так..." << std::endl;
                break;
            }
        }
        else {
            std::cin >> command;
            if (command == "invite") {
                std::string invite_login;
                std::cin >> invite_login;
                std::cout << "Вы пригласили игрока с ником " + invite_login << std::endl;
                std::cout << "Ждем ответ..." << std::endl;
                std::string invite_cmd = "invite:" + std::to_string(port_iter) + ":" + invite_login; 
                send_message(main_socket, invite_cmd);
                received_message = receive_message(main_socket);
                std::stringstream ss(received_message);
                std::getline(ss, tmp, ':');
                if (tmp == "accept") {
                    std::cout << "Запрос принят, игра началась!" << std::endl;
                    break;
                }
                else if (tmp == "reject") {
                    std::cout << "Запрос отклонен! С тобой не хотят играть(" << std::endl;
                    // надо не делать ничего и вернуться к выбору команды хз
                }
            }
            // не уверен что экзит вообще нужен
            else if (command == "exit") {
                std::string exit_cmd = "exit:" + std::to_string(port_iter); 
                send_message(main_socket, exit_cmd);
                received_message = receive_message(main_socket);
                

                int n = system("killall client");
                break;
            }
            else if (command == "y") {
                // std::cout << "Подтвердите свою команду (напишите ее еще раз)" << std::endl;
                std::this_thread::sleep_for(2000ms);
                break;
            }
            else if (command == "n") {
                // std::cout << "Подтвердите свою команду (напишите ее еще раз)" << std::endl;
                std::this_thread::sleep_for(2000ms);
                // break;
            }
        }
    }

    std::cout << "ИГРА НАЧАЛАСЬ!!" << std::endl;
}
 