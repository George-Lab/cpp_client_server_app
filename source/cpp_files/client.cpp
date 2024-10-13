
#include <unordered_map>

#include "common.hpp"
#include "network.hpp"

class GuessingGameClient : public network::client_interface<MessageTypes> {
public:
    GuessingGameClient() {}

    bool ConnectToServer(const std::string& host, uint16_t port) {
        return Connect(host, port);
    }

    void DisconnectFromServer() {
        Disconnect();
    }

    void SendGuess(uint32_t guess) {
        network::message<MessageTypes> msgReply;
        msgReply.header.id = MessageTypes::Client_TriesToGuess;
        msgReply << guess;
        Send(msgReply);
    }

    void ReceiveMessages() {
        if (!Incoming().empty()) {
            auto msg = Incoming().pop_front().msg;
            switch (msg.header.id) {
                case MessageTypes::Server_StartsNewExperiment:
                    std::cout << "Experiment started! Make a guess." << std::endl;
                    break;
                case MessageTypes::Server_RepliesToClient_NumberTooLow:
                    std::cout << "Your guess is too low." << std::endl;
                    break;
                case MessageTypes::Server_RepliesToClient_NumberTooHigh:
                    std::cout << "Your guess is too high." << std::endl;
                    break;
                case MessageTypes::Server_RepliesToClient_Correct:
                    std::cout << "Congratulations! You've guessed the number!" << std::endl;
                    break;
            }
        }
    }
};

int main() {
    GuessingGameClient client;


    std::string host = "127.0.0.1";  
    uint16_t port = 60000;           
    if (client.ConnectToServer(host, port)) {
        std::cout << "Connected to the server at " << host << ":" << port << std::endl;
    } else {
        std::cerr << "Failed to connect to server!" << std::endl;
        return -1;
    }


    bool experimentOngoing = true;
    while (experimentOngoing) {
        client.ReceiveMessages();  

        std::cout << "Enter your guess: ";
        int guess;
        std::cin >> guess;

        client.SendGuess(guess);  

        client.ReceiveMessages();  
    }

    client.DisconnectFromServer();

    return 0;
}