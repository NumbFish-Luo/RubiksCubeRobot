#pragma once

#include <Windows.h>
#include <fstream>
#include <iostream>

#define BUFFER_SIZE 40000

struct IOBuf // IOTextBuffer
{
    char input[BUFFER_SIZE / 2];
    char output[BUFFER_SIZE];
    char serialBuffer[BUFFER_SIZE / 2];
    bool outputChange;
    bool serialBufferChange;
    size_t  outputIndex;

    void PushBack(const std::string& str);
    void PushBack(const size_t num);
    void PushBack(const int num);
    void Send(const std::string& str);
};
extern IOBuf g_IOBuf; // The golobal variable ȫ�ֱ���

/* A Serial port communication class. This class implements(ʵ����) the basic operation
*  of a serial port, such as listening to specify(ָ����) serial data, send data to the 
*  serial port. (Youdao Translation)
*  Copyright (c) 2009, ���пƼ���ѧtickTick Group����Ȩ������
*  @version 1.0
*  @author  ¬��
*  @E-mail��lujun.hust@gmail.com
*  @date    2010/03/19
*/
class CSerialPort
{
public:
    CSerialPort(void);
    ~CSerialPort(void);
public:  
    /*
    *   @ portNo:       Serial number, the default value is 1, namely(��) COM1. Note that as far as possible don't more than 9!
    *   @ baud:         Baud rate, the default is 115200.
    *   @ parity:       Whether(�Ƿ�) parity(��żУ��), 'Y' said need parity, 'N' said don't need to parity.
    *   @ databits:     The number of data bits, the default value is 8 bits.
    *   @ stopsbits:    "Stop bit" format, the default value is 1.
    *   @ return bool:  Initialize success?
    *   @ <NOTE>:       Before using other function of this class, please call this function to initialize serial port.
    *                   This function provides(�ṩ) some commonly used serial port paramenters(����) setting, 
    *                   if you need to set a detailed(��ϸ��) DCB paramenters, overloaded(����) function can be used.
    *                   This serial port class destructor will shut down automatically, no additional(ע��) execution(ִ��) close the serial port.
    */
    bool InitPort(UINT portNo = 1, UINT baud = CBR_115200, char parity = 'N', UINT databits = 8, UINT stopsbits = 1);
    /* This function provide(�ṩ) direct(ֱ��) according to DCB paramenters set serial port, 
        and provides the user a custom to a serial port initialization parameter. */
    bool InitPort(UINT portNo, const LPDCB& plDCB);
    /*
    *   @ return bool:  The operation is successful?
    *   @ <NOTE>:       This listener thread to complete the serial data to monitor(���), and will receive the data output to the screen printing. 
    *                   When a thread is in an open state, returns a flase.
    */
    bool OpenListenThread();
    // return bool:  The operation is successful?
    bool CloseListenTread();
    // return bool:  The operation is successful?
    bool WriteData(unsigned char* pData, unsigned int length);
    // Get serial number of bytes in buffer
    UINT GetBytesInCOM();
    // return bool:  The operation is successful?
    bool ReadChar(char &cRecved);
    void InitSerialPortData();
private:
    // return bool:  The operation is successful?
    bool openPort(UINT  portNo);
    void ClosePort();
    /*
    *   @ pParam:               The thread parameter
    *   @ return UINT WINAPI:   The thread return value
    *   @ <NOTE>:               To monitor data and information from serial port
    */
    static UINT WINAPI ListenThread(void* pParam);
private:
    HANDLE           m_hComm;               // Serial port handle
    static   bool    s_bExit;               // Thread exit flag
    volatile HANDLE  m_hListenThread;       // Thread handle
    CRITICAL_SECTION m_csCommunicationSync; // Mutex(����) serial operation, synchronous(ͬ��) mutex, critical area(�ٽ���) protection
};
