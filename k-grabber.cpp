/// ./k-grabber "dir for images" "count images" "port"
/// ./k-grabber /mnt/ram_disk 255 31333
/// //////////////////////////////////////////////////

#include <sys/statvfs.h> //для файловой системы
#include "socket_server.hpp"
#include <unistd.h> // close
#include <opencv2/core/mat.hpp> // Mat()
#include <opencv2/opencv.hpp>   // imwrite()

using namespace std;
using namespace cv;

std::vector<uchar> imgBuf;
Mat mainImg;

/////SOCKET ФУНКЦИИ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SocketServer::SocketServer(int port, std::string out_path) :
    image_dims_(cv::Size2i(0, 0)),
    out_path_(out_path),
    client_len_(0),
    server_addr_size_(sizeof(server_addr_)),
    port_(port),
    pic_count_(0),
    sock_fdesc_init_(0),
    sock_fdesc_conn_(0) {
    client_len_ = server_addr_size_;
}

void SocketServer::ConnectToNetwork() {
    sleep(1);

    IsNoErrConnectVS=true;

    sock_fdesc_init_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fdesc_init_ == -1) {
        IsNoErrConnectVS=false;
        shutdown(sock_fdesc_init_, SHUT_RDWR);
        close(sock_fdesc_init_);
    } else {
        int enable = 1;
        setsockopt(sock_fdesc_init_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    }

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = INADDR_ANY;
    server_addr_.sin_port = htons(port_);

    if (IsNoErrConnectVS) {
        if (bind(sock_fdesc_init_, (struct sockaddr*)&server_addr_, server_addr_size_) == -1) {
            IsNoErrConnectVS=false;
            shutdown(sock_fdesc_init_, SHUT_RDWR);
            close(sock_fdesc_init_);
        }
        else {

            if (listen(sock_fdesc_init_, 3) == -1) {
                IsNoErrConnectVS=false;
                shutdown(sock_fdesc_init_, SHUT_RDWR);
                close(sock_fdesc_init_);
            }
            else {
                sock_fdesc_conn_ = accept(sock_fdesc_init_, (struct sockaddr*)&client_addr_, &client_len_);
                if (sock_fdesc_conn_ == -1) {
                    IsNoErrConnectVS=false;
                    shutdown(sock_fdesc_init_, SHUT_RDWR);
                    close(sock_fdesc_init_);
                    shutdown(sock_fdesc_conn_, SHUT_RDWR);
                    close(sock_fdesc_conn_);
                } else {
                    IsNoErrConnectVS=true;
                }
            }
        }
    }
}

void SocketServer::ReceiveVectorSize() {
    ssize_t bytes_sent = 0;
    int vSize = 0;

    size_t sizeof_dimss = sizeof(vSize);

    if (bytes_sent = recv(sock_fdesc_conn_, (char*)&vSize, sizeof_dimss, 0) == -1) {
        IsNoErrConnectVS=false;
    } else {
        vectorSize = (int)vSize;
    }
}

void SocketServer::ReceiveVector(std::vector<uchar> getVector) {
    int num_bytes = 0;

    for (int i = 0; i < vectorSize; i += num_bytes) {
        num_bytes = recv(sock_fdesc_conn_, getVector.data() + i, vectorSize - i, 0);

        if (num_bytes==0) {
            i=vectorSize;
        }

        if (num_bytes == -1) {
            IsNoErrConnectVS=false;
        }
    }

    if (IsNoErrConnectVS)
    {
        imgBuf = getVector;
    }

    std::string sinchro = "1";
    send(sock_fdesc_conn_, sinchro.c_str(), sinchro.length(), 0);
}

/////END SOCKET ФУНКЦИИ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static char path[1000];
int sock;
struct sockaddr_in dest;
std::string path_t;

int main(int argc, char *argv[]){
    int mod, modMax;
    
    strcpy(path, argv[1]);
    strcat(path,"/");
    
    modMax = atoi(argv[2]);
    mod = 0;
    
    int port = atoi(argv[3]);

    std::unique_ptr<SocketServer> server_ptr(new SocketServer(port, ""));
    server_ptr->ConnectToNetwork();

    while(true){

        usleep (3);
        if (server_ptr->IsNoErrConnectVS) {

            server_ptr->ReceiveVectorSize();

            if (server_ptr->vectorSize > 0) {
                imgBuf.resize(server_ptr->vectorSize);
            } else {
                imgBuf.resize(0);
            }

            server_ptr->ReceiveVector(imgBuf);

            if (imgBuf.size() > 0) {
                mainImg = cv::imdecode(imgBuf, CV_LOAD_IMAGE_COLOR);
                path_t = path + to_string(mod) + ".jpg";
                imwrite(path_t, mainImg);

                //std::cout

                mod++;
                if(mod >= modMax) mod = 0;
            } else {
                server_ptr->IsNoErrConnectVS=false;
            }

        } else {
            shutdown(server_ptr->sock_fdesc_init_, SHUT_RDWR);
            close(server_ptr->sock_fdesc_init_);
            shutdown(server_ptr->sock_fdesc_conn_, SHUT_RDWR);
            close(server_ptr->sock_fdesc_conn_);

            server_ptr->ConnectToNetwork();

            server_ptr->IsNoErrConnectVS = true;
        }
    }
}










