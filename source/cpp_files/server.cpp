#include <iostream>

#include "common.hpp"
#include "network.hpp"

class Server : public network::server_interface<MessageTypes> {
 public:
  Server(uint16_t nPort) : network::server_interface<MessageTypes>(nPort) {}

  std::vector<uint32_t> m_vGarbageIDs;
  uint32_t hidden_number;

  void StartExperiment(uint32_t numberToGuess) {
    hidden_number = numberToGuess;

    network::message<MessageTypes> msg;
    msg.header.id = MessageTypes::Server_StartsNewExperiment;
    MessageAllClients(msg);

    std::cout << "Experiment started! Hidden number is " << hidden_number
              << std::endl;
  }

 protected:
  bool OnClientConnect(
      std::shared_ptr<network::connection<MessageTypes>> client) override {
        return true;
  }

  void OnClientValidated(
      std::shared_ptr<network::connection<MessageTypes>> client) override {
            network::message<MessageTypes> msg;
    msg.header.id = MessageTypes::Client_Accepted;
    client->Send(msg);
  }

  void OnClientDisconnect(
      std::shared_ptr<network::connection<MessageTypes>> client) override {
    if (client) {
      m_vGarbageIDs.push_back(client->GetID());
    }
  }

  void OnMessage(std::shared_ptr<network::connection<MessageTypes>> client,
                 network::message<MessageTypes>& msg) override {
    if (!m_vGarbageIDs.empty()) {
      for (auto pid : m_vGarbageIDs) {
        std::cout << "Removing " << pid << "\n";
      }
      m_vGarbageIDs.clear();
    }

    switch (msg.header.id) {
      case MessageTypes::Client_RegisterWithServer: {
        network::message<MessageTypes> msgSendID;
        msgSendID.header.id = MessageTypes::Client_AssignID;
        msgSendID << client->GetID();
        MessageClient(client, msgSendID);

        break;
      }

	  case MessageTypes::Client_TriesToGuess: {
		uint32_t client_guess;
		msg >> client_guess;
		if (client_guess < hidden_number) {
			network::message<MessageTypes> msgReply;
			msgReply.header.id = MessageTypes::Server_RepliesToClient_NumberTooLow;
			MessageClient(client, msgReply);
		} else if (client_guess > hidden_number) {
			network::message<MessageTypes> msgReply;
			msgReply.header.id = MessageTypes::Server_RepliesToClient_NumberTooHigh;
			MessageClient(client, msgReply);
		} else {
			network::message<MessageTypes> msgReply;
			msgReply.header.id = MessageTypes::Server_RepliesToClient_Correct;
			MessageClient(client, msgReply);
		}
		break;
	  }

      case MessageTypes::Client_UnregisterWithServer: {
        break;
      }
    }
  }
};

int main() {
  Server server(60000);
  server.Start();
  server.StartExperiment(70);

  while (1) {
    server.Update(-1, true);
  }
  return 0;
}