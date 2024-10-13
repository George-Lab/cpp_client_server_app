#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

namespace network {


template <typename T>
class message_header {
 public:
  T id{};
  uint32_t size = 0;
};

template <typename T>
class message {
 public:
    message_header<T> header{};
  std::vector<uint8_t> body;

    size_t size() const;

      friend std::ostream& operator<<(std::ostream& os, const message<T>& msg);

            
    template <typename DataType>
  friend message<T>& operator<<(message<T>& msg, const DataType& data);

    template <typename DataType>
  friend message<T>& operator>>(message<T>& msg, DataType& data);
};

template <typename T>
class connection;

template <typename T>
class owned_message {
 public:
  std::shared_ptr<connection<T>> remote = nullptr;
  message<T> msg;

    friend std::ostream& operator<<(std::ostream& os,
                                  const owned_message<T>& msg);
};

template <typename T>
class tsqueue {
 public:
  tsqueue() = default;
  tsqueue(const tsqueue<T>&) = delete;
  virtual ~tsqueue() { clear(); }

 public:
    const T& front();

    const T& back();

    T pop_front();

    T pop_back();

    void push_back(const T& item);

    void push_front(const T& item);

    bool empty();

    size_t count();

    void clear();

  void wait();

 protected:
  std::mutex muxQueue;
  std::deque<T> deqQueue;
  std::condition_variable cvBlocking;
  std::mutex muxBlocking;
};

template <typename T>
class server_interface;

template <typename T>
class connection : public std::enable_shared_from_this<connection<T>> {
 public:
      enum class owner { server, client };

 public:
      connection(owner parent, asio::io_context& asioContext,
             asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn);

  virtual ~connection() {}

      uint32_t GetID() const;

 public:
  void ConnectToClient(network::server_interface<T>* server, uint32_t uid = 0);

  void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);

  void Disconnect();

  bool IsConnected() const;

    void StartListening() {}

 public:
      void Send(const message<T>& msg);

 private:
    void WriteHeader();

    void WriteBody();

    void ReadHeader();

    void ReadBody();

    uint64_t scramble(uint64_t nInput);

    void WriteValidation();

  void ReadValidation(network::server_interface<T>* server = nullptr);

    void AddToIncomingMessageQueue();

 protected:
    asio::ip::tcp::socket m_socket;

    asio::io_context& m_asioContext;

      tsqueue<message<T>> m_qMessagesOut;

    tsqueue<owned_message<T>>& m_qMessagesIn;

      message<T> m_msgTemporaryIn;

    owner m_nOwnerType = owner::server;

    uint64_t m_nHandshakeOut = 0;
  uint64_t m_nHandshakeIn = 0;
  uint64_t m_nHandshakeCheck = 0;

  bool m_bValidHandshake = false;
  bool m_bConnectionEstablished = false;

  uint32_t id = 0;
};

template <typename T>
class client_interface {
 public:
  client_interface() {}

  virtual ~client_interface() {
        Disconnect();
  }

 public:
    bool Connect(const std::string& host, const uint16_t port);

    void Disconnect();

    bool IsConnected();

 public:
    void Send(const message<T>& msg);

    tsqueue<owned_message<T>>& Incoming();

 protected:
    asio::io_context m_context;
    std::thread thrContext;
      std::unique_ptr<connection<T>> m_connection;

 private:
    tsqueue<owned_message<T>> m_qMessagesIn;
};

template <typename T>
class server_interface {
 public:
    server_interface(uint16_t port)
      : m_asioAcceptor(m_asioContext,
                       asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

  virtual ~server_interface() {
        Stop();
  }

    bool Start();

    void Stop();

    void WaitForClientConnection();

    void MessageClient(std::shared_ptr<connection<T>> client,
                     const message<T>& msg);

    void MessageAllClients(
      const message<T>& msg,
      std::shared_ptr<connection<T>> pIgnoreClient = nullptr);

    void Update(size_t nMaxMessages = -1, bool bWait = false);

 protected:
    
      virtual bool OnClientConnect(std::shared_ptr<connection<T>> client) {
    return false;
  }

    virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client) {}

    virtual void OnMessage(std::shared_ptr<connection<T>> client,
                         message<T>& msg) {}

 public:
    virtual void OnClientValidated(std::shared_ptr<connection<T>> client) {}

 protected:
    tsqueue<owned_message<T>> m_qMessagesIn;

    std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

      asio::io_context m_asioContext;
  std::thread m_threadContext;

    asio::ip::tcp::acceptor
      m_asioAcceptor;  
    uint32_t nIDCounter = 10000;
};
}  