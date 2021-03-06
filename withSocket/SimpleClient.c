//-----------------------------------------------------------------------------
// PROGRAMMER   : cucudas0127
// REVISION     : 2020.11.05
// DESCRIPTS    : Simple Socket Client Code 
// Environment ----------------------------------------------------------------
// OS : ubuntu 18.04
//-----------------------------------------------------------------------------
#include "header.h"



// Server's Host Address 
struct sockaddr_in	    HostAddr;                   

//-----------------------------------------------------------------------------
// DESCRIPTS  : Create & Open Socket 
//-----------------------------------------------------------------------------
int CLT_OpenSocket_Client(char *ip_addr,int ip_port)
{
    int socket_fd;
    int itmp;
    int vOpt = 1;       // OptVal = TRUE

    // Create TCP/IP Socket & Init
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);           

    if(socket_fd < 0)    return C_FAIL;

    bzero(&HostAddr, sizeof(struct sockaddr_in));

    HostAddr.sin_family      = AF_INET;
    HostAddr.sin_addr.s_addr = inet_addr(ip_addr);
    HostAddr.sin_port        = htons(ip_port);

    itmp = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &vOpt, sizeof(vOpt));

    if(itmp != 0)  return C_FAIL;  
    
    itmp =connect(socket_fd, (struct sockaddr  *)&HostAddr, sizeof(HostAddr));
    if(itmp != 0 ) return C_FAIL;


    // Open Success
    return socket_fd;

}

//-----------------------------------------------------------------------------
// DESCRIPTS  :Send data to server 
//-----------------------------------------------------------------------------
int CLT_SendData(int sock_fd, char *tx_buf,int tx_len)
{
    int itmp; 

    // Send Packet
    itmp = send(sock_fd, tx_buf, tx_len, MSG_NOSIGNAL);

    // Check Tx Status
    if(itmp < 0)  return C_FAIL;   
    else          return C_SUCCESS; 
}


//-----------------------------------------------------------------------------
// DESCRIPTS  :Recive data, dodnt wait for data to be received
//-----------------------------------------------------------------------------
int CLT_RecivedData_NoWait(int sock_fd, char *rx_buf, int rx_buf_size)
{
    int  	    nData;
    int	      st_socket_size = sizeof(struct sockaddr_in);
    struct    timeval  timeOut;
    fd_set  	readFds ;
    char buf[128];

    memset(rx_buf, 0, rx_buf_size);

    // Set 1ms Timeout counter
    // Just wait 1 msecond and exit
    timeOut.tv_sec  = 0;
    timeOut.tv_usec = 1000;   

    FD_ZERO(&readFds);
    FD_SET(sock_fd, &readFds);


    // if Recivedata Exist
    if(select(sock_fd+1, &readFds, NULL, NULL, &timeOut) > 0) 
    {
        nData = recvfrom(sock_fd, rx_buf, rx_buf_size, 0,
                        (struct sockaddr *)&HostAddr, &st_socket_size);
        return C_SUCCESS;
    }  
    else return C_FAIL;


}

//-----------------------------------------------------------------------------
// Function descripts : Read Buffer,wait for data to be recieved
//-----------------------------------------------------------------------------
void CLT_RecivedData_Wait(int sock_fd, char *rx_buf, int rx_buf_size)
{
    if(sock_fd > 0)   read(sock_fd, rx_buf, rx_buf_size);
    
}

//-----------------------------------------------------------------------------
// DESCRIPTS  :Close the Socket
//-----------------------------------------------------------------------------
void CLT_CloseSocket(int sock_fd)
{
    if(sock_fd > 0 )              
        close(sock_fd);
}


//-----------------------------------------------------------------------------
// DESCRIPTS  :Init Socket
//-----------------------------------------------------------------------------
# define MIN_IP_ADDR_LEN 7
int CLT_Init(char *ip_addr,int ip_len ,int ip_port)
{
    int itmp;

    //check config error
    if(ip_addr==NULL)   { printf("Sever ip config is empty!\n"); return C_FAIL;}
    if(ip_len < MIN_IP_ADDR_LEN) { printf("Wrong server ip!\n"); return C_FAIL;}
    if(ip_len < 0)             { printf("Wrong server port!\n"); return C_FAIL;}
    
     
    // Socket Open
    itmp = CLT_OpenSocket_Client(ip_addr,ip_port);
    if(itmp == C_FAIL) { printf("Socket open Error!\n"); return C_FAIL;}
    
    //register socket file discripter
    return itmp;

}
    

//-----------------------------------------------------------------------------
// DESCRIPTS  :Management Socket
//-----------------------------------------------------------------------------
void  CLT_TaskManager(int sock_fd)
{

    char  rx_buf[MAX_SOCKET_RX_LEN] = {0,};
    int   itmp;


    itmp = CLT_RecivedData_NoWait(sock_fd,rx_buf, MAX_SOCKET_RX_LEN);
    if(itmp ==C_SUCCESS)   printf("RxData : %s",rx_buf);
    else                   return ;

}
