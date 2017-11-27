#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cctype> // isdigit
#include <sys/socket.h>
#include <netinet/in.h>

#include <opencv2/core/core.hpp>

class SocketServer {
 public:
  SocketServer(int port, std::string out_path);
  void ConnectToNetwork();
  void ReceiveImageDims();
  void ReceiveRegSost();
  void ReceiveImage(cv::Mat& image);
  bool ReceiveImageType();
  void ReceiveVectorSize();
  void ReceiveVector(std::vector<uchar> getVector);
  void WriteImage(cv::Mat& image);
  void ShowImage(cv::Mat& image);
  void SendData(std::string datas);
  int GetWidth();
  int GetHeight();
  bool IsNoErrConnectVS;
  int sock_fdesc_init_;
  int sock_fdesc_conn_;
  int vectorSize;

 private:
  cv::Size2i image_dims_;

  struct sockaddr_in server_addr_;
  struct sockaddr_in client_addr_;
  std::string pic_filename_;
  std::string out_path_;
  socklen_t client_len_;
  size_t server_addr_size_;
  int port_;
  int pic_count_;

};

#endif
