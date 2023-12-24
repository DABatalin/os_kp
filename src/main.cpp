#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <cassert>
#include <zmq.hpp>
#include <chrono>
#include <thread>
#include <exception>
#include <map>


using namespace std::chrono_literals;

const int TIMER = 50000;
const int SMOL_TIMER = 500;
int n = 2;


// общая функция для отправки сообщения в дочерний процесс
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
        return "Root is dead";
    }
    return recieved_message;
}

// меняем созданый fork процесс на дочерний, передавая туда нужные нам аргументы (клиент)
void create_node(int id, int port) {
    char* arg0 = strdup("./client");
    char* arg1 = strdup((std::to_string(id)).c_str());
    char* arg2 = strdup((std::to_string(port)).c_str());
    char* args[] = {arg0, arg1, arg2, NULL};
    execv("./client", args);
}

// функция, собирающая полный адрес до дочернего процесса
std::string get_port_name(const int port) {
    return "tcp://127.0.0.1:" + std::to_string(port);
}

bool is_number(std::string val) {
    try {
        int tmp = std::stoi(val);
        return true;
    }
    catch(std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return false;
    }
}




int main() {
    zmq::context_t context(3);
    zmq::socket_t main_socket(context, ZMQ_REP);
    zmq::socket_t  first_player_socket(context, ZMQ_REQ);
    zmq::socket_t second_player_socket(context, ZMQ_REQ);
    main_socket.bind("tcp://*:5555");
    first_player_socket.bind("tcp://*:5556");
    second_player_socket.bind("tcp://*:5557");
    std::cout << "Сервер начал работу" << std::endl;
    std::map<int, std::string> login_map;
    int port_iter = 1;
    while (true) {
        std::string received_message = receive_message(main_socket);
        std::cout << "На сервер поступил запрос: '" + received_message + "'" << std::endl;
        std::stringstream ss(received_message);
        std::string tmp;
        std::getline(ss, tmp, ':');
        if (tmp == "login") {
            if (port_iter > 2) {
                send_message(main_socket, "Error:TwoPlayersAlreadyExist");
            }
            else {
                std::getline(ss, tmp, ':');
                std::getline(ss, tmp, ':');
                std::cout << "Логин игрока номер " + std::to_string(port_iter) + ": " + tmp << std::endl;
                std::string login = tmp;
                login_map[port_iter] = login;
                
                send_message(main_socket, "Ok:" + std::to_string(port_iter));
                port_iter += 1;
            }
        }
        // получили от клиента запрос на игру другому клиенту
        else if (tmp == "invite") {
            std::string invite_login;
            std::getline(ss, tmp, ':');
            int sender_id = std::stoi(tmp);
            std::getline(ss, invite_login, ':');
            
            // если клиент 1 отправил запрос клиенту 2
            if (invite_login == login_map[2]) {
                send_message(second_player_socket, "invite:" + login_map[1]);
                std::string invite_message = receive_message(second_player_socket);
                if (invite_message == "accept") {
                    send_message(main_socket, invite_message);
                    std::this_thread::sleep_for(2000ms);
                    break;
                }
                else if (invite_message == "reject") {
                    send_message(main_socket, invite_message);
                }
                else {
                    std::cout << "Что-то пошло не так во время обработки запроса на игру" << std::endl;
                }

            }
            // если клиент 2 отправил запрос клиенту 1
            else if (invite_login == login_map[1]){
                send_message(first_player_socket, "invite:" + login_map[1]);
                std::string invite_message = receive_message(first_player_socket);
                if (invite_message == "accept") {
                    send_message(main_socket, invite_message);
                    break;
                }
                else if (invite_message == "reject") {
                    send_message(main_socket, invite_message);
                }
                else {
                    std::cout << "Что-то пошло не так во время обработки запроса на игру" << std::endl;
                }
            }
            else {
                if (sender_id == 1) {
                    send_message(first_player_socket, "error:login_doesnt_exist");
                }
                else {
                    send_message(second_player_socket, "error:login_doesnt_exist");
                }
            }
            std::getline(ss, tmp, ':');

        }
        else if (received_message.substr(0, std::min<int>(received_message.size(), 4)) == "exit") {
            // все игры закончить всех послать нахуй
        }
    }
    std::cout << "Начинаю игру!" << std::endl;
}
