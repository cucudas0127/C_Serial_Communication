
//-----------------------------------------------------------------------------
// PROGRAMMER   : cucudas0127
// REVISION     : 2020.11.03
// DESCRIPTS    : Simple Serial Communication with Queue Function.
// Environment Setting
// OS : ubuntu 18.04
//-----------------------------------------------------------------------------


#include "header.h"

// Global variables (STATIC)
//-----------------------------------------------------------------------------
//=============================================================================


//-----------------------------------------------------------------------------
// Function descripts : Send Setting Packet to IR Sensor
//-----------------------------------------------------------------------------
int SendPacket(int com_fd, char* send_packet, int packet_len) 
{   
    int tmp;

    if (com_fd <= 0)         return C_FAIL;
    if (send_packet == NULL) return C_FAIL;
    if (packet_len < 0 )     return C_FAIL;

    tmp  = write(com_fd, send_packet, packet_len);      
    
    if(tmp <= 0) return C_FAIL; 

    //Send Success
    return C_SUCCESS;
}



//-----------------------------------------------------------------------------
// Function descripts : Read Packet (Serial)
//-----------------------------------------------------------------------------
int ReadPacket(int com_fd, Queue *rx_buf)
{        
    int rx_num;
    int i,j;
    char temp_buf[MAX_RX_BUF_SIZE]; // Buffer        
    memset( &temp_buf, 0, sizeof(temp_buf) );

    // Read Data
    if(com_fd > 0 )  rx_num = read(com_fd, temp_buf, sizeof(temp_buf));
    else             return C_FAIL;

    // if Notihing Recived return FAIL
    if(rx_num < 1)   return C_FAIL ;

    for(i=0; i < rx_num; i++) Push_Queue(rx_buf ,temp_buf[i]);
    return rx_num;

}


//-----------------------------------------------------------------------------
// Function descripts : Port Open (Serial)
//-----------------------------------------------------------------------------
int OpenPortSerialPort(char* com_name)
{    
    int com_fd = 0;    
    struct termios  newtio;
    
    com_fd = open(com_name, O_RDWR | O_NOCTTY | O_NONBLOCK );      
    if(0 > com_fd) 
    {    
        printf("Serial Port Open Error..\n"); 
        return C_FAIL; 
    }

    
    ///Init Port
    memset( &newtio, 0, sizeof(newtio) );
    newtio.c_cflag     = BAUDRATE | CS8 | CLOCAL | CREAD ;    
    newtio.c_iflag     = IGNPAR;
    newtio.c_oflag     = 0;
    newtio.c_lflag     = 0; // LF recive filter unused
    newtio.c_cc[VTIME] = 0; // inter charater timer unused
    newtio.c_cc[VMIN]  = 0; // blocking read until 1 character arrives    

    tcflush  (com_fd, TCIFLUSH);
    tcsetattr(com_fd, TCSANOW, &newtio);    

    return com_fd;
}

//-----------------------------------------------------------------------------
// Function descripts : close the port
//-----------------------------------------------------------------------------
int Finalize(int comfd)
{   
    if (comfd > 0) 
    {
        close(comfd);
        return C_SUCCESS;
    }
    else
    {
        return C_FAIL;
    }
}

//-----------------------------------------------------------------------------
// Function descripts : print Recived pakcet
//-----------------------------------------------------------------------------
int Print_RecivePacket(Queue *rx_buf,int rx_len)
{   
    char ctmp;
    int  i;

    printf("Rx Data :");
    for(i = 0; i < rx_len; i++)
    {
        ctmp = Pop_Queue(rx_buf);
        printf("|[%2d] =  %3d|",i,ctmp);
    }
    printf("\n");
}



void main(void)
{
    int  i,itmp;
    int  com_fd;

    Queue rx_buf;
    int  rx_len;

    //---------------------Config--------------------
    // Config Serial Port
    char* port_name = "/dev/ttyS3";

    // Config Send Packet
    char  send_packet[MAX_TX_BUF_SIZE] = {0x01,0x02,0x03};
    int   send_packet_len = 3;

    // if you want changing rx_buf size 
    // you can config {MAX_RX_BUF_SIZE} in header.h
    // Default rxbuf size : 128
    //================================================

    // Open serial Port
    com_fd = OpenPortSerialPort(port_name);
    
    if(com_fd < 0 )   return ;
    else              printf("Serial Port Open Success..\n");

    // replay 5 time for test
    for(i = 0 ; i < 5 ; i++)
    {
        // Send Packet
        itmp = SendPacket(com_fd,send_packet,send_packet_len);
        if(itmp ==C_SUCCESS) printf("Send Packet Success..\n");

        // wait 1sec
        sleep(5);

        // Read Pakcet 
        rx_len = ReadPacket(com_fd,&rx_buf);

        //Output Recived data
        if(rx_len > 0) Print_RecivePacket(&rx_buf,rx_len);
    }
      

    // Colse serial port
    itmp = Finalize(com_fd);
    if(itmp ==C_SUCCESS ) printf("Serial Port Close Success..\n");
}